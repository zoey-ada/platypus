#include "directXPixelShader.hpp"

#include <DirectXMath.h>
#include <d3d11.h>

#include <exceptions/creationException.hpp>
#include <resource_cache/iResourceCache.hpp>
#include <resource_cache/resources/pixelShaderResource.hpp>
#include <resource_cache/resources/textureResource.hpp>
#include <utilities/common/safeRelease.hpp>
#include <utilities/logging/logger.hpp>

#include "../scene.hpp"
#include "../scene_nodes/cameraNode.hpp"
#include "../scene_nodes/sceneNode.hpp"
#include "directXObjectCreator.hpp"
#include "directXRenderer.hpp"

#define _DISABLE_EXTENDED_ALIGNED_STORAGE

struct ConstantBuffer_Material
{
	DirectX::XMVECTOR _diffuseObjectColor;
	bool _hasTexture;
};

DirectXPixelShader::DirectXPixelShader(std::shared_ptr<const IRenderer> renderer,
	std::shared_ptr<platypus::IResourceCache> cache, std::string path, std::string texture_path)
	: _path(std::move(path)), _texture_path(texture_path), _resource_cache(cache)
{
	if (this->_path.empty())
		this->_path = "assets.zip/shaders/simple_pixel.cso";

	_renderer = std::dynamic_pointer_cast<const DirectXRenderer>(renderer);
	if (_renderer == nullptr)
		logWarning(
			"attempting to create DirectX pixel shader when not using DirectX "
			"renderer",
			"renderer_init");
}

DirectXPixelShader::~DirectXPixelShader()
{
	this->deinitialize();
}

bool DirectXPixelShader::initialize(const std::shared_ptr<Scene>& /*scene*/)
{
	this->deinitialize();

	auto resource = this->_resource_cache.lock()->getPixelShader(this->_path);
	if (resource == nullptr)
		return false;

	this->_pixel_shader = reinterpret_cast<ID3D11PixelShader*>(resource->getShader());

	if (this->_pixel_shader == nullptr)
		return false;

	// create buffer for shader
	D3D11_BUFFER_DESC desc {};
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = sizeof(ConstantBuffer_Material);

	try
	{
		this->_material = this->_renderer->create()->newBuffer(desc);
	}
	catch (const CreationException&)
	{
		this->deinitialize();
		return false;
	}

	if (!this->_texture_path.empty())
	{
		auto texture = this->_resource_cache.lock()->getTexture(this->_texture_path);
	}

	return true;
}

bool DirectXPixelShader::setupRender(const std::shared_ptr<Scene>& /*scene*/,
	const std::shared_ptr<SceneNode>& node)
{
	auto* context = this->_renderer->context();

	// set the pixel shader
	context->PSSetShader(this->_pixel_shader, nullptr, 0);

	// material
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	ZeroMemory(&mapped_resource, sizeof(mapped_resource));
	context->Map(this->_material, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

	auto material = (ConstantBuffer_Material*)mapped_resource.pData;
	material->_diffuseObjectColor = node->properties()->diffuse().toVec4().load();
	material->_hasTexture = !this->_texture_path.empty() ? true : false;
	material = nullptr;

	context->Unmap(this->_material, 0);

	context->PSSetConstantBuffers(0, 1, &this->_material);

	// setup the texture
	this->setTexture(this->_texture_path);

	return true;
}

std::shared_ptr<platypus::TextureResource> DirectXPixelShader::getTexture()
{
	if (!this->_texture_path.empty())
	{
		return this->_resource_cache.lock()->getTexture(this->_texture_path);
	}

	return nullptr;
}

bool DirectXPixelShader::setTexture(const std::string& texture_path)
{
	this->_texture_path = texture_path;

	if (!this->_texture_path.empty())
	{
		auto texture = this->_resource_cache.lock()->getTexture(this->_texture_path);
		if (texture)
			return this->setTexture(texture->getTexture(), texture->getSamplerState());
	}

	return true;
}

bool DirectXPixelShader::setTexture(platypus::graphics::TextureResource texture,
	platypus::graphics::SamplerState sampler_state)
{
	auto temp_texture = reinterpret_cast<ID3D11ShaderResourceView*>(texture);
	auto temp_sampler = reinterpret_cast<ID3D11SamplerState*>(sampler_state);

	this->_renderer->context()->PSSetShaderResources(0, 1, &temp_texture);
	this->_renderer->context()->PSSetSamplers(0, 1, &temp_sampler);

	return true;
}

void DirectXPixelShader::deinitialize()
{
	safeRelease(&this->_material);
}
