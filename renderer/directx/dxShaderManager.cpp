#include "dxShaderManager.hpp"

#include <d3d11.h>

#include <utilities/common/safeRelease.hpp>
#include <utilities/logging/logger.hpp>

#include "directXShaderLoader.hpp"

bool DxShaderManager::initialize(ID3D11Device* device)
{
	if (device == nullptr)
	{
		// error
		return false;
	}

	this->_device = device;
	return true;
}

ID3D11ComputeShader* DxShaderManager::createComputeShader(ID3D10Blob* shader_bytecode) const
{
	auto* device = this->_device;
	if (device == nullptr)
	{
		logWarning("Unable to aquire D3D device to create compute shader.", "render");
		return nullptr;
	}

	ID3D11ComputeShader* shader = nullptr;
	auto hr = device->CreateComputeShader(shader_bytecode->GetBufferPointer(),
		shader_bytecode->GetBufferSize(), nullptr, &shader);

	if (FAILED(hr))
	{
		logWarning("Unable to create compute shader", hr, "render");
	}

	return shader;
}

void DxShaderManager::destroyComputeShader(PtComputeShader shader) const
{
	ID3D11ComputeShader* compute_shader = reinterpret_cast<ID3D11ComputeShader*>(shader);
	safeRelease(&compute_shader);
}

PtComputeShader DxShaderManager::createComputeShader(std::byte* shader_data,
	const uint64_t data_size) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!loadShaderFromBuffer(shader_data, data_size, &bytecode))
	{
		logWarning("Unable to load compute shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11ComputeShader* shader = this->createComputeShader(bytecode);
	safeRelease(&bytecode);
	return (PtComputeShader)shader;
}

PtComputeShader DxShaderManager::compileComputeShader(std::byte* shader_data,
	const uint64_t data_size, const char* filename, const char* entry_point) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!compileShaderFromBuffer(filename, shader_data, data_size, nullptr, entry_point,
			ShaderType::Compute, &bytecode))
	{
		logWarning("Unable to compile and load compute shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11ComputeShader* shader = this->createComputeShader(bytecode);
	safeRelease(&bytecode);
	return (PtComputeShader)shader;
}

ID3D11DomainShader* DxShaderManager::createDomainShader(ID3D10Blob* shader_bytecode) const
{
	auto* device = this->_device;
	if (device == nullptr)
	{
		logWarning("Unable to aquire D3D device to create domain shader.", "render");
		return nullptr;
	}

	ID3D11DomainShader* shader = nullptr;
	auto hr = device->CreateDomainShader(shader_bytecode->GetBufferPointer(),
		shader_bytecode->GetBufferSize(), nullptr, &shader);

	if (FAILED(hr))
	{
		logWarning("Unable to create domain shader", hr, "render");
	}

	return shader;
}

void DxShaderManager::destroyDomainShader(PtDomainShader shader) const
{
	ID3D11DomainShader* domain_shader = reinterpret_cast<ID3D11DomainShader*>(shader);
	safeRelease(&domain_shader);
}

PtDomainShader DxShaderManager::createDomainShader(std::byte* shader_data,
	const uint64_t data_size) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!loadShaderFromBuffer(shader_data, data_size, &bytecode))
	{
		logWarning("Unable to load domain shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11DomainShader* shader = this->createDomainShader(bytecode);
	safeRelease(&bytecode);
	return (PtDomainShader)shader;
}

PtDomainShader DxShaderManager::compileDomainShader(std::byte* shader_data,
	const uint64_t data_size, const char* filename, const char* entry_point) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!compileShaderFromBuffer(filename, shader_data, data_size, nullptr, entry_point,
			ShaderType::Domain, &bytecode))
	{
		logWarning("Unable to compile and load domain shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11DomainShader* shader = this->createDomainShader(bytecode);
	safeRelease(&bytecode);
	return (PtDomainShader)shader;
}

ID3D11GeometryShader* DxShaderManager::createGeometryShader(ID3D10Blob* shader_bytecode) const
{
	auto* device = this->_device;
	if (device == nullptr)
	{
		logWarning("Unable to aquire D3D device to create geometry shader.", "render");
		return nullptr;
	}

	ID3D11GeometryShader* shader = nullptr;
	auto hr = device->CreateGeometryShader(shader_bytecode->GetBufferPointer(),
		shader_bytecode->GetBufferSize(), nullptr, &shader);

	if (FAILED(hr))
	{
		logWarning("Unable to create geometry shader", hr, "render");
	}

	return shader;
}

void DxShaderManager::destroyGeometryShader(PtGeometryShader shader) const
{
	ID3D11GeometryShader* domain_shader = reinterpret_cast<ID3D11GeometryShader*>(shader);
	safeRelease(&domain_shader);
}

PtGeometryShader DxShaderManager::createGeometryShader(std::byte* shader_data,
	const uint64_t data_size) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!loadShaderFromBuffer(shader_data, data_size, &bytecode))
	{
		logWarning("Unable to load geometry shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11GeometryShader* shader = this->createGeometryShader(bytecode);
	safeRelease(&bytecode);
	return (PtGeometryShader)shader;
}

PtGeometryShader DxShaderManager::compileGeometryShader(std::byte* shader_data,
	const uint64_t data_size, const char* filename, const char* entry_point) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!compileShaderFromBuffer(filename, shader_data, data_size, nullptr, entry_point,
			ShaderType::Geometry, &bytecode))
	{
		logWarning("Unable to compile and load geometry shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11GeometryShader* shader = this->createGeometryShader(bytecode);
	safeRelease(&bytecode);
	return (PtGeometryShader)shader;
}

ID3D11HullShader* DxShaderManager::createHullShader(ID3D10Blob* shader_bytecode) const
{
	auto* device = this->_device;
	if (device == nullptr)
	{
		logWarning("Unable to aquire D3D device to create hull shader.", "render");
		return nullptr;
	}

	ID3D11HullShader* shader = nullptr;
	auto hr = device->CreateHullShader(shader_bytecode->GetBufferPointer(),
		shader_bytecode->GetBufferSize(), nullptr, &shader);

	if (FAILED(hr))
	{
		logWarning("Unable to create hull shader", hr, "render");
	}

	return shader;
}

void DxShaderManager::destroyHullShader(PtHullShader shader) const
{
	ID3D11HullShader* domain_shader = reinterpret_cast<ID3D11HullShader*>(shader);
	safeRelease(&domain_shader);
}

PtHullShader DxShaderManager::createHullShader(std::byte* shader_data,
	const uint64_t data_size) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!loadShaderFromBuffer(shader_data, data_size, &bytecode))
	{
		logWarning("Unable to load hull shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11HullShader* shader = this->createHullShader(bytecode);
	safeRelease(&bytecode);
	return (PtHullShader)shader;
}

PtHullShader DxShaderManager::compileHullShader(std::byte* shader_data, const uint64_t data_size,
	const char* filename, const char* entry_point) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!compileShaderFromBuffer(filename, shader_data, data_size, nullptr, entry_point,
			ShaderType::Hull, &bytecode))
	{
		logWarning("Unable to compile and load hull shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11HullShader* shader = this->createHullShader(bytecode);
	safeRelease(&bytecode);
	return (PtHullShader)shader;
}

ID3D11PixelShader* DxShaderManager::createPixelShader(ID3D10Blob* shader_bytecode) const
{
	auto* device = this->_device;
	if (device == nullptr)
	{
		logWarning("Unable to aquire D3D device to create pixel shader.", "render");
		return nullptr;
	}

	ID3D11PixelShader* shader = nullptr;
	auto hr = device->CreatePixelShader(shader_bytecode->GetBufferPointer(),
		shader_bytecode->GetBufferSize(), nullptr, &shader);

	if (FAILED(hr))
	{
		logWarning("Unable to create pixel shader", hr, "render");
	}

	return shader;
}

void DxShaderManager::destroyPixelShader(PtPixelShader shader) const
{
	ID3D11PixelShader* domain_shader = reinterpret_cast<ID3D11PixelShader*>(shader);
	safeRelease(&domain_shader);
}

PtPixelShader DxShaderManager::createPixelShader(std::byte* shader_data,
	const uint64_t data_size) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!loadShaderFromBuffer(shader_data, data_size, &bytecode))
	{
		logWarning("Unable to load pixel shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11PixelShader* shader = this->createPixelShader(bytecode);
	safeRelease(&bytecode);
	return (PtPixelShader)shader;
}

PtPixelShader DxShaderManager::compilePixelShader(std::byte* shader_data, const uint64_t data_size,
	const char* filename, const char* entry_point) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!compileShaderFromBuffer(filename, shader_data, data_size, nullptr, entry_point,
			ShaderType::Pixel, &bytecode))
	{
		logWarning("Unable to compile and load pixel shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11PixelShader* shader = this->createPixelShader(bytecode);
	safeRelease(&bytecode);
	return (PtPixelShader)shader;
}

ID3D11VertexShader* DxShaderManager::createVertexShader(ID3D10Blob* shader_bytecode) const
{
	auto device = this->_device;
	if (device == nullptr)
	{
		logWarning("Unable to aquire D3D device to create vertex shader.", "render");
		return nullptr;
	}

	ID3D11VertexShader* shader = nullptr;
	auto hr = device->CreateVertexShader(shader_bytecode->GetBufferPointer(),
		shader_bytecode->GetBufferSize(), nullptr, &shader);

	if (FAILED(hr))
	{
		logWarning("Unable to create vertex shader", hr, "render");
	}

	return shader;
}

void DxShaderManager::destroyVertexShader(PtVertexShader shader) const
{
	ID3D11VertexShader* domain_shader = reinterpret_cast<ID3D11VertexShader*>(shader);
	safeRelease(&domain_shader);
}

PtVertexShader DxShaderManager::createVertexShader(std::byte* shader_data,
	const uint64_t data_size) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!loadShaderFromBuffer(shader_data, data_size, &bytecode))
	{
		logWarning("Unable to load vertex shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11VertexShader* shader = this->createVertexShader(bytecode);
	safeRelease(&bytecode);
	return (PtVertexShader)shader;
}

PtVertexShader DxShaderManager::compileVertexShader(std::byte* shader_data,
	const uint64_t data_size, const char* filename, const char* entry_point) const
{
	ID3D10Blob* bytecode = nullptr;
	if (!compileShaderFromBuffer(filename, shader_data, data_size, nullptr, entry_point,
			ShaderType::Vertex, &bytecode))
	{
		logWarning("Unable to compile and load vertex shader data to bytecode", "render");
		return nullptr;
	}

	ID3D11VertexShader* shader = this->createVertexShader(bytecode);
	safeRelease(&bytecode);
	return (PtVertexShader)shader;
}
