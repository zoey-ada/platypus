#pragma once

#include <cstddef>
#include <cstdint>

#include "graphics.hpp"

class IShaderManager
{
public:
	virtual void destroyComputeShader(PtComputeShader shader) const = 0;
	[[nodiscard]] virtual PtComputeShader createComputeShader(std::byte* shader_data,
		const uint64_t data_size) const = 0;
	[[nodiscard]] virtual PtComputeShader compileComputeShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename, const char* entry_point = "main") const = 0;

	virtual void destroyDomainShader(PtDomainShader shader) const = 0;
	[[nodiscard]] virtual PtDomainShader createDomainShader(std::byte* shader_data,
		const uint64_t data_size) const = 0;
	[[nodiscard]] virtual PtDomainShader compileDomainShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename, const char* entry_point = "main") const = 0;

	virtual void destroyGeometryShader(PtGeometryShader shader) const = 0;
	[[nodiscard]] virtual PtGeometryShader createGeometryShader(std::byte* shader_data,
		const uint64_t data_size) const = 0;
	[[nodiscard]] virtual PtGeometryShader compileGeometryShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename, const char* entry_point = "main") const = 0;

	virtual void destroyHullShader(PtHullShader shader) const = 0;
	[[nodiscard]] virtual PtHullShader createHullShader(std::byte* shader_data,
		const uint64_t data_size) const = 0;
	[[nodiscard]] virtual PtHullShader compileHullShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename, const char* entry_point = "main") const = 0;

	virtual void destroyPixelShader(PtPixelShader shader) const = 0;
	[[nodiscard]] virtual PtPixelShader createPixelShader(std::byte* shader_data,
		const uint64_t data_size) const = 0;
	[[nodiscard]] virtual PtPixelShader compilePixelShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename, const char* entry_point = "main") const = 0;

	virtual void destroyVertexShader(PtVertexShader shader) const = 0;
	[[nodiscard]] virtual PtVertexShader createVertexShader(std::byte* shader_data,
		const uint64_t data_size) const = 0;
	[[nodiscard]] virtual PtVertexShader compileVertexShader(std::byte* shader_data,
		const uint64_t data_size, const char* filename, const char* entry_point = "main") const = 0;
};
