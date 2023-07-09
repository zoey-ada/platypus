#pragma once

#include <resource_cache/resources/resource.hpp>

using namespace platypus;

class MockResource: public Resource
{
public:
	MockResource(): Resource(ResourceData {"", "", 0}) {}

	virtual ~MockResource() = default;

	ResourceType type() const override { return this->_resource_type; }

	void setResourceId(std::string resource_id) { this->_resource_id = resource_id; }
	void setStoreId(std::string store_id) { this->_store_id = store_id; }
	void setSize(uint64_t size) { this->_size = size; }

	ResourceType _resource_type {ResourceType::General};
};
