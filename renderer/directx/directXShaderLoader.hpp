#pragma once

#include <string>

#include <d3d11.h>

enum class ShaderType
{
	Pixel,
	Geometry,
	Vertex,
	Compute,
	Domain,
	Hull
};

bool loadShaderBytecode(const std::string& filename, uint8_t* buffer, const uint64_t size,
	ID3DBlob** bytecode, const ShaderType type);
