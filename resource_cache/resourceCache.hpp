#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>

#include <platypus_proto/settings.hpp>

#include "iResourceCache.hpp"
#include "resources/resourceType.hpp"

class IRenderer;
class ILoggingSystem;

namespace platypus
{

using ResourceList = std::list<std::shared_ptr<Resource>>;
using ResourceMap = std::map<std::string, std::shared_ptr<Resource>>;
using ResourceLoaderMap = std::map<ResourceType, std::shared_ptr<platypus::IResourceLoader>>;
using ResourceStoreMap = std::map<std::string, std::shared_ptr<platypus::IResourceStore>>;

class ResourceCache: public platypus::IResourceCache,
					 public std::enable_shared_from_this<ResourceCache>
{
public:
	explicit ResourceCache(const uint32_t cache_size_in_mb,
		const std::shared_ptr<ILoggingSystem>& logging);

	virtual ~ResourceCache();

	bool initialize(
		const std::list<std::shared_ptr<platypus::IResourceStore>>& resource_stores) override;

	void registerLoader(const std::shared_ptr<platypus::IResourceLoader>& loader) override;

	std::shared_ptr<Resource> getResource(const ResourceType& type,
		const std::string& path) override;
	std::shared_ptr<PixelShaderResource> getPixelShader(const std::string& path) override;
	std::shared_ptr<VertexShaderResource> getVertexShader(const std::string& path) override;
	std::shared_ptr<TextureResource> getTexture(const std::string& path) override;
	std::shared_ptr<MeshResource> getMesh(const std::string& path) override;
	std::shared_ptr<AudioResource> getAudio(const std::string& path) override;
	std::shared_ptr<platypus::ProtobufResource> getProtobuf(const std::string& path) override;

	bool exists(const ResourceType& type, const std::string& path) const override;
	bool addResource(const std::shared_ptr<Resource>& resource) override;
	void touchResource(const ResourceType& type, const std::string& resource_id) override;

	void flush() override;
	uint8_t* allocate(const uint64_t size) override;

protected:
	std::shared_ptr<platypus::IResourceStore> getStore(const std::string& store);
	std::shared_ptr<platypus::IResourceLoader> getLoader(const ResourceType& type) const;

	std::shared_ptr<Resource> tryShareResource(const ResourceType& type,
		const std::string& path) const;

	std::shared_ptr<Resource> loadResource(const ResourceType& type, const std::string& path);
	std::byte* loadResourceData(const char* relative_filepath,
		const std::shared_ptr<platypus::IResourceStore>& store);

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
	std::shared_ptr<ILoggingSystem> _logging {nullptr};

	uint64_t _cache_size;
	uint64_t _allocated {0};
};

};
