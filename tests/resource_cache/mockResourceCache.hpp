#pragma once

#include <resource_cache/iResourceCache.hpp>

class MockResourceCache: public IResourceCache
{
public:
	virtual ~MockResourceCache() = default;

	uint8_t* allocate(const uint64_t size) override { return this->_allocated_data; }

	uint8_t* _allocated_data {nullptr};
};
