#pragma once

#include <d3d11.h>

#include "../iRenderer.hpp"
#include "directXObjectCreator.hpp"

class DxShaderManager;

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

	void enableDebugMode() override;

	std::shared_ptr<IShaderManager> shaderManager() override;

	void drawMesh(const std::shared_ptr<MeshResource>& mesh) override;
	std::shared_ptr<MeshResource> createCommonMesh(const CommonMesh mesh_type) override;

	std::shared_ptr<IRendererState> prepareAlphaPass() override;

	void setBackgroundColor(const Color& backgroundColor) override;

	void setWorldTransform(const Mat4x4& world) override;

	PtInputLayout createInputLayout(std::byte* shader_data, const uint64_t data_size,
		PtInputLayoutDesc* layout_elements, const uint64_t element_count) override;
	void destroyInputLayout(PtInputLayout layout) override;

	PtSamplerState createSamplerState(const PtAddressOverscanMode overscan_mode) override;
	void destroySamplerState(PtSamplerState sampler_state) override;

	PtTexture createTexture(std::byte* shader_data, const uint64_t data_size) override;
	void destroyTexture(PtTexture texture) override;

	PtVertexBuffer createVertexBuffer(const graphics::Vertex* vertices,
		const uint64_t vertex_count) override;
	void destroyVertexBuffer(PtVertexBuffer buffer) override;

	PtIndexBuffer createIndexBuffer(const uint32_t* indices, const uint64_t index_count) override;
	void destroyIndexBuffer(PtIndexBuffer buffer) override;

	std::shared_ptr<TextureResource> rasterizeText(const char* message, const char* font_family,
		const uint16_t point_size) override;

	std::shared_ptr<IVertexShader> loadVertexShader(std::string path) override;
	std::shared_ptr<IPixelShader> loadPixelShader(std::string path,
		std::string texture = std::string()) override;

	// std::shared_ptr<ITexture> loadTexture(std::string path) override;

	bool doesFormatSupport(DXGI_FORMAT format, D3D11_FORMAT_SUPPORT resource_type) const;

	std::shared_ptr<DirectXObjectCreator> create() { return this->_creator; }

	ID3D11DeviceContext* context() const { return this->_context; }
	ID3D11Device* device() const { return this->_device; }

private:
	HWND _hwnd {nullptr};
	HINSTANCE _hinstance {nullptr};
	ID3D11Device* _device {nullptr};
	ID3D11DeviceContext* _context {nullptr};
	IDXGISwapChain* _swapChain {nullptr};
	ID3D11RenderTargetView* _backBufferTarget {nullptr};
	ID3D11RasterizerState* _rasterizer_state {nullptr};

	D3D_FEATURE_LEVEL _featureLevel;
	D3D_DRIVER_TYPE _driverType;

	Color _backgroundColor;

	std::weak_ptr<ResourceCache> _cache;
	std::shared_ptr<DirectXObjectCreator> _creator;
	std::shared_ptr<DxShaderManager> _shader_manager;
};
