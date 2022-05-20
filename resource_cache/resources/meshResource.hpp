#pragma once

#include <vector>

#include <d3d11.h>

#include <utilities/math/mathTypes.hpp>

#include "resource.hpp"

namespace graphics
{
using VertexBuffer = ID3D11Buffer;
using IndexBuffer = ID3D11Buffer;
};

struct Vertex
{
	Vec3 coord {};
	Vec3 normal {};
	Vec2 textCoord {};
};

struct DrawableVertex
{
	float coord_x;
	float coord_y;
	float coord_z;

	float norm_x;
	float norm_y;
	float norm_z;

	float text_coord_x;
	float text_coord_y;
};

DrawableVertex drawable(const Vertex& vertex);
std::vector<DrawableVertex> drawable(const std::vector<Vertex>& vertices);

class MeshResource: public Resource
{
public:
	explicit MeshResource(std::string name, uint8_t* buffer, uint64_t size,
		std::shared_ptr<IResourceStore> store, std::shared_ptr<ResourceCache> cache,
		std::shared_ptr<graphics::VertexBuffer> vertex_buffer, std::vector<Vertex> vertices,
		uint64_t vertex_count, std::shared_ptr<graphics::IndexBuffer> index_buffer,
		std::vector<uint32_t> indices, uint64_t index_count);

	virtual ~MeshResource() = default;

	[[nodiscard]] ResourceType type() const override { return ResourceType::Mesh; }

	[[nodiscard]] std::shared_ptr<graphics::VertexBuffer> getVertexBuffer() const
	{
		return this->_vertex_buffer;
	}

	[[nodiscard]] std::vector<DrawableVertex> getVertices() { return drawable(this->_vertices); }
	[[nodiscard]] uint64_t getVertexCount() const { return this->_vertex_count; }

	[[nodiscard]] std::shared_ptr<graphics::IndexBuffer> getIndexBuffer() const
	{
		return this->_index_buffer;
	}

	[[nodiscard]] std::vector<uint32_t> getIndices() { return this->_indices; }
	[[nodiscard]] uint64_t getIndexCount() const { return this->_index_count; }

private:
	std::shared_ptr<graphics::VertexBuffer> _vertex_buffer {};
	std::vector<Vertex> _vertices {};
	uint64_t _vertex_count {0};

	std::shared_ptr<graphics::IndexBuffer> _index_buffer {};
	std::vector<uint32_t> _indices {};
	uint64_t _index_count {0};
};
