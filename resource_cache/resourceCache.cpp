#include "resourceCache.hpp"

#include <renderer/iRenderer.hpp>
#include <utilities/logging/logger.hpp>

#include "loaders/directXMeshLoader.hpp"
#include "loaders/directXPixelShaderLoader.hpp"
#include "loaders/directXTextureLoader.hpp"
#include "loaders/directXVertexShaderLoader.hpp"
#include "loaders/iResourceLoader.hpp"
#include "resources/meshResource.hpp"
#include "resources/pixelShaderResource.hpp"
#include "resources/resource.hpp"
#include "resources/resourceType.hpp"
#include "resources/textureResource.hpp"
#include "resources/vertexShaderResource.hpp"
#include "stores/iResourceStore.hpp"
#include "stores/zipResourceStore.hpp"

const unsigned int bytes_in_kilobyte = 1024;
const unsigned int bytes_in_megabyte = bytes_in_kilobyte * 1024;

ResourceCache::ResourceCache(const platypus::ResourceCacheSettings& settings)
	: _cache_size(settings.cache_size_in_mb() * bytes_in_megabyte), _allocated(0)
{
	logInfo("resource cache created with '" + std::to_string(_cache_size) + "' bytes of memory",
		"resource_cache");

	this->_store_locations.resize(static_cast<size_t>(settings.store_locations_size()));
	for (const auto& location : settings.store_locations())
	{
		this->_store_locations.push_back(location);
	}
}

ResourceCache::~ResourceCache()
{
	this->flush();
	logInfo("resource cache destroyed", "resource_cache");
}

bool ResourceCache::initialize(const std::shared_ptr<IRenderer>& renderer)
{
	auto cache = this->shared_from_this();

	this->registerLoader(std::make_shared<DirectXVertexShaderLoader>(cache, renderer));
	this->registerLoader(std::make_shared<DirectXPixelShaderLoader>(cache, renderer));
	this->registerLoader(std::make_shared<DirectXTextureLoader>(cache, renderer));
	this->registerLoader(std::make_shared<DirectXMeshLoader>(cache, renderer));

	return true;
}

void ResourceCache::registerLoader(const std::shared_ptr<IResourceLoader>& loader)
{
	this->_resource_loaders[loader->getType()] = loader;
}

std::shared_ptr<Resource> ResourceCache::getResource(const ResourceType& type,
	const std::string& path)
{
	logDebug("attempting to get resource " + path, "resource_cache");
	auto resource = this->tryShareResource(type, path);

	if (resource != nullptr)
	{
		logDebug("resource was found in the cache", "resource_cache");
		this->updateResource(resource);
	}
	else
	{
		logVerbose("resource was not in the cache", "resource_cache");
		resource = this->loadResource(type, path);
	}

	return resource;
}

std::shared_ptr<PixelShaderResource> ResourceCache::getPixelShader(const std::string& path)
{
	return std::dynamic_pointer_cast<PixelShaderResource>(
		this->getResource(ResourceType::PixelShader, path));
}

std::shared_ptr<VertexShaderResource> ResourceCache::getVertexShader(const std::string& path)
{
	return std::dynamic_pointer_cast<VertexShaderResource>(
		this->getResource(ResourceType::VertexShader, path));
}

std::shared_ptr<TextureResource> ResourceCache::getTexture(const std::string& path)
{
	return std::dynamic_pointer_cast<TextureResource>(
		this->getResource(ResourceType::Texture, path));
}

std::shared_ptr<MeshResource> ResourceCache::getMesh(const std::string& path)
{
	return std::dynamic_pointer_cast<MeshResource>(this->getResource(ResourceType::Mesh, path));
}

bool ResourceCache::exists(const ResourceType& type, const std::string& path) const
{
	return this->tryShareResource(type, path) != nullptr;
}

bool ResourceCache::addResource(const std::shared_ptr<Resource>& resource)
{
	if (resource == nullptr)
		return false;

	if (!this->makeRoom(resource->size()))
		return false;

	this->_allocated += resource->size();
	this->_resources[resource->type()][resource->path()] = resource;
	this->_recently_used.push_front(resource);

	return true;
}

void ResourceCache::flush()
{
	logInfo("clearing all cached files", "resource_cache");

	while (!this->_recently_used.empty())
		this->freeOneResource();
}

std::shared_ptr<IResourceStore> ResourceCache::getStore(const std::string& store)
{
	logDebug("searching for resource store " + store, "resource_cache");

	auto found_store = this->_stores.find(store);
	if (found_store != this->_stores.end())
	{
		logDebug("found resource store " + store, "resource_cache");
		return found_store->second;
	}

	logDebug("failed to find resource store " + store, "resource_cache");

	auto new_store = std::make_shared<ZipResourceStore>(store);
	if (new_store->open())
	{
		logDebug("successfully loaded resource store " + store, "resource_cache");
		_stores[store] = new_store;
		return new_store;
	}

	return nullptr;
}

std::shared_ptr<IResourceLoader> ResourceCache::getLoader(const ResourceType& type) const
{
	logDebug("searching for loader for " + std::to_string(type), "resource_cache");

	try
	{
		return _resource_loaders.at(type);
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}
}

std::shared_ptr<Resource> ResourceCache::tryShareResource(const ResourceType& type,
	const std::string& path) const
{
	auto resource_map = this->_resources.find(type);
	if (resource_map != this->_resources.end())
	{
		auto resource = resource_map->second.find(path);
		if (resource != resource_map->second.end())
			return resource->second;
	}

	return nullptr;
}

std::shared_ptr<Resource> ResourceCache::loadResource(const ResourceType& type,
	const std::string& path)
{
	auto index = path.find(".zip");
	auto store = path.substr(0, index + 4);
	auto name = path.substr(index + 5);

	auto loader = this->getLoader(type);
	if (loader == nullptr)
	{
		logWarning("failed to find a loader for " + std::to_string(type), "resource_cache");
		return nullptr;
	}

	auto resource_store = this->getStore(store);
	if (resource_store == nullptr)
	{
		logWarning("failed to find resource store " + store, "resource_cache");
		return nullptr;
	}

	auto resource = loader->load(resource_store, name);
	if (resource == nullptr)
	{
		logWarning("failed to load resource " + path, "resource_cache");
		return nullptr;
	}

	logVerbose("successfully loaded resource " + path, "resource_cache");

	this->_resources[type][path] = resource;
	this->_recently_used.push_front(resource);
	return resource;
}

void ResourceCache::updateResource(const std::shared_ptr<Resource>& resource)
{
	logDebug("moving " + resource->path() + " to the front of the recently used list",
		"resource_cache");
	this->_recently_used.remove(resource);
	this->_recently_used.push_front(resource);
}

void ResourceCache::free(const std::shared_ptr<Resource>& resource)
{
	logVerbose("removing " + resource->path() + " (" + std::to_string(resource->size()) +
			") from the cache",
		"resource_cache");

	auto type = resource->type();
	_resources[type].erase(resource->path());
	this->_recently_used.remove(resource);

	if (_resources[type].empty())
		_resources.erase(type);
}

bool ResourceCache::makeRoom(const uint64_t size)
{
	logDebug("attempting to find " + std::to_string(size) + " bytes of memory", "resource_cache");

	if (size > this->_cache_size)
	{
		logWarning("the cache is smaller than " + std::to_string(size) + " bytes",
			"resource_cache");
		return false;
	}

	while (size > (this->_cache_size - this->_allocated))
	{
		if (this->_recently_used.empty())
		{
			logWarning("unable to find " + std::to_string(size) + " bytes of memory",
				"resource_cache");
			return false;
		}

		this->freeOneResource();
	}

	return true;
}

uint8_t* ResourceCache::allocate(const uint64_t size)
{
	logDebug("attemptying to allocate " + std::to_string(size) + " bytes of memory",
		"resource_cache");

	if (!this->makeRoom(size))
		return nullptr;

	auto* memory = new (std::nothrow) uint8_t[size];
	if (memory != nullptr)
	{
		logDebug(std::to_string(size) + " bytes allocated", "resource_cache");
	}

	return memory;
}

void ResourceCache::freeOneResource()
{
	this->free(this->_recently_used.back());
}

void ResourceCache::memoryHasBeenFreed(const uint64_t size)
{
	logDebug(std::to_string(size) + " bytes freed", "resource_cache");
	this->_allocated -= size;
}
