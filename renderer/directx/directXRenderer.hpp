#pragma once

#include <d3d11.h>

#include "../iRenderer.hpp"
#include "directXObjectCreator.hpp"

class DxShaderManager;
class TextRenderer;

namespace platypus
{
class IResourceCache;
class MeshResource;
class TextureResource;
};

class DirectXRenderer: public IRenderer, public std::enable_shared_from_this<DirectXRenderer>
{
	friend class DirectXObjectCreator;

public:
	DirectXRenderer() = delete;
	DirectXRenderer(HWND hwnd, HINSTANCE hinstance);
	virtual ~DirectXRenderer() { DirectXRenderer::deinitialize(); }

	bool initialize(const platypus::RendererSettings& settings,
		const std::weak_ptr<platypus::IResourceCache>& cache) override;
	void deinitialize() override;

	bool preRender() override;
	bool postRender() override;

	void enableDebugMode() override;

	std::shared_ptr<IShaderManager> shaderManager() override;

	void drawMesh(const std::shared_ptr<platypus::MeshResource>& mesh) override;
	std::shared_ptr<platypus::MeshResource> createCommonMesh(
		const CommonMesh mesh_type) const override;
	std::shared_ptr<platypus::MeshResource> createCommonMesh(const CommonMesh mesh_type,
		const std::string& resource_id) const override;

	std::shared_ptr<IRendererState> prepareAlphaPass() override;

	void setBackgroundColor(const Color& backgroundColor) override;

	void setWorldTransform(const Mat4x4& world) override;

	PtInputLayout createInputLayout(std::byte* shader_data, const uint64_t data_size,
		PtInputLayoutDesc* layout_elements, const uint64_t element_count) const override;
	void destroyInputLayout(PtInputLayout layout) const override;

	PtSamplerState createSamplerState(const PtAddressOverscanMode overscan_mode) const override;
	void destroySamplerState(PtSamplerState sampler_state) const override;

	std::optional<platypus::graphics::Texture> createTexture(
		const platypus::Data& texture_data) const override;
	void destroyTexture(platypus::graphics::TextureResource texture) const override;

	PtVertexBuffer createVertexBuffer(const platypus::graphics::Vertex* vertices,
		const uint64_t vertex_count) const override;
	void destroyVertexBuffer(PtVertexBuffer buffer) const override;

	PtIndexBuffer createIndexBuffer(const uint32_t* indices,
		const uint64_t index_count) const override;
	void destroyIndexBuffer(PtIndexBuffer buffer) const override;

	PtTextMetrics measureText(const char* message, const char* font_family,
		const uint16_t point_size) override;
	std::shared_ptr<platypus::TextureResource> rasterizeText(const char* message,
		const char* font_family, const uint16_t point_size) override;

	std::unique_ptr<IVertexShader> loadVertexShader(std::string path) const override;
	std::unique_ptr<IPixelShader> loadPixelShader(std::string path,
		std::string texture = std::string()) const override;

	std::unique_ptr<IMesh> loadMesh(const std::string& resource_id) const override;
	std::unique_ptr<IMesh> loadCommonMesh(const CommonMesh mesh_type,
		const std::string& resource_id) const override;

	// std::shared_ptr<ITexture> loadTexture(std::string path) override;

	bool doesFormatSupport(DXGI_FORMAT format, D3D11_FORMAT_SUPPORT resource_type) const;

	std::shared_ptr<DirectXObjectCreator> create() const { return this->_creator; }

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

	std::weak_ptr<platypus::IResourceCache> _cache;
	std::shared_ptr<DirectXObjectCreator> _creator;
	std::shared_ptr<DxShaderManager> _shader_manager;
	std::shared_ptr<TextRenderer> _text_renderer;
};
