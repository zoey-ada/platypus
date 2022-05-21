#include "MeshResource.hpp"

MeshResource::MeshResource(PtResourceData* resource_data, PtMeshResourceData* mesh_data)
	: Resource(resource_data),
	  _primative(mesh_data->primative),
	  _vertex_buffer(mesh_data->vertex_buffer),
	  _index_buffer(mesh_data->index_buffer),
	  _index_count(mesh_data->index_count)
{}
