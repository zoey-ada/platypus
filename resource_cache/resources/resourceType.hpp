#pragma once

#include <string>

namespace platypus
{

enum class ResourceType
{
	General,
	Audio,
	Mesh,
	PixelShader,
	Protobuf,
	Texture,
	VertexShader,
	Unknown
};

};

namespace std
{

std::string to_string(const platypus::ResourceType type);

};
