#pragma once

#include <renderer/graphics.hpp>

#include "resource.hpp"

struct PtPixelShaderData
{
	PtPixelShader pixel_shader;
};

class PixelShaderResource: public Resource
{
public:
	explicit PixelShaderResource(PtResourceData* resource_data, PtPixelShaderData* shader_data);
	virtual ~PixelShaderResource() = default;

	ResourceType type() const override { return ResourceType::PixelShader; }

	PtPixelShader getShader() const { return this->_shader; }

private:
	PtPixelShader _shader;
};
