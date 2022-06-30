#include "directXShaderLoader.hpp"

#include <map>
#include <string>

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <utilities/encoding.hpp>
#include <utilities/logging/logger.hpp>
#include <utilities/wildcardMatch.hpp>

const std::map<ShaderType, std::string> compilers {
	{ShaderType::Compute, "cs_5_0"},
	{ShaderType::Domain, "ds_5_0"},
	{ShaderType::Geometry, "gs_5_0"},
	{ShaderType::Hull, "hs_5_0"},
	{ShaderType::Pixel, "ps_5_0"},
	{ShaderType::Vertex, "vs_5_0"},
};

bool compileShaderFromFile(const std::string& filename, const D3D_SHADER_MACRO* defines,
	LPCSTR entry_point, std::string shader_compiler, ID3DBlob** bytecode)
{
	UINT compiler_flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

#ifdef _DEBUG
	compiler_flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* error_blob = nullptr;
	auto utf16_filename = Encoding::toWindowsString(filename);

	HRESULT hr =
		D3DCompileFromFile(utf16_filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entry_point, shader_compiler.c_str(), compiler_flags, 0, bytecode, &error_blob);

	if (FAILED(hr))
	{
		if (error_blob != nullptr && error_blob->GetBufferSize() > 0)
		{
			// log error message
		}
	}

	if (error_blob != nullptr)
		error_blob->Release();

	return SUCCEEDED(hr) == TRUE;
}

bool compileShaderFromBuffer(const std::string& filename, uint8_t* buffer, const uint64_t size,
	const D3D_SHADER_MACRO* defines, LPCSTR entry_point, const ShaderType shader_compiler,
	ID3DBlob** bytecode)
{
	return compileShaderFromBuffer(filename, (std::byte*)buffer, size, defines, entry_point,
		shader_compiler, bytecode);
}

bool compileShaderFromBuffer(const std::string& filename, std::byte* buffer, const uint64_t size,
	const D3D_SHADER_MACRO* defines, LPCSTR entry_point, const ShaderType shader_compiler,
	ID3DBlob** bytecode)
{
	UINT compiler_flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

#ifdef _DEBUG
	compiler_flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* error_blob = nullptr;

	HRESULT hr = D3DCompile(buffer, size, filename.c_str(), defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point, compilers.at(shader_compiler).c_str(),
		compiler_flags, 0, bytecode, &error_blob);

	if (FAILED(hr))
	{
		if (error_blob != nullptr && error_blob->GetBufferSize() > 0)
		{
			logWarning("failed to compile shader " +
				std::string((char*)error_blob->GetBufferPointer(), error_blob->GetBufferSize()));
			error_blob->Release();
			// log error message
		}
	}

	if (error_blob != nullptr)
		error_blob->Release();

	return SUCCEEDED(hr) == TRUE;
}

bool loadShaderFromFile(const std::string& filename, ID3DBlob** bytecode)
{
	auto utf16_filename = Encoding::toWindowsString(filename);

	if (FAILED(D3DReadFileToBlob(utf16_filename.c_str(), bytecode)))
		bytecode = nullptr;

	return bytecode != nullptr;
}

bool loadShaderFromBuffer(uint8_t* buffer, const uint64_t size, ID3DBlob** bytecode)
{
	return loadShaderFromBuffer((std::byte*)buffer, size, bytecode);
}

bool loadShaderFromBuffer(std::byte* buffer, const uint64_t size, ID3DBlob** bytecode)
{
	if (FAILED(D3DCreateBlob(size, bytecode)))
	{
		bytecode = nullptr;
	}
	else
	{
		auto* addr = (*bytecode)->GetBufferPointer();
		memcpy(addr, buffer, size);
	}

	return bytecode != nullptr;
}

bool loadShaderBytecode(const std::string& filename, uint8_t* buffer, const uint64_t size,
	ID3DBlob** bytecode, const ShaderType type)
{
	if (wildcardMatch("*.cso", filename.c_str()))
	{
		if (buffer == nullptr)
		{
			if (!loadShaderFromFile(filename, bytecode))
			{
				// log error
				return false;
			}
		}
		else
		{
			if (!loadShaderFromBuffer(buffer, size, bytecode))
			{
				// log error
				return false;
			}
		}
	}
	else
	{
		if (buffer == nullptr)
		{
			auto compiler = compilers.at(type);
			if (!compileShaderFromFile(filename, nullptr, "main", compiler, bytecode))
			{
				// log error
				return false;
			}
		}
		else
		{
			if (!compileShaderFromBuffer(filename, buffer, size, nullptr, "main", type, bytecode))
			{
				// log error
				return false;
			}
		}
	}

	return true;
}
