#include "resource.hpp"

Resource::Resource(std::string name, uint8_t* buffer, uint64_t size,
	std::shared_ptr<IResourceStore> store, std::shared_ptr<ResourceCache> cache)
	: _name(std::move(name)),
	  _buffer(buffer),
	  _size(size),
	  _store(std::move(store)),
	  _cache(std::move(cache))
{}

Resource::~Resource()
{
	if (_buffer != nullptr)
		delete[](_buffer);
	_buffer = nullptr;

	// _cache->memoryHasBeenFreed(_size);
}

std::string Resource::path()
{
	// if (_archive)
	// 	return _archive->getArchivePath() + std::string("/") + _name;

	return _name;
}
