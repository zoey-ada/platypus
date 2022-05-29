#pragma once

#include <d3d11.h>

#include <resource_cache/resourceCache.hpp>

#include "../iRenderer.hpp"
#include "directXObjectCreator.hpp"

class DirectXRenderer: public IRenderer, public std::enable_shared_from_this<DirectXRenderer>
{
	friend class DirectXObjectCreator;

public:
	DirectXRenderer() = delete;
	DirectXRenderer(HWND hwnd, HINSTANCE hinstance);
	virtual ~DirectXRenderer() { DirectXRenderer::deinitialize(); }

	bool initialize(const platypus::RendererSettings& settings,
		const std::weak_ptr<ResourceCache>& cache) override;
	void deinitialize() override;

	bool preRender() override;
	bool postRender() override;

	void drawMesh(const std::shared_ptr<MeshResource>& mesh) override;

	void setBackgroundColor(const Color& backgroundColor) override;

	void setWorldTransform(const std::shared_ptr<Mat4x4>& world) override;

	std::shared_ptr<IVertexShader> loadVertexShader(std::string path) override;
	std::shared_ptr<IPixelShader> loadPixelShader(std::string path,
		std::string texture = std::string()) override;
	// std::shared_ptr<ITexture> loadTexture(std::string path) override;
	bool doesFormatSupport(DXGI_FORMAT format, D3D11_FORMAT_SUPPORT resource_type) const;

	std::shared_ptr<DirectXObjectCreator> create() { return this->_creator; }

	ID3D11DeviceContext* context() const { return this->_context; }
	ID3D11Device* device() const { return this->_device; }

private:
	HWND _hwnd;
	HINSTANCE _hinstance;
	ID3D11Device* _device;
	ID3D11DeviceContext* _context;
	IDXGISwapChain* _swapChain;
	ID3D11RenderTargetView* _backBufferTarget;

	D3D_FEATURE_LEVEL _featureLevel;
	D3D_DRIVER_TYPE _driverType;

	Color _backgroundColor;

	std::weak_ptr<ResourceCache> _cache;
	std::shared_ptr<DirectXObjectCreator> _creator;
};
