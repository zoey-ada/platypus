#include "resourceCache.hpp"

#include <renderer/iRenderer.hpp>
#include <serviceProvider.hpp>
#include <utilities/logging/iLoggingSystem.hpp>

#include "loaders/iResourceLoader.hpp"
#include "resources/audioResource.hpp"
#include "resources/meshResource.hpp"
#include "resources/pixelShaderResource.hpp"
#include "resources/protobufResource.hpp"
#include "resources/resource.hpp"
#include "resources/resourceType.hpp"
#include "resources/textureResource.hpp"
#include "resources/vertexShaderResource.hpp"
#include "stores/iResourceStore.hpp"

namespace platypus
{

const unsigned int bytes_in_kilobyte = 1024;
const unsigned int bytes_in_megabyte = bytes_in_kilobyte * 1024;

ResourceCache::ResourceCache(const uint32_t cache_size_in_mb,
	const std::shared_ptr<ILoggingSystem>& logging)
	: _logging(logging), _cache_size(cache_size_in_mb * bytes_in_megabyte)
{
	this->_logging->info("resource_cache",
		"resource cache created with '" + std::to_string(_cache_size) + "' bytes of memory");
}

ResourceCache::~ResourceCache()
{
	this->flush();
	this->_logging->info("resource_cache", "resource cache destroyed");
}

bool ResourceCache::initialize(const std::list<std::shared_ptr<IResourceStore>>& resource_stores)
{
	for (auto& store : resource_stores)
		this->_stores[store->identifier()] = store;

	return true;
}

void ResourceCache::registerLoader(const std::shared_ptr<IResourceLoader>& loader)
{
	this->_resource_loaders[loader->getType()] = loader;
}

std::shared_ptr<Resource> ResourceCache::getResource(const ResourceType& type,
	const std::string& path)
{
	this->_logging->debug("resource_cache", "attempting to get resource " + path);
	auto resource = this->tryShareResource(type, path);

	if (resource != nullptr)
	{
		this->_logging->debug("resource_cache", "resource was found in the cache");
		this->updateResource(resource);
	}
	else
	{
		this->_logging->verbose("resource_cache", "resource was not in the cache");
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

std::shared_ptr<AudioResource> ResourceCache::getAudio(const std::string& path)
{
	return std::dynamic_pointer_cast<AudioResource>(this->getResource(ResourceType::Audio, path));
}

std::shared_ptr<ProtobufResource> ResourceCache::getProtobuf(const std::string& path)
{
	return std::dynamic_pointer_cast<ProtobufResource>(
		this->getResource(ResourceType::Protobuf, path));
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
	this->_resources[resource->type()][resource->id()] = resource;
	this->_recently_used.push_front(resource);

	return true;
}

void ResourceCache::touchResource(const ResourceType& type, const std::string& resource_id)
{
	this->_logging->debug("resource_cache", "attempting to touch resource " + resource_id);
	auto resource = this->tryShareResource(type, resource_id);

	if (resource != nullptr)
	{
		this->_logging->debug("resource_cache", "resource was found in the cache");
		this->updateResource(resource);
	}
	else
	{
		this->_logging->warning("resource_cache", "resource was not in the cache");
	}
}

void ResourceCache::flush()
{
	this->_logging->info("resource_cache", "clearing all cached files");

	while (!this->_recently_used.empty())
		this->freeOneResource();
}

std::shared_ptr<IResourceStore> ResourceCache::getStore(const std::string& identifier)
{
	this->_logging->debug("resource_cache", "searching for resource store " + identifier);

	auto found_store = this->_stores.find(identifier);
	if (found_store == this->_stores.end())
	{
		this->_logging->warning("resource_cache", "unable to find resource store " + identifier);
		return nullptr;
	}

	auto store = found_store->second;
	if (!store->open())
	{
		this->_logging->warning("resource_cache", "unable to open resource store " + identifier);
		return nullptr;
	}

	return store;
}

std::shared_ptr<IResourceLoader> ResourceCache::getLoader(const ResourceType& type) const
{
	this->_logging->debug("resource_cache", "searching for loader for " + std::to_string(type));

	try
	{
		return _resource_loaders.at(type);
	}
	catch (std::out_of_range)
	{
		this->_logging->warning("resource_cache",
			"unable to find resource loader for " + std::to_string(type));
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
		return nullptr;
	}

	auto resource_store = this->getStore(store);
	if (resource_store == nullptr)
	{
		return nullptr;
	}

	auto resource_data = this->loadResourceData(name.c_str(), resource_store);
	if (resource_data == nullptr)
	{
		this->_logging->warning("resource_cache",
			"unable to find resource " + name + " in store " + store);
		return nullptr;
	}

	auto data_size = resource_store->getResourceSize(name);
	auto resource = loader->load(name.c_str(), store.c_str(), resource_data, data_size);
	if (resource == nullptr)
	{
		this->_logging->warning("resource_cache", "failed to load resource " + path);
		return nullptr;
	}

	delete[] resource_data;
	resource_data = nullptr;

	this->_logging->verbose("resource_cache", "successfully loaded resource " + path);

	this->_resources[type][path] = resource;
	this->_recently_used.push_front(resource);
	return resource;
}

std::byte* ResourceCache::loadResourceData(const char* relative_filepath,
	const std::shared_ptr<IResourceStore>& store)
{
	auto size = store->getResourceSize(relative_filepath);
	auto buffer = new (std::nothrow) std::byte[size];

	if (buffer == nullptr)
	{
		this->_logging->warning("resource_cache", "unable to get memory from the system");
		return nullptr;
	}

	if (!store->getResource(relative_filepath, (std::uint8_t*)buffer))
	{
		return nullptr;
	}

	return buffer;
}

void ResourceCache::updateResource(const std::shared_ptr<Resource>& resource)
{
	this->_logging->debug("resource_cache",
		"moving " + std::string(resource->id()) + " to the front of the recently used list");

	this->_recently_used.remove(resource);
	this->_recently_used.push_front(resource);
}

void ResourceCache::free(const std::shared_ptr<Resource>& resource)
{
	this->_logging->verbose("resource_cache",
		"removing " + std::string(resource->id()) + " (" + std::to_string(resource->size()) +
			") from the cache");

	auto type = resource->type();
	_resources[type].erase(resource->id());
	this->_recently_used.remove(resource);

	if (_resources[type].empty())
		_resources.erase(type);
}

bool ResourceCache::makeRoom(const uint64_t size)
{
	this->_logging->debug("resource_cache",
		"attempting to find " + std::to_string(size) + " bytes of memory");

	if (size > this->_cache_size)
	{
		this->_logging->warning("resource_cache",
			"the cache is smaller than " + std::to_string(size) + " bytes");
		return false;
	}

	while (size > (this->_cache_size - this->_allocated))
	{
		if (this->_recently_used.empty())
		{
			this->_logging->warning("resource_cache",
				"unable to find " + std::to_string(size) + " bytes of memory");
			return false;
		}

		this->freeOneResource();
	}

	return true;
}

uint8_t* ResourceCache::allocate(const uint64_t size)
{
	this->_logging->debug("resource_cache",
		"attemptying to allocate " + std::to_string(size) + " bytes of memory");

	if (!this->makeRoom(size))
		return nullptr;

	auto* memory = new (std::nothrow) uint8_t[size];
	if (memory != nullptr)
	{
		this->_logging->debug("resource_cache", std::to_string(size) + " bytes allocated");
	}

	return memory;
}

void ResourceCache::freeOneResource()
{
	this->free(this->_recently_used.back());
}

void ResourceCache::memoryHasBeenFreed(const uint64_t size)
{
	this->_logging->debug("resource_cache", std::to_string(size) + " bytes freed");
	this->_allocated -= size;
}

};
