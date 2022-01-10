#pragma once

#include <memory>
#include <string>

#include "resourceType.hpp"

class IResourceStore;
class ResourceCache;

class Resource
{
public:
	Resource(std::string name, uint8_t* buffer, unsigned int size,
		std::shared_ptr<IResourceStore> store,
		std::shared_ptr<ResourceCache> cache);

	virtual ~Resource();

	[[nodiscard]] std::string path();

	[[nodiscard]] inline std::shared_ptr<IResourceStore> store() const { return _store; }
	[[nodiscard]] inline std::string name() const { return _name; }
	[[nodiscard]] inline unsigned int size() const { return _size; }
	[[nodiscard]] inline uint8_t* buffer() const { return _buffer; }
	[[nodiscard]] inline uint8_t* writableBuffer() const { return _buffer; }
	[[nodiscard]] virtual ResourceType type() const { return ResourceType::General; }

protected:
	std::string _name;
	uint8_t* _buffer;
	unsigned int _size;
	std::shared_ptr<IResourceStore> _store;
	std::shared_ptr<ResourceCache> _cache;
};
