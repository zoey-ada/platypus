#pragma once

#include <renderer/graphics.hpp>

#include "resource.hpp"
#include "stringResourceData.hpp"

namespace platypus
{

class ProtobufResource: public Resource
{
public:
	explicit ProtobufResource(StringData* resource_data)
		: Resource(resource_data), _protobuf_json(std::move(resource_data->string_data))
	{}

	virtual ~ProtobufResource() = default;

	[[nodiscard]] ResourceType type() const override { return ResourceType::Protobuf; }

	[[nodiscard]] const char* getProtobufJson() const { return this->_protobuf_json.c_str(); }

private:
	std::string _protobuf_json;
};

};
