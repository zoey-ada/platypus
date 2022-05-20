#include "meshNode.hpp"

#include <DirectXCollision.h>

#include "../../resource_cache/resourceCache.hpp"
#include "../../resource_cache/resources/meshResource.hpp"
#include "../directx/directXPixelShader.hpp"
#include "../directx/directXRenderer.hpp"
#include "../directx/directXVertexShader.hpp"
#include "../scene.hpp"

MeshNode::MeshNode(const std::string& name, const std::string& mesh_path,
	std::shared_ptr<IRenderer> renderer, std::shared_ptr<ResourceCache> cache, EntityId entity_id,
	RenderPass render_pass, std::shared_ptr<Mat4x4> to, Color diffuse_color)
	: SceneNode(name, entity_id, render_pass, to, diffuse_color), _meshFilename(mesh_path)
{
	auto d3d_renderer = std::dynamic_pointer_cast<DirectXRenderer>(renderer);
	this->_vertexShader = std::make_shared<DirectXVertexShader>(d3d_renderer, cache, "");
	this->_pixelShader =
		std::make_shared<DirectXPixelShader>(d3d_renderer, cache, "", "assets.zip/platypus.jpg");

	//this->createRectangle(nullptr, "", 360, 640, d3d_renderer);
}

// bool MeshNode::OnRestore(std::shared_ptr<Scene> scene)
// {
// 	if (!SceneNode::OnRestore(scene) || !this->_pixelShader->OnRestore(scene) ||
// 		!this->_vertexShader->OnRestore(scene))
// 	{
// 		return false;
// 	}

// 	// force the mesh to reload
// 	/*auto appLayer = std::shared_ptr<ApplicationLayerFactory>()->GetApplicationLayer();
// 	auto resCache = appLayer->_resourceCache;

// 	auto meshResource =
// 	std::dynamic_pointer_cast<MeshResource>(resCache->GetResource(this->_meshFilename));
// 	BoundingSphere sphere;
// 	BoundingSphere::CreateFromPoints(sphere, meshResource->GetVertexCount(),
// 	&(meshResource->GetVertices()[0]), sizeof(Vec3)); this->SetRadius(sphere.Radius);*/

// 	return true;
// }

void MeshNode::createRectangle(std::shared_ptr<ResourceCache>, std::string, const uint32_t height,
	const uint32_t width, std::shared_ptr<DirectXRenderer> renderer)
{
	// if (cache == nullptr)
	// 	return nullptr;

	auto screenHeight = 720.0f;

	std::vector<Vertex> vertices = {
		Vertex() = {Vec3(0, screenHeight - height, 0), Vec3(1, 1, 0), Vec2(0, 1)},
		Vertex() = {Vec3(0, screenHeight, 0), Vec3(1, 1, 0), Vec2(0, 0)},
		Vertex() = {Vec3(static_cast<float>(width), screenHeight - height, 0), Vec3(1, 1, 0), Vec2(1, 1)},
		Vertex() = {Vec3(static_cast<float>(width), screenHeight, 0), Vec3(1, 1, 0), Vec2(1, 0)}};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(DrawableVertex) * vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	auto drawable_verts = drawable(vertices);
	vertexData.pSysMem = &drawable_verts[0];

	ID3D11Buffer* v = nullptr;
	renderer->device()->CreateBuffer(&vertexBufferDesc, &vertexData, &v);

	// if (FAILED(hr))
	// 	return false;

	this->_vertexBuffer = std::shared_ptr<ID3D11Buffer>(v, [](ID3D11Buffer* v) { v->Release(); });
	// std::vector<uint32_t> indices;
}

bool MeshNode::render(const std::shared_ptr<Scene>& scene)
{
	bool success = this->_pixelShader->initialize(scene);
	success = this->_vertexShader->initialize(scene);

	if (!this->_pixelShader->setupRender(scene, this->shared_from_this()) ||
		!this->_vertexShader->setupRender(scene, this->shared_from_this()))
	{
		return false;
	}

	// get the mesh
	auto resCache = scene->cache();
	auto meshResource = std::dynamic_pointer_cast<MeshResource>(
	 	resCache->getResource(ResourceType::Mesh, this->_meshFilename));

	// IA setup
	unsigned int stride = sizeof(DrawableVertex);
	unsigned int offset = 0;

	auto renderer = std::dynamic_pointer_cast<DirectXRenderer>(scene->renderer());
	auto verts_ptr = meshResource->getVertexBuffer();
	auto verts = verts_ptr.get();
	//auto verts = _vertexBuffer.get();

	renderer->context()->IASetVertexBuffers(0, 1, &verts, &stride, &offset);
	renderer->context()->IASetIndexBuffer(meshResource->getIndexBuffer().get(),
	 	DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	// render
	renderer->context()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	renderer->context()->DrawIndexed((UINT)meshResource->getIndexCount(), 0, 0);
	//renderer->context()->Draw(4, 0);

	return false;
}
