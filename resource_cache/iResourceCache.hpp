#pragma once

#include <cstdint>
#include <list>

#include "resources/resourceType.hpp"

class IResourceStore;
class IResourceLoader;

class Resource;
class AudioResource;
class MeshResource;
class PixelShaderResource;
class TextureResource;
class VertexShaderResource;

namespace platypus
{
class ProtobufResource;
};

class IResourceCache
{
public:
	virtual ~IResourceCache() = default;

	virtual bool initialize(const std::list<std::shared_ptr<IResourceStore>>& resource_stores) = 0;

	virtual void registerLoader(const std::shared_ptr<IResourceLoader>& loader) = 0;

	virtual std::shared_ptr<Resource> getResource(const ResourceType& type,
		const std::string& path) = 0;
	virtual std::shared_ptr<PixelShaderResource> getPixelShader(const std::string& path) = 0;
	virtual std::shared_ptr<VertexShaderResource> getVertexShader(const std::string& path) = 0;
	virtual std::shared_ptr<TextureResource> getTexture(const std::string& path) = 0;
	virtual std::shared_ptr<MeshResource> getMesh(const std::string& path) = 0;
	virtual std::shared_ptr<AudioResource> getAudio(const std::string& path) = 0;
	virtual std::shared_ptr<platypus::ProtobufResource> getProtobuf(const std::string& path) = 0;

	virtual bool exists(const ResourceType& type, const std::string& path) const = 0;
	virtual bool addResource(const std::shared_ptr<Resource>& resource) = 0;
	virtual void touchResource(const ResourceType& type, const std::string& resource_id) = 0;

	virtual void flush() = 0;
	virtual uint8_t* allocate(const uint64_t size) = 0;
};
