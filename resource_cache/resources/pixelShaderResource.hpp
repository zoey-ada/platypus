#pragma once

#include <renderer/graphics.hpp>

#include "resource.hpp"

namespace platypus
{

struct PixelShaderData: public ResourceData
{
	PtPixelShader pixel_shader;
};

class PixelShaderResource: public Resource
{
public:
	explicit PixelShaderResource(PixelShaderData* resource_data);
	virtual ~PixelShaderResource();

	[[nodiscard]] ResourceType type() const override { return ResourceType::PixelShader; }

	[[nodiscard]] PtPixelShader getShader() const { return this->_shader; }

private:
	PtPixelShader _shader {nullptr};
};

};
