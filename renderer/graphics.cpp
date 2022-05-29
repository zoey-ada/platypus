#include "graphics.hpp"

namespace graphics
{

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

}