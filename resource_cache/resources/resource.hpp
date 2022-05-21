#pragma once

#include <memory>
#include <string>

#include "resourceType.hpp"

class IResourceStore;
class ResourceCache;

struct PtResourceData
{
	std::string name;
	uint8_t* buffer;
	uint64_t size;
	std::shared_ptr<IResourceStore> store;
	std::shared_ptr<ResourceCache> cache;
};

class Resource
{
public:
	Resource(PtResourceData* resource_data);

	virtual ~Resource();

	[[nodiscard]] std::string path();

	[[nodiscard]] inline std::shared_ptr<IResourceStore> store() const { return _store; }
	[[nodiscard]] inline std::string name() const { return _name; }
	[[nodiscard]] inline uint64_t size() const { return _size; }
	[[nodiscard]] inline uint8_t* buffer() const { return _buffer; }
	[[nodiscard]] inline uint8_t* writableBuffer() const { return _buffer; }
	[[nodiscard]] virtual ResourceType type() const { return ResourceType::General; }

protected:
	std::string _name;
	uint8_t* _buffer {nullptr};
	uint64_t _size {0};
	std::shared_ptr<IResourceStore> _store {nullptr};
	std::shared_ptr<ResourceCache> _cache {nullptr};
};
