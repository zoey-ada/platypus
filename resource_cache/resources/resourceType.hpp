#pragma once

#include <string>

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

namespace std
{

std::string to_string(const ResourceType type);

};
