#include "vertexShaderResource.hpp"

#include "../stores/iResourceStore.hpp"

VertexShaderResource::VertexShaderResource(PtResourceData* resource_data,
	PtVertexShaderData* shader_data)
	: Resource(resource_data),
	  _shader(shader_data->vertex_shader),
	  _input_layout(shader_data->input_layout)
{}
