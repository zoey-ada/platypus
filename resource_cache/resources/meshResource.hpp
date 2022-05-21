#pragma once

#include <vector>

#include <renderer/graphics.hpp>
#include <utilities/math/mathTypes.hpp>

#include "resource.hpp"

struct PtMeshResourceData
{
	PtPrimitiveType primative;
	PtVertexBuffer vertex_buffer;
	PtIndexBuffer index_buffer;
	uint64_t index_count;
};

class MeshResource: public Resource
{
public:
	explicit MeshResource(PtResourceData* resource_data, PtMeshResourceData* mesh_data);

	virtual ~MeshResource() = default;

	[[nodiscard]] ResourceType type() const override { return ResourceType::Mesh; }

	[[nodiscard]] PtPrimitiveType getPrimitiveType() const { return this->_primative; }
	[[nodiscard]] PtVertexBuffer getVertexBuffer() const { return this->_vertex_buffer; }
	[[nodiscard]] PtIndexBuffer getIndexBuffer() const { return this->_index_buffer; }
	[[nodiscard]] uint64_t getIndexCount() const { return this->_index_count; }

private:
	PtPrimitiveType _primative {PtPrimitiveType::Invalid};
	PtVertexBuffer _vertex_buffer {nullptr};
	PtIndexBuffer _index_buffer {nullptr};
	uint64_t _index_count {0};
};
