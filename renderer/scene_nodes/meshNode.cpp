#include "meshNode.hpp"

#include <resource_cache/resourceCache.hpp>
#include <resource_cache/resources/meshResource.hpp>

#include "../iPixelShader.hpp"
#include "../iRenderer.hpp"
#include "../iVertexShader.hpp"
#include "../scene.hpp"

MeshNode::MeshNode(PtSceneNodeData* base_node_data, PtMeshNodeData* mesh_node_data)
	: SceneNode(base_node_data),
	  _mesh_path(mesh_node_data->mesh_path),
	  _texture_path(mesh_node_data->texture_path),
	  _pixel_shader_path(mesh_node_data->pixel_shader_path),
	  _vertex_shader_path(mesh_node_data->vetex_shader_path)
{}

[[nodiscard]] bool MeshNode::initialize(const std::shared_ptr<Scene>& scene)
{
	if (this->_vertex_shader == nullptr)
	{
		this->_vertex_shader = scene->renderer()->loadVertexShader(this->_vertex_shader_path);
	}
	if (this->_pixel_shader == nullptr)
	{
		this->_pixel_shader =
			scene->renderer()->loadPixelShader(this->_pixel_shader_path, this->_texture_path);
	}

	if (!SceneNode::initialize(scene) || !this->_pixel_shader->initialize(scene) ||
		!this->_vertex_shader->initialize(scene))
	{
		return false;
	}

	// force the mesh to reload
	auto mesh = scene->cache()->getMesh(this->_mesh_path);

	// calculate bounding sphere

	return true;
}

// void MeshNode::createRectangle(std::shared_ptr<ResourceCache>, std::string, const uint32_t
// height, 	const uint32_t width, std::shared_ptr<DirectXRenderer> renderer)
// {
// 	// if (cache == nullptr)
// 	// 	return nullptr;

// 	auto screenHeight = 720.0f;

// 	std::vector<graphics::Vertex> vertices = {
// 		{Vec3(0, screenHeight - height, 0), Vec3(1, 1, 0), Vec2(0, 1)},
// 		{Vec3(0, screenHeight, 0), Vec3(1, 1, 0), Vec2(0, 0)},
// 		{Vec3(static_cast<float>(width), screenHeight - height, 0), Vec3(1, 1, 0), Vec2(1, 1)},
// 		{Vec3(static_cast<float>(width), screenHeight, 0), Vec3(1, 1, 0), Vec2(1, 0)}};

// 	D3D11_BUFFER_DESC vertexBufferDesc;
// 	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
// 	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
// 	vertexBufferDesc.ByteWidth =
// 		static_cast<UINT>(sizeof(graphics::DrawableVertex) * vertices.size());
// 	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

// 	D3D11_SUBRESOURCE_DATA vertexData;
// 	ZeroMemory(&vertexData, sizeof(vertexData));
// 	auto drawable_verts = drawable(vertices);
// 	vertexData.pSysMem = &drawable_verts[0];

// 	ID3D11Buffer* v = nullptr;
// 	renderer->device()->CreateBuffer(&vertexBufferDesc, &vertexData, &v);

// 	// if (FAILED(hr))
// 	// 	return false;

// 	this->_vertexBuffer = std::shared_ptr<ID3D11Buffer>(v, [](ID3D11Buffer* v) { v->Release(); });
// 	// std::vector<uint32_t> indices;
// }

bool MeshNode::render(const std::shared_ptr<Scene>& scene)
{
	if (!this->_pixel_shader->setupRender(scene, this->shared_from_this()) ||
		!this->_vertex_shader->setupRender(scene, this->shared_from_this()))
	{
		return false;
	}

	// get the mesh
	auto mesh_resource = scene->cache()->getMesh(this->_mesh_path);
	scene->renderer()->drawMesh(mesh_resource);
	return true;
}
