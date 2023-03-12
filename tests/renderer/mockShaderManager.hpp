#pragma once

#include <renderer/iShaderManager.hpp>

class MockShaderManager: public IShaderManager
{
public:
	virtual ~MockShaderManager() = default;

	void destroyComputeShader(PtComputeShader shader) const override {}

	PtComputeShader createComputeShader(std::byte* shader_data,
		const uint64_t data_size) const override
	{
		return this->_compute_shader;
	}

	PtComputeShader compileComputeShader(std::byte* shader_data, const uint64_t data_size,
		const char* filename, const char* entry_point = "main") const override
	{
		return this->_compiled_compute_shader;
	}

	void destroyDomainShader(PtDomainShader shader) const override {}

	PtDomainShader createDomainShader(std::byte* shader_data,
		const uint64_t data_size) const override
	{
		return this->_domain_shader;
	}

	PtDomainShader compileDomainShader(std::byte* shader_data, const uint64_t data_size,
		const char* filename, const char* entry_point = "main") const override
	{
		return this->_compiled_domain_shader;
	}

	void destroyGeometryShader(PtGeometryShader shader) const override {}

	PtGeometryShader createGeometryShader(std::byte* shader_data,
		const uint64_t data_size) const override
	{
		return this->_geometry_shader;
	}

	PtGeometryShader compileGeometryShader(std::byte* shader_data, const uint64_t data_size,
		const char* filename, const char* entry_point = "main") const override
	{
		return this->_compiled_geometry_shader;
	}

	void destroyHullShader(PtHullShader shader) const override {}

	PtHullShader createHullShader(std::byte* shader_data, const uint64_t data_size) const override
	{
		return this->_hull_shader;
	}

	PtHullShader compileHullShader(std::byte* shader_data, const uint64_t data_size,
		const char* filename, const char* entry_point = "main") const override
	{
		return this->_compiled_hull_shader;
	}

	void destroyPixelShader(PtPixelShader shader) const override {}

	PtPixelShader createPixelShader(std::byte* shader_data, const uint64_t data_size) const override
	{
		return this->_pixel_shader;
	}

	PtPixelShader compilePixelShader(std::byte* shader_data, const uint64_t data_size,
		const char* filename, const char* entry_point = "main") const override
	{
		return this->_compiled_pixel_shader;
	}

	void destroyVertexShader(PtVertexShader shader) const override {}

	PtVertexShader createVertexShader(std::byte* shader_data,
		const uint64_t data_size) const override
	{
		return this->_vertex_shader;
	}

	PtVertexShader compileVertexShader(std::byte* shader_data, const uint64_t data_size,
		const char* filename, const char* entry_point = "main") const override
	{
		return this->_compiled_vertex_shader;
	}

	PtComputeShader _compute_shader;
	PtComputeShader _compiled_compute_shader;
	PtDomainShader _domain_shader;
	PtDomainShader _compiled_domain_shader;
	PtGeometryShader _geometry_shader;
	PtGeometryShader _compiled_geometry_shader;
	PtHullShader _hull_shader;
	PtHullShader _compiled_hull_shader;
	PtPixelShader _pixel_shader;
	PtPixelShader _compiled_pixel_shader;
	PtVertexShader _vertex_shader;
	PtVertexShader _compiled_vertex_shader;
};
