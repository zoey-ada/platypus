#pragma once

#include <string>

enum class ResourceType
{
	General,
	Mesh,
	PixelShader,
	Texture,
	VertexShader,
	Unknown
};

namespace std
{

std::string to_string(const ResourceType type);

};
