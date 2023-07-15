#pragma once

#include <optional>
#include <vector>

#include <renderer/graphics.hpp>
#include <renderer/material.hpp>

#include "resource.hpp"

namespace platypus
{

struct MeshData: public ResourceData
{
	graphics::PrimitiveType primative;

	graphics::IndexBuffer index_buffer;
	uint64_t index_count;

	std::vector<graphics::Vertex> vertices;
	graphics::VertexBuffer vertex_buffer;

	std::optional<Material> material;
	graphics::ConstantBuffer material_buffer;

	std::string texture_resource_id;
};

class MeshResource: public Resource
{
public:
	explicit MeshResource(MeshData* resource_data);
	virtual ~MeshResource();

	inline ResourceType type() const override { return ResourceType::Mesh; }

	inline graphics::PrimitiveType getPrimitiveType() const { return this->_primative; }

	inline graphics::IndexBuffer getIndexBuffer() const { return this->_index_buffer; }

	inline uint64_t getIndexCount() const { return this->_index_count; }
	inline void setIndexCount(const uint64_t& count) { this->_index_count = count; }

	inline std::vector<graphics::DrawableVertex> getVertices() const { return this->_vertices; }
	inline std::vector<graphics::DrawableVertex>* getEditableVertices() { return &this->_vertices; }
	inline void setVertices(const std::vector<graphics::DrawableVertex>& vertices)
	{
		this->_vertices = vertices;
	}

	inline graphics::VertexBuffer getVertexBuffer() const { return this->_vertex_buffer; }

	inline uint64_t getVertexCount() const { return this->_vertices.size(); }

	inline std::optional<Material> getMaterial() const { return this->_material; }
	inline std::optional<Material>& getEditableMaterial() { return this->_material; }
	inline void setMaterial(const Material& material) { this->_material = material; }
	inline void clearMaterial() { this->_material.reset(); }

	inline graphics::ConstantBuffer getMaterialBuffer() const { return this->_material_buffer; }

	inline std::string getTextureResourceId() const { return this->_texture_res_id; }
	inline void setTextureResourceId(const std::string& resource_id)
	{
		this->_texture_res_id = resource_id;
	}

private:
	graphics::PrimitiveType _primative {graphics::PrimitiveType::Invalid};
	graphics::IndexBuffer _index_buffer {nullptr};
	uint64_t _index_count {0};

	std::vector<graphics::DrawableVertex> _vertices;
	graphics::VertexBuffer _vertex_buffer {nullptr};

	std::optional<Material> _material;
	graphics::ConstantBuffer _material_buffer {nullptr};

	std::string _texture_res_id;
};
};
