#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>

#include <platypus_proto/settings.hpp>

#include "resources/resourceType.hpp"

class IRenderer;
class IResourceStore;
class IResourceLoader;
class Resource;
class PixelShaderResource;
class VertexShaderResource;
class TextureResource;
class MeshResource;

using ResourceList = std::list<std::shared_ptr<Resource>>;
using ResourceMap = std::map<std::string, std::shared_ptr<Resource>>;
using ResourceLoaderMap = std::map<ResourceType, std::shared_ptr<IResourceLoader>>;
using ResourceStoreMap = std::map<std::string, std::shared_ptr<IResourceStore>>;

class ResourceCache: public std::enable_shared_from_this<ResourceCache>
{
public:
	explicit ResourceCache(const platypus::ResourceCacheSettings& settings);
	virtual ~ResourceCache();

	bool initialize(const std::shared_ptr<IRenderer>& renderer);

	void registerLoader(const std::shared_ptr<IResourceLoader>& loader);

	std::shared_ptr<Resource> getResource(const ResourceType& type, const std::string& path);
	std::shared_ptr<PixelShaderResource> getPixelShader(const std::string& path);
	std::shared_ptr<VertexShaderResource> getVertexShader(const std::string& path);
	std::shared_ptr<TextureResource> getTexture(const std::string& path);
	std::shared_ptr<MeshResource> getMesh(const std::string& path);

	bool exists(const ResourceType& type, const std::string& path) const;
	bool addResource(const std::shared_ptr<Resource>& resource);

	void flush();
	uint8_t* allocate(const uint64_t size);

protected:
	std::shared_ptr<IResourceStore> getStore(const std::string& store);
	std::shared_ptr<IResourceLoader> getLoader(const ResourceType& type) const;

	std::shared_ptr<Resource> tryShareResource(const ResourceType& type,
		const std::string& path) const;

	std::shared_ptr<Resource> loadResource(const ResourceType& type, const std::string& path);

	void updateResource(const std::shared_ptr<Resource>& resource);
	void free(const std::shared_ptr<Resource>& source);

	bool makeRoom(const uint64_t size);
	void freeOneResource();
	void memoryHasBeenFreed(const uint64_t size);

private:
	ResourceList _recently_used;
	std::map<ResourceType, ResourceMap> _resources;
	ResourceLoaderMap _resource_loaders;
	ResourceStoreMap _stores;

	uint64_t _cache_size;
	uint64_t _allocated;
	std::list<std::string> _store_locations;
};
