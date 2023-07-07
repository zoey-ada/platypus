#include "directXMesh.hpp"

#include <resource_cache/iResourceCache.hpp>
#include <resource_cache/resources/meshResource.hpp>
#include <serviceProvider.hpp>

#include "directXRenderer.hpp"

DirectXMesh::DirectXMesh(std::shared_ptr<IResourceCache> cache, const std::string& resource_id)
	: _mesh_resource_id(resource_id)
{
	if (cache == nullptr)
	{
		// error
		return;
	}

	this->_mesh = cache->getMesh(resource_id);
}

void DirectXMesh::render(const std::shared_ptr<IRenderer>& renderer)
{
	auto dx_renderer = std::dynamic_pointer_cast<DirectXRenderer>(renderer);
	if (dx_renderer == nullptr)
	{
		// error
		return;
	}

	auto mesh_res = this->_mesh.lock();
	if (mesh_res == nullptr)
	{
		// error
		return;
	}

	// IA setup
	unsigned int stride = sizeof(graphics::DrawableVertex);
	unsigned int offset = 0;

	auto vertex_buffer = reinterpret_cast<ID3D11Buffer*>(mesh_res->getVertexBuffer());
	auto index_buffer = reinterpret_cast<ID3D11Buffer*>(mesh_res->getIndexBuffer());

	dx_renderer->context()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	dx_renderer->context()->IASetIndexBuffer(index_buffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	// render
	switch (mesh_res->getPrimitiveType())
	{
	case PtPrimitiveType::TriangleList:
	{
		dx_renderer->context()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	}
	case PtPrimitiveType::TriangleStrip:
	{
		dx_renderer->context()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;
	}
	case PtPrimitiveType::Invalid:
		// error log
		return;
	}

	dx_renderer->context()->DrawIndexed((UINT)mesh_res->getIndexCount(), 0, 0);
}

uint64_t DirectXMesh::getVertexCount() const
{
	auto mesh_res = this->_mesh.lock();
	if (mesh_res == nullptr)
	{
		// error
		return 0;
	}

	return mesh_res->getIndexCount();
}

void DirectXMesh::updateVertices(const graphics::Vertex* vertices, uint64_t count)
{
	auto renderer = ServiceProvider::getRenderer();
	auto dx_renderer = std::dynamic_pointer_cast<DirectXRenderer>(renderer);
	if (dx_renderer == nullptr)
	{
		// error
		return;
	}

	auto mesh_res = this->_mesh.lock();
	if (mesh_res == nullptr)
	{
		// error
		return;
	}

	auto* vertex_buffer = reinterpret_cast<ID3D11Buffer*>(mesh_res->getVertexBuffer());

	UINT subresource = 0;
	UINT map_flags = 0;
	D3D11_MAP map_type = D3D11_MAP::D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE resource;
	dx_renderer->context()->Map(vertex_buffer, subresource, map_type, map_flags, &resource);

	auto* existing_verticies = reinterpret_cast<graphics::DrawableVertex*>(resource.pData);

	for (uint64_t i = 0; i < count; ++i)
		existing_verticies[i] = drawable(vertices[i]);

	dx_renderer->context()->Unmap(vertex_buffer, subresource);
}

void DirectXMesh::updateVertexTextures(const Vec2* texture_coordinates, uint64_t count)
{
	auto renderer = ServiceProvider::getRenderer();
	auto dx_renderer = std::dynamic_pointer_cast<DirectXRenderer>(renderer);
	if (dx_renderer == nullptr)
	{
		// error
		return;
	}

	auto mesh_res = this->_mesh.lock();
	if (mesh_res == nullptr)
	{
		// error
		return;
	}

	auto vertex_buffer = reinterpret_cast<ID3D11Buffer*>(mesh_res->getVertexBuffer());

	UINT subresource = 0;
	UINT map_flags = 0;
	D3D11_MAP map_type = D3D11_MAP::D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE resource;
	dx_renderer->context()->Map(vertex_buffer, subresource, map_type, map_flags, &resource);

	memcpy(resource.pData, mesh_res->getVertices().data(),
		sizeof(graphics::DrawableVertex) * mesh_res->getIndexCount());

	auto* vertices = reinterpret_cast<graphics::DrawableVertex*>(resource.pData);

	for (uint64_t i = 0; i < count; ++i)
	{
		vertices[i].text_coord_x = texture_coordinates[i].x;
		vertices[i].text_coord_y = texture_coordinates[i].y;
	}

	dx_renderer->context()->Unmap(vertex_buffer, subresource);
}
