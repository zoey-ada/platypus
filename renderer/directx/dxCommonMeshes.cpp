#include "dxCommonMeshes.hpp"

#include <resource_cache/resources/meshResource.hpp>

#include "../graphics.hpp"
#include "directXRenderer.hpp"

std::shared_ptr<MeshResource> createCommonMesh(const CommonMesh mesh_type,
	const std::shared_ptr<DirectXRenderer>& renderer)
{
	switch (mesh_type)
	{
	case CommonMesh::Rectangle:
		return createRectangle(renderer);
	case CommonMesh::Rectangle_uFlipped:
		return createRectangle_uFlipped(renderer);
	default:
		return nullptr;
	}
}

std::shared_ptr<MeshResource> createRectangle_uFlipped(
	const std::shared_ptr<DirectXRenderer>& renderer)
{
	std::vector<graphics::Vertex> vertices = {{Vec3(0, 1, 0), Vec3(1, 1, 0), Vec2(0, 0)},
		{Vec3(1, 1, 0), Vec3(1, 1, 0), Vec2(1, 0)}, {Vec3(0, 0, 0), Vec3(1, 1, 0), Vec2(0, 1)},
		{Vec3(1, 0, 0), Vec3(1, 1, 0), Vec2(1, 1)}};

	std::vector<uint32_t> indices {0, 1, 2, 3};

	auto vertex_buffer = renderer->createVertexBuffer(vertices.data(), vertices.size());
	auto index_buffer = renderer->createIndexBuffer(indices.data(), indices.size());

	PtMeshResourceData resource_data {};
	resource_data.resource_id = "rectangle_1x1_u_flipped";
	resource_data.store_id = "internal";
	resource_data.size = 0;
	resource_data.primative = PtPrimitiveType::TriangleStrip;
	resource_data.vertex_buffer = vertex_buffer;
	resource_data.index_buffer = index_buffer;
	resource_data.index_count = indices.size();

	return std::make_shared<MeshResource>(&resource_data);
}

std::shared_ptr<MeshResource> createRectangle(const std::shared_ptr<DirectXRenderer>& renderer)
{
	std::vector<graphics::Vertex> vertices = {{Vec3(0, 1, 0), Vec3(1, 1, 0), Vec2(1, 0)},
		{Vec3(1, 1, 0), Vec3(1, 1, 0), Vec2(0, 0)}, {Vec3(0, 0, 0), Vec3(1, 1, 0), Vec2(1, 1)},
		{Vec3(1, 0, 0), Vec3(1, 1, 0), Vec2(0, 1)}};

	std::vector<uint32_t> indices {0, 1, 2, 3};

	auto vertex_buffer = renderer->createVertexBuffer(vertices.data(), vertices.size());
	auto index_buffer = renderer->createIndexBuffer(indices.data(), indices.size());

	PtMeshResourceData resource_data {};
	resource_data.resource_id = "rectangle_1x1";
	resource_data.store_id = "internal";
	resource_data.size = 0;
	resource_data.primative = PtPrimitiveType::TriangleStrip;
	resource_data.vertex_buffer = vertex_buffer;
	resource_data.index_buffer = index_buffer;
	resource_data.index_count = indices.size();

	return std::make_shared<MeshResource>(&resource_data);
}