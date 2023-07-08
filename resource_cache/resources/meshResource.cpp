#include "meshResource.hpp"

#include <renderer/iRenderer.hpp>
#include <serviceProvider.hpp>

namespace platypus
{

MeshResource::MeshResource(PtMeshResourceData* resource_data)
	: Resource(resource_data),
	  _primative(resource_data->primative),
	  _vertex_buffer(resource_data->vertex_buffer),
	  _index_buffer(resource_data->index_buffer),
	  _index_count(resource_data->index_count),
	  _vertices(graphics::drawable(resource_data->vertices.data(), resource_data->index_count))
{}

MeshResource::~MeshResource()
{
	if (this->_vertex_buffer != nullptr)
	{
		ServiceProvider::getRenderer()->destroyVertexBuffer(this->_vertex_buffer);
		this->_vertex_buffer = nullptr;
	}

	if (this->_index_buffer != nullptr)
	{
		ServiceProvider::getRenderer()->destroyIndexBuffer(this->_index_buffer);
		this->_index_buffer = nullptr;
	}
}

};
