#include "directXMesh.hpp"

#include <resource_cache/iResourceCache.hpp>
#include <resource_cache/resources/meshResource.hpp>
#include <serviceProvider.hpp>
#include <utilities/logging/iLoggingSystem.hpp>

#include "../scene.hpp"
#include "../scene_nodes/sceneNode.hpp"
#include "directXRenderer.hpp"

DirectXMesh::DirectXMesh(std::shared_ptr<platypus::IResourceCache> cache,
	const std::string& resource_id)
	: _mesh_resource_id(resource_id)
{
	if (cache == nullptr)
	{
		// error
		return;
	}

	auto mesh = cache->getMesh(resource_id);
	this->_mesh = mesh;

	if (!mesh->getTextureResourceId().empty())
	{
		auto texture = cache->getTexture(mesh->getTextureResourceId());
		this->_texture = texture;
	}
}

bool DirectXMesh::setupRender(const std::shared_ptr<Scene>& scene,
	const std::shared_ptr<SceneNode>& /*node*/)
{
	auto mesh_res = this->_mesh.lock();
	if (mesh_res == nullptr)
	{
		// error
		return false;
	}

	if (mesh_res->getMaterial().has_value())
	{
		auto dx_renderer = std::dynamic_pointer_cast<DirectXRenderer>(scene->renderer());
		auto material_buffer = reinterpret_cast<ID3D11Buffer*>(mesh_res->getMaterialBuffer());
		dx_renderer->context()->PSSetConstantBuffers(0, 1, &material_buffer);
	}

	return true;
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
	unsigned int stride = sizeof(platypus::graphics::DrawableVertex);
	unsigned int offset = 0;

	auto vertex_buffer = reinterpret_cast<ID3D11Buffer*>(mesh_res->getVertexBuffer());
	auto index_buffer = reinterpret_cast<ID3D11Buffer*>(mesh_res->getIndexBuffer());

	dx_renderer->context()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	dx_renderer->context()->IASetIndexBuffer(index_buffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	// render
	switch (mesh_res->getPrimitiveType())
	{
	case platypus::graphics::PrimitiveType::TriangleList:
	{
		dx_renderer->context()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	}
	case platypus::graphics::PrimitiveType::TriangleStrip:
	{
		dx_renderer->context()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;
	}
	case platypus::graphics::PrimitiveType::Invalid:
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

	return mesh_res->getVertexCount();
}

void DirectXMesh::updateVertices(const platypus::graphics::Vertex* vertices, uint64_t count)
{
	auto drawable_vertices = platypus::graphics::drawable(vertices, count);
	return this->updateVertices(drawable_vertices.data(), count);
}

void DirectXMesh::updateVertices(const platypus::graphics::DrawableVertex* vertices, uint64_t count)
{
	auto mesh_res = this->_mesh.lock();
	if (mesh_res == nullptr)
	{
		// error
		return;
	}

	auto current_vertices = mesh_res->getEditableVertices();

	if (count != mesh_res->getVertexCount())
		current_vertices->resize(count);

	memcpy(current_vertices->data(), vertices, count * sizeof(platypus::graphics::DrawableVertex));

	this->updateVertexBuffer();
}

void DirectXMesh::updateCoords(const Vec3* coords, uint64_t count)
{
	auto mesh_res = this->_mesh.lock();
	if (mesh_res == nullptr)
	{
		// error
		return;
	}

	auto current_vertices = mesh_res->getEditableVertices();

	for (uint64_t i = 0; i < count; ++i)
	{
		(*current_vertices)[i].coord_x = coords[i].x;
		(*current_vertices)[i].coord_y = coords[i].y;
		(*current_vertices)[i].coord_z = coords[i].z;
	}

	this->updateVertexBuffer();
}

void DirectXMesh::updateNormals(const Vec3* normals, uint64_t count)
{
	auto mesh_res = this->_mesh.lock();
	if (mesh_res == nullptr)
	{
		// error
		return;
	}

	auto current_vertices = mesh_res->getEditableVertices();

	for (uint64_t i = 0; i < count; ++i)
	{
		(*current_vertices)[i].norm_x = normals[i].x;
		(*current_vertices)[i].norm_y = normals[i].y;
		(*current_vertices)[i].norm_z = normals[i].z;
	}

	this->updateVertexBuffer();
}

void DirectXMesh::updateTextureCoords(const Vec2* texture_coords, uint64_t count)
{
	auto mesh_res = this->_mesh.lock();
	if (mesh_res == nullptr)
	{
		// error
		return;
	}

	auto current_vertices = mesh_res->getEditableVertices();

	for (uint64_t i = 0; i < count; ++i)
	{
		(*current_vertices)[i].text_coord_x = texture_coords[i].x;
		(*current_vertices)[i].text_coord_y = texture_coords[i].y;
	}

	this->updateVertexBuffer();
}

void DirectXMesh::updateVertexBuffer()
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
	auto vertices = mesh_res->getEditableVertices();

	UINT subresource = 0;
	UINT map_flags = 0;
	D3D11_MAP map_type = D3D11_MAP::D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE resource;
	dx_renderer->context()->Map(vertex_buffer, subresource, map_type, map_flags, &resource);

	auto vert_data_size = mesh_res->getVertexCount() * sizeof(platypus::graphics::DrawableVertex);
	memcpy(resource.pData, vertices->data(), vert_data_size);

	dx_renderer->context()->Unmap(vertex_buffer, subresource);
}

// void DirectXMesh::updateMaterial(const Material& material) const
// {
// 	auto material_buffer = reinterpret_cast<ID3D11Buffer*>(mesh_res->getMaterialBuffer());

// 	// material
// 	D3D11_MAPPED_SUBRESOURCE mapped_resource;
// 	ZeroMemory(&mapped_resource, sizeof(mapped_resource));
// 	context->Map(material_buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

// 	auto material = (platypus::graphics::ConstantBuffer_Material*)mapped_resource.pData;
// 	material->_diffuseObjectColor = node->properties()->diffuse().toVec4().load();
// 	material->_hasTexture = !mesh_res->getTextureResourceId().empty() ? true : false;
// 	material = nullptr;

// 	context->Unmap(material_buffer, 0);

// 	context->PSSetConstantBuffers(0, 1, &material_buffer);
// }
