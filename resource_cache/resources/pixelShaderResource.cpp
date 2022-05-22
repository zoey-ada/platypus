#include "pixelShaderResource.hpp"

#include "../stores/iResourceStore.hpp"

PixelShaderResource::PixelShaderResource(PtResourceData* resource_data,
	PtPixelShaderData* shader_data)
	: Resource(resource_data), _shader(shader_data->pixel_shader)
{}
