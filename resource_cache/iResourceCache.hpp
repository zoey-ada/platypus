#pragma once

#include <cstdint>

class IResourceCache
{
public:
	virtual ~IResourceCache() = default;

	virtual uint8_t* allocate(const uint64_t size) = 0;
};
