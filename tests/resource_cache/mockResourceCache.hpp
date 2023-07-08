#pragma once

#include <resource_cache/iResourceCache.hpp>

class MockResourceCache: public platypus::IResourceCache
{
public:
	virtual ~MockResourceCache() = default;

	bool initialize(
		const std::list<std::shared_ptr<platypus::IResourceStore>>& resource_stores) override
	{
		return true;
	}

	void registerLoader(const std::shared_ptr<platypus::IResourceLoader>& /*loader*/) override {}

	std::shared_ptr<platypus::Resource> getResource(const platypus::ResourceType& type,
		const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<platypus::PixelShaderResource> getPixelShader(const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<platypus::VertexShaderResource> getVertexShader(
		const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<platypus::TextureResource> getTexture(const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<platypus::MeshResource> getMesh(const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<platypus::AudioResource> getAudio(const std::string& path) override
	{
		return nullptr;
	}
	std::shared_ptr<platypus::ProtobufResource> getProtobuf(const std::string& path) override
	{
		return nullptr;
	}

	bool exists(const platypus::ResourceType& type, const std::string& path) const override
	{
		return true;
	}

	bool addResource(const std::shared_ptr<platypus::Resource>& resource) override { return false; }

	void touchResource(const platypus::ResourceType& type, const std::string& resource_id) override
	{}

	void flush() override {}

	uint8_t* allocate(const uint64_t size) override { return this->_allocated_data; }

	uint8_t* _allocated_data {nullptr};
};
