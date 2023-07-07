#pragma once

#include <resource_cache/loaders/iResourceLoader.hpp>

class MockResourceLoader: public platypus::IResourceLoader
{
public:
	virtual ~MockResourceLoader() = default;

	std::string getPattern() override { return this->_pattern; }

	ResourceType getType() override { return this->_resource_type; }

	std::shared_ptr<Resource> load(const char* resource_id, const char* store_id,
		std::byte* resource_data, const uint64_t data_size) override
	{
		return this->_resource;
	}

	uint8_t* allocate(unsigned int size) override { return this->_data; }

	std::string _pattern {"*"};
	ResourceType _resource_type {ResourceType::General};
	std::shared_ptr<Resource> _resource {nullptr};
	uint8_t* _data {nullptr};
};
