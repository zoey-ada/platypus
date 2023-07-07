#pragma once

#include <resource_cache/iResourceCache.hpp>

class MockResourceCache: public IResourceCache
{
public:
	virtual ~MockResourceCache() = default;

	bool initialize(const std::list<std::shared_ptr<IResourceStore>>& resource_stores) override
	{
		return true;
	}

	void registerLoader(const std::shared_ptr<platypus::IResourceLoader>& /*loader*/) override {}

	std::shared_ptr<Resource> getResource(const ResourceType& type,
		const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<PixelShaderResource> getPixelShader(const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<VertexShaderResource> getVertexShader(const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<TextureResource> getTexture(const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<MeshResource> getMesh(const std::string& path) override { return nullptr; }
	std::shared_ptr<AudioResource> getAudio(const std::string& path) override { return nullptr; }
	std::shared_ptr<platypus::ProtobufResource> getProtobuf(const std::string& path) override
	{
		return nullptr;
	}

	bool exists(const ResourceType& type, const std::string& path) const override { return true; }

	bool addResource(const std::shared_ptr<Resource>& resource) override { return false; }

	void touchResource(const ResourceType& type, const std::string& resource_id) override {}

	void flush() override {}

	uint8_t* allocate(const uint64_t size) override { return this->_allocated_data; }

	uint8_t* _allocated_data {nullptr};
};
