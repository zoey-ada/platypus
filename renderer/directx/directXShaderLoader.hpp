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

bool loadShaderFromBuffer(std::byte* buffer, const uint64_t size, ID3DBlob** bytecode);

bool compileShaderFromBuffer(const std::string& filename, std::byte* buffer, const uint64_t size,
	const D3D_SHADER_MACRO* defines, LPCSTR entry_point, const ShaderType shader_compiler,
	ID3DBlob** bytecode);
