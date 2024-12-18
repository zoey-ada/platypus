#pragma once

#include "resourceType.hpp"

namespace platypus
{

struct ResourceData
{
	const char* resource_id;
	const char* store_id;
	uint64_t size;
};

class Resource
{
public:
	Resource(ResourceData* resource_data)
		: _resource_id(resource_data->resource_id),
		  _store_id(resource_data->store_id),
		  _size(resource_data->size)
	{}

	virtual ~Resource() = default;

	// [[nodiscard]] std::string path();

	[[nodiscard]] virtual ResourceType type() const { return ResourceType::General; }

	[[nodiscard]] inline const char* id() const { return _resource_id.c_str(); }
	[[nodiscard]] inline const char* store_id() const { return _store_id.c_str(); }
	[[nodiscard]] inline uint64_t size() const { return _size; }

protected:
	std::string _resource_id;
	std::string _store_id;
	uint64_t _size {0};

	Resource(const ResourceData& resource_data)
		: _resource_id(resource_data.resource_id),
		  _store_id(resource_data.store_id),
		  _size(resource_data.size)
	{}
};

};
