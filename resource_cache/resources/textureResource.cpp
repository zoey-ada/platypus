#include "textureResource.hpp"

TextureResource::TextureResource(PtResourceData* resource_data, PtTextureData* texture_data)
	: Resource(resource_data),
	  _sampler_state(texture_data->sampler_state),
	  _texture(texture_data->texture)
{}

Vec2 TextureResource::getDimensions() const
{
	return Vec2();
}
