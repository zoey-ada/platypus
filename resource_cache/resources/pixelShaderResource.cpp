#include "pixelShaderResource.hpp"

#include <renderer/iRenderer.hpp>
#include <renderer/iShaderManager.hpp>
#include <serviceProvider.hpp>

namespace platypus
{

PixelShaderResource::PixelShaderResource(PtPixelShaderData* resource_data)
	: Resource(resource_data), _shader(resource_data->pixel_shader)
{}

PixelShaderResource::~PixelShaderResource()
{
	if (this->_shader)
	{
		ServiceProvider::getRenderer()->shaderManager()->destroyPixelShader(this->_shader);
		this->_shader = nullptr;
	}
};

};
