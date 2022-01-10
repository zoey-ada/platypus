#pragma once

#include "../iRenderer.hpp"

#include <d3d11.h>

#include "directXObjectCreator.hpp"

class DirectXRenderer : public IRenderer, public std::enable_shared_from_this<DirectXRenderer>
{
friend class DirectXObjectCreator;

public:
	DirectXRenderer() = delete;
	DirectXRenderer(HWND hwnd, HINSTANCE hinstance);
	virtual ~DirectXRenderer() { DirectXRenderer::shutdown(); }

	bool initialize(const platypus::RendererSettings& settings) override;
	void shutdown() override;

	bool preRender() override;
	bool postRender() override;

	void setBackgroundColor(const Color& backgroundColor) override;

	std::shared_ptr<DirectXObjectCreator> create() { return this->_creator; }

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

	std::shared_ptr<DirectXObjectCreator> _creator;
};
