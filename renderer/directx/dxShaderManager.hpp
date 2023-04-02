#pragma once

#include "../iShaderManager.hpp"

struct ID3D10Blob;
struct ID3D11ComputeShader;
struct ID3D11Device;
struct ID3D11DomainShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

class DxShaderManager: public IShaderManager
{
public:
	virtual ~DxShaderManager() = default;

	[[nodiscard]] bool initialize(ID3D11Device* device);
	void deinitialize() { this->_device = nullptr; }

	void destroyComputeShader(PtComputeShader shader) const override;
	[[nodiscard]] PtComputeShader createComputeShader(std::byte* shader_data,
		const uint64_t data_size) const override;
	[[nodiscard]] PtComputeShader compileComputeShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename,
		const char* entry_point = "main") const override;

	void destroyDomainShader(PtDomainShader shader) const override;
	[[nodiscard]] PtDomainShader createDomainShader(std::byte* shader_data,
		const uint64_t data_size) const override;
	[[nodiscard]] PtDomainShader compileDomainShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename,
		const char* entry_point = "main") const override;

	void destroyGeometryShader(PtGeometryShader shader) const override;
	[[nodiscard]] PtGeometryShader createGeometryShader(std::byte* shader_data,
		const uint64_t data_size) const override;
	[[nodiscard]] PtGeometryShader compileGeometryShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename,
		const char* entry_point = "main") const override;

	void destroyHullShader(PtHullShader shader) const override;
	[[nodiscard]] PtHullShader createHullShader(std::byte* shader_data,
		const uint64_t data_size) const override;
	[[nodiscard]] PtHullShader compileHullShader(std::byte* shader_data, const uint64_t data_size,
		const char* filename, const char* entry_point = "main") const override;

	void destroyPixelShader(PtPixelShader shader) const override;
	[[nodiscard]] PtPixelShader createPixelShader(std::byte* shader_data,
		const uint64_t data_size) const override;
	[[nodiscard]] PtPixelShader compilePixelShader(std::byte* shader_data, const uint64_t data_size,
		const char* filename, const char* entry_point = "main") const override;

	void destroyVertexShader(PtVertexShader shader) const override;
	[[nodiscard]] PtVertexShader createVertexShader(std::byte* shader_data,
		const uint64_t data_size) const override;
	[[nodiscard]] PtVertexShader compileVertexShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename,
		const char* entry_point = "main") const override;

private:
	ID3D11Device* _device {nullptr};

	[[nodiscard]] ID3D11ComputeShader* createComputeShader(ID3D10Blob* shader_bytecode) const;
	[[nodiscard]] ID3D11DomainShader* createDomainShader(ID3D10Blob* shader_bytecode) const;
	[[nodiscard]] ID3D11GeometryShader* createGeometryShader(ID3D10Blob* shader_bytecode) const;
	[[nodiscard]] ID3D11HullShader* createHullShader(ID3D10Blob* shader_bytecode) const;
	[[nodiscard]] ID3D11PixelShader* createPixelShader(ID3D10Blob* shader_bytecode) const;
	[[nodiscard]] ID3D11VertexShader* createVertexShader(ID3D10Blob* shader_bytecode) const;
};
