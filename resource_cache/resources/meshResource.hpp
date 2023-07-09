#pragma once

#include <vector>

#include <renderer/graphics.hpp>

#include "resource.hpp"

namespace platypus
{

struct MeshData: public ResourceData
{
	PtPrimitiveType primative;
	PtVertexBuffer vertex_buffer;
	PtIndexBuffer index_buffer;
	uint64_t index_count;
	std::vector<graphics::Vertex> vertices;
};

class MeshResource: public Resource
{
public:
	explicit MeshResource(MeshData* resource_data);
	virtual ~MeshResource();

	[[nodiscard]] ResourceType type() const override { return ResourceType::Mesh; }

	[[nodiscard]] PtPrimitiveType getPrimitiveType() const { return this->_primative; }
	[[nodiscard]] PtVertexBuffer getVertexBuffer() const { return this->_vertex_buffer; }
	[[nodiscard]] PtIndexBuffer getIndexBuffer() const { return this->_index_buffer; }
	[[nodiscard]] uint64_t getIndexCount() const { return this->_index_count; }
	[[nodiscard]] std::vector<graphics::DrawableVertex> getVertices() const
	{
		return this->_vertices;
	}

private:
	PtPrimitiveType _primative {PtPrimitiveType::Invalid};
	PtVertexBuffer _vertex_buffer {nullptr};
	PtIndexBuffer _index_buffer {nullptr};
	uint64_t _index_count {0};
	std::vector<graphics::DrawableVertex> _vertices;
};

};
