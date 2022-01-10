#pragma once

#include <string>

enum class ResourceType
{
	PixelShader,
	VertexShader,
	General
};

namespace std
{
	std::string to_string(const ResourceType type);
};
