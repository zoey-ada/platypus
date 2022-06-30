#include "textureResource.hpp"

#include <renderer/iRenderer.hpp>
#include <serviceProvider.hpp>

TextureResource::TextureResource(PtTextureData* resource_data)
	: Resource(resource_data),
	  _sampler_state(resource_data->sampler_state),
	  _texture(resource_data->texture),
	  _dimensions(resource_data->dimensions)
{}

TextureResource::~TextureResource()
{
	if (this->_sampler_state != nullptr)
	{
		ServiceProvider::getRenderer()->destroySamplerState(this->_sampler_state);
		this->_sampler_state = nullptr;
	}

	if (this->_texture != nullptr)
	{
		ServiceProvider::getRenderer()->destroyTexture(this->_texture);
		this->_texture = nullptr;
	}
}
