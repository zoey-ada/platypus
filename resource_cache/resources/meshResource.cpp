#include "MeshResource.hpp"

DrawableVertex drawable(const Vertex& vertex)
{
	return DrawableVertex {vertex.coord.x, vertex.coord.y, vertex.coord.z, vertex.normal.x,
		vertex.normal.y, vertex.normal.z, vertex.textCoord.x, vertex.textCoord.y};
}

std::vector<DrawableVertex> drawable(const std::vector<Vertex>& vertices)
{
	std::vector<DrawableVertex> drawable_verts(vertices.size());

	uint32_t index = 0;
	for (const auto& vert : vertices)
	{
		drawable_verts.at(index) = drawable(vert);
		++index;
	}

	return drawable_verts;
}

MeshResource::MeshResource(std::string name, uint8_t* buffer, uint64_t size,
	std::shared_ptr<IResourceStore> store, std::shared_ptr<ResourceCache> cache,
	std::shared_ptr<graphics::VertexBuffer> vertex_buffer, std::vector<Vertex> vertices,
	uint64_t vertex_count, std::shared_ptr<graphics::IndexBuffer> index_buffer,
	std::vector<uint32_t> indices, uint64_t index_count)
	: Resource(name, buffer, size, store, cache),
	  _vertex_buffer(vertex_buffer),
	  _vertices(vertices),
	  _vertex_count(vertex_count),
	  _index_buffer(index_buffer),
	  _indices(indices),
	  _index_count(index_count)
{}
