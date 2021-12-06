#pragma once

#include "IRenderer.hpp"

#include <d3d11.h>

class DirectXRenderer : public IRenderer
{
public:
	DirectXRenderer() = delete;
	DirectXRenderer(HWND hwnd, HINSTANCE hinstance);
	virtual ~DirectXRenderer() { DirectXRenderer::shutdown(); }

	bool initialize(const RendererSettings& settings) override;
	void shutdown() override;

	bool preRender() override;
	bool postRender() override;

	void setBackgroundColor(const Color& backgroundColor) override;

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
};
