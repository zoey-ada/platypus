#pragma once

#include <memory>

#include "../resources/resourceType.hpp"

class IResourceStore;
class Resource;
class ResourceCache;
class ResourceHandle;

class IResourceLoader
{
public:
	virtual ~IResourceLoader() = default;

	virtual std::string getPattern() = 0;
	virtual ResourceType getType() = 0;
	virtual std::shared_ptr<Resource> load(const std::shared_ptr<IResourceStore>& store,
		const std::string& filename) = 0;

protected:
	virtual uint8_t* allocate(unsigned int size) = 0;
};
