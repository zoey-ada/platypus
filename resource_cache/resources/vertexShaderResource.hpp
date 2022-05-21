#pragma once

#include <renderer/graphics.hpp>

#include "resource.hpp"

struct PtVertexShaderData
{
	PtVertexShader vertex_shader;
	PtInputLayout input_layout;
};

class VertexShaderResource: public Resource
{
public:
	explicit VertexShaderResource(PtResourceData* resource_data, PtVertexShaderData* shader_data);
	virtual ~VertexShaderResource() = default;

	[[nodiscard]] ResourceType type() const override { return ResourceType::VertexShader; }

	[[nodiscard]] PtVertexShader getShader() const { return this->_shader; }
	[[nodiscard]] PtInputLayout getInputLayout() const { return this->_input_layout; }

private:
	PtVertexShader _shader {nullptr};
	PtInputLayout _input_layout {nullptr};
};
