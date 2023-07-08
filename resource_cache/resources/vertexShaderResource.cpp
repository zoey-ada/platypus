#include "vertexShaderResource.hpp"

#include <renderer/iRenderer.hpp>
#include <renderer/iShaderManager.hpp>
#include <serviceProvider.hpp>

#include "../stores/iResourceStore.hpp"

namespace platypus
{

VertexShaderResource::VertexShaderResource(PtVertexShaderData* resource_data)
	: Resource(resource_data),
	  _shader(resource_data->vertex_shader),
	  _input_layout(resource_data->input_layout)
{}

VertexShaderResource::~VertexShaderResource()
{
	if (this->_shader != nullptr)
	{
		ServiceProvider::getRenderer()->shaderManager()->destroyVertexShader(this->_shader);
		this->_shader = nullptr;
	}

	if (this->_input_layout != nullptr)
	{
		ServiceProvider::getRenderer()->destroyInputLayout(this->_input_layout);
		this->_input_layout = nullptr;
	}
};

};
