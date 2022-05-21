#include "resource.hpp"

Resource::Resource(PtResourceData* resource_data)
	: _name(std::move(resource_data->name)),
	  _buffer(resource_data->buffer),
	  _size(resource_data->size),
	  _store(std::move(resource_data->store)),
	  _cache(std::move(resource_data->cache))
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
