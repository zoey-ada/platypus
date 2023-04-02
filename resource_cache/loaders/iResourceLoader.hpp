#pragma once

#include <memory>

#include "../resources/resourceType.hpp"

class IResourceStore;
class Resource;
class ResourceHandle;

class IResourceLoader
{
public:
	virtual ~IResourceLoader() = default;

	virtual std::string getPattern() = 0;
	virtual ResourceType getType() = 0;
	virtual std::shared_ptr<Resource> load(const char* resource_id, const char* store_id,
		std::byte* resource_data, const uint64_t data_size) = 0;

protected:
	virtual uint8_t* allocate(unsigned int size) = 0;
};
