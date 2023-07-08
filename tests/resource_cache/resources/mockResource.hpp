#pragma once

#include <resource_cache/resources/resource.hpp>

class MockResource: public platypus::Resource
{
public:
	MockResource(): Resource(PtResourceData {"", "", 0}) {}

	virtual ~MockResource() = default;

	platypus::ResourceType type() const override { return this->_resource_type; }

	void setResourceId(std::string resource_id) { this->_resource_id = resource_id; }
	void setStoreId(std::string store_id) { this->_store_id = store_id; }
	void setSize(uint64_t size) { this->_size = size; }

	platypus::ResourceType _resource_type {platypus::ResourceType::General};
};
