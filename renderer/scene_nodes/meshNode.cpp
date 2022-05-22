#include "meshNode.hpp"

#include <resource_cache/resourceCache.hpp>
#include <resource_cache/resources/meshResource.hpp>

#include "../iPixelShader.hpp"
#include "../iRenderer.hpp"
#include "../iVertexShader.hpp"
#include "../scene.hpp"

MeshNode::MeshNode(const std::string& name, const std::string& mesh_path,
	std::shared_ptr<IRenderer> renderer, std::shared_ptr<ResourceCache> cache, EntityId entity_id,
	RenderPass render_pass, std::shared_ptr<Mat4x4> to, Color diffuse_color)
	: SceneNode(name, entity_id, render_pass, to, diffuse_color), _meshFilename(mesh_path)
{
	this->_vertex_shader = renderer->loadVertexShader("");
	this->_pixel_shader = renderer->loadPixelShader("", "assets.zip/platypus.jpg");
}

[[nodiscard]] bool MeshNode::initialize(const std::shared_ptr<Scene>& scene)
{
	if (!SceneNode::initialize(scene) || !this->_pixel_shader->initialize(scene) ||
		!this->_vertex_shader->initialize(scene))
	{
		return false;
	}

	// force the mesh to reload
	auto resCache = scene->cache();
	auto mesh = resCache->getMesh(this->_meshFilename);

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
	auto resCache = scene->cache();
	auto meshResource = resCache->getMesh(this->_meshFilename);

	scene->renderer()->drawMesh(meshResource);
	return false;
}
