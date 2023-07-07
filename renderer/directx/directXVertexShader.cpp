#include "directXVertexShader.hpp"

#include <DirectXMath.h>
#include <d3d11.h>

#include <exceptions/creationException.hpp>
#include <resource_cache/resourceCache.hpp>
#include <resource_cache/resources/vertexShaderResource.hpp>
#include <utilities/common/safeRelease.hpp>
#include <utilities/logging/logger.hpp>

#include "../scene.hpp"
#include "../scene_nodes/cameraNode.hpp"
#include "../scene_nodes/sceneNode.hpp"
#include "directXObjectCreator.hpp"
#include "directXRenderer.hpp"

#define _DISABLE_EXTENDED_ALIGNED_STORAGE

struct ConstantBuffer_Matrices
{
	DirectX::XMMATRIX _worldViewProj;
	DirectX::XMMATRIX _world;
};

struct ConstantBuffer_Material
{
	DirectX::XMVECTOR _diffuseObjectColor;
	uint32_t _hasTexture;
};

DirectXVertexShader::DirectXVertexShader(std::shared_ptr<const IRenderer> renderer,
	std::weak_ptr<IResourceCache> cache, std::string path)
	: _path(std::move(path)), _resource_cache(cache)
{
	if (this->_path.empty())
		this->_path = "assets.zip/shaders/simple_vertex.cso";

	_renderer = std::dynamic_pointer_cast<const DirectXRenderer>(renderer);
	if (_renderer == nullptr)
		logWarning(
			"attempting to create DirectX vertex shader when not using DirectX "
			"renderer",
			"renderer_init");
}

DirectXVertexShader::~DirectXVertexShader()
{
	this->deinitialize();
}

bool DirectXVertexShader::initialize(const std::shared_ptr<Scene>& /*scene*/)
{
	this->deinitialize();

	auto resource = this->_resource_cache.lock()->getVertexShader(this->_path);
	if (resource == nullptr)
		return false;

	this->_vertex_shader = reinterpret_cast<ID3D11VertexShader*>(resource->getShader());
	this->_input_layout = reinterpret_cast<ID3D11InputLayout*>(resource->getInputLayout());

	if (this->_input_layout == nullptr || this->_vertex_shader == nullptr)
		return false;

	// create buffers for shader
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	try
	{
		desc.ByteWidth = sizeof(ConstantBuffer_Matrices);
		this->_matrices = this->_renderer->create()->newBuffer(desc);

		desc.ByteWidth = sizeof(ConstantBuffer_Material);
		this->_material = this->_renderer->create()->newBuffer(desc);

		// desc.ByteWidth = sizeof(ConstantBuffer_Lighting);
		// this->_lighting = this->_renderer->create()->newBuffer(desc);
	}
	catch (const CreationException&)
	{
		this->deinitialize();
		return false;
	}

	return true;
}

bool DirectXVertexShader::setupRender(const std::shared_ptr<Scene>& scene,
	const std::shared_ptr<SceneNode>& node)
{
	auto* context = this->_renderer->context();

	// set the vertex shader and vertex layout
	context->VSSetShader(this->_vertex_shader, nullptr, 0);
	context->IASetInputLayout(this->_input_layout);

	// get the projection and view matrices from the camera
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	ZeroMemory(&mapped_resource, sizeof(mapped_resource));

	context->Map(this->_matrices, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

	// transform matrices
	auto* matrices = (ConstantBuffer_Matrices*)mapped_resource.pData;
	matrices->_worldViewProj = scene->getCamera()->getWorldViewProjection(scene).transpose().load();
	matrices->_world = scene->getTopMatrix().transpose().load();

	context->Unmap(this->_matrices, 0);

	// lighting

	// material
	context->Map(this->_material, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

	auto material = (ConstantBuffer_Material*)mapped_resource.pData;
	material->_diffuseObjectColor = node->properties()->diffuse().toVec4().load();
	material->_hasTexture = false;
	material = nullptr;

	context->Unmap(this->_material, 0);

	context->VSSetConstantBuffers(0, 1, &this->_matrices);
	// context->VSSetConstantBuffers(1, 1, &this->_lighting);
	context->VSSetConstantBuffers(1, 1, &this->_material);

	return true;
}

void DirectXVertexShader::deinitialize()
{
	safeRelease(&this->_matrices);
	safeRelease(&this->_material);
	// safeRelease(&this->_lighting);
}
