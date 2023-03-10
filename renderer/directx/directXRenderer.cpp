#include "directXRenderer.hpp"

#include <array>

#include <resource_cache/resources/meshResource.hpp>
#include <resource_cache/resources/resource.hpp>
#include <resource_cache/resources/resourceType.hpp>
#include <utilities/common/safeRelease.hpp>

#include "directXAlphaPass.hpp"
#include "directXPixelShader.hpp"
#include "directXVertexShader.hpp"
#include "dxCommonMeshes.hpp"
#include "dxShaderManager.hpp"

DirectXRenderer::DirectXRenderer(HWND hwnd, HINSTANCE hinstance)
	: _hwnd(hwnd),
	  _hinstance(hinstance),
	  _device(nullptr),
	  _context(nullptr),
	  _swapChain(nullptr),
	  _backBufferTarget(nullptr),
	  _featureLevel(D3D_FEATURE_LEVEL_1_0_CORE),
	  _driverType(D3D_DRIVER_TYPE_UNKNOWN),
	  _backgroundColor(Color::black)
{}

bool DirectXRenderer::initialize(const platypus::RendererSettings& settings,
	const std::weak_ptr<ResourceCache>& cache)
{
	this->_cache = cache;

	this->setBackgroundColor(Color::fromHex(settings.background_color()));

	// prioritized list of feature levels
	std::array<D3D_FEATURE_LEVEL, 4> featureLevels = {D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0};

	// prioritized list of driver types
	std::array<D3D_DRIVER_TYPE, 3> driverTypes = {D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE};

	// get the window dimensions
	RECT dimensions;
	ZeroMemory(&dimensions, sizeof(RECT));
	GetClientRect(this->_hwnd, &dimensions);
	unsigned int height = dimensions.bottom - dimensions.top;
	unsigned int width = dimensions.right - dimensions.left;

	// swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator =
		static_cast<UINT>(settings.frame_rate().numerator());
	swapChainDesc.BufferDesc.RefreshRate.Denominator =
		static_cast<UINT>(settings.frame_rate().denominator());
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = this->_hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = settings.full_screen() ? FALSE : TRUE;

	// set the creation flags
	unsigned int creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// create the device, context, and swap chain
	HRESULT hr = S_OK;
	for (const auto driver : driverTypes)
	{
		hr = D3D11CreateDeviceAndSwapChain(nullptr, driver, nullptr, creationFlags,
			featureLevels._Elems, static_cast<UINT>(featureLevels.size()), D3D11_SDK_VERSION,
			&swapChainDesc, &this->_swapChain, &this->_device, &this->_featureLevel,
			&this->_context);

		if (SUCCEEDED(hr))
		{
			this->_driverType = driver;
			break;
		}
	}

	if (FAILED(hr))
		return false;

	// setup the back buffer
	ID3D11Texture2D* backBufferTexture = nullptr;
	hr = this->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);

	if (FAILED(hr))
		return false;

	hr =
		this->_device->CreateRenderTargetView(backBufferTexture, nullptr, &this->_backBufferTarget);

	if (backBufferTexture != nullptr)
		backBufferTexture->Release();
	backBufferTexture = nullptr;

	if (FAILED(hr))
		return false;

	this->_context->OMSetRenderTargets(1, &this->_backBufferTarget, nullptr);

	// setup the viewport
	D3D11_VIEWPORT viewport;
	viewport.Height = static_cast<float>(height);
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width);

	this->_context->RSSetViewports(1, &viewport);

	this->_creator = std::make_shared<DirectXObjectCreator>();
	if (!this->_creator->initialize(this->shared_from_this()))
	{
		// error
		return false;
	}

	this->_shader_manager = std::make_shared<DxShaderManager>();
	if (!this->_shader_manager->initialize(this->_device))
	{
		// error
		return false;
	}

	this->_text_renderer = std::make_shared<TextRenderer>();
	if (!this->_text_renderer->initialize())
	{
		// error
		return false;
	}

	return true;
}

void DirectXRenderer::deinitialize()
{
	this->_shader_manager->deinitialize();

	safeRelease(&this->_backBufferTarget);
	safeRelease(&this->_swapChain);
	safeRelease(&this->_context);
	safeRelease(&this->_device);
	safeRelease(&this->_rasterizer_state);

	_hwnd = nullptr;
	_hinstance = nullptr;
}

bool DirectXRenderer::preRender()
{
	if (_context != nullptr && _backBufferTarget != nullptr)
	{
		_context->ClearRenderTargetView(_backBufferTarget, this->_backgroundColor.toFloat()._Elems);
	}

	return true;
}

bool DirectXRenderer::postRender()
{
	HRESULT hr = _swapChain->Present(0, 0);
	return SUCCEEDED(hr);
}

void DirectXRenderer::enableDebugMode()
{
	D3D11_RASTERIZER_DESC rasterizer_state {};
	rasterizer_state.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	rasterizer_state.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

	this->_device->CreateRasterizerState(&rasterizer_state, &this->_rasterizer_state);
	this->_context->RSSetState(this->_rasterizer_state);
	//   D3D11_FILL_MODE FillMode;
	//   D3D11_CULL_MODE CullMode;
	//   BOOL            FrontCounterClockwise;
	//   INT             DepthBias;
	//   FLOAT           DepthBiasClamp;
	//   FLOAT           SlopeScaledDepthBias;
	//   BOOL            DepthClipEnable;
	//   BOOL            ScissorEnable;
	//   BOOL            MultisampleEnable;
	//   BOOL            AntialiasedLineEnable;
}

std::shared_ptr<IShaderManager> DirectXRenderer::shaderManager()
{
	return this->_shader_manager;
}

void DirectXRenderer::drawMesh(const std::shared_ptr<MeshResource>& mesh)
{
	// IA setup
	unsigned int stride = sizeof(graphics::DrawableVertex);
	unsigned int offset = 0;

	auto vertex_buffer = reinterpret_cast<ID3D11Buffer*>(mesh->getVertexBuffer());
	auto index_buffer = reinterpret_cast<ID3D11Buffer*>(mesh->getIndexBuffer());

	this->context()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	this->context()->IASetIndexBuffer(index_buffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	// render
	switch (mesh->getPrimitiveType())
	{
	case PtPrimitiveType::TriangleList:
	{
		this->context()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	}
	case PtPrimitiveType::TriangleStrip:
	{
		this->context()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;
	}
	case PtPrimitiveType::Invalid:
		// error log
		return;
	}
	this->context()->DrawIndexed((UINT)mesh->getIndexCount(), 0, 0);
}

std::shared_ptr<MeshResource> DirectXRenderer::createCommonMesh(const CommonMesh mesh_type)
{
	return ::createCommonMesh(mesh_type, this->shared_from_this());
}

std::shared_ptr<IRendererState> DirectXRenderer::prepareAlphaPass()
{
	return std::make_shared<DirectXAlphaPass>(this->shared_from_this());
}

void DirectXRenderer::setBackgroundColor(const Color& backgroundColor)
{
	this->_backgroundColor = backgroundColor;
}

void DirectXRenderer::setWorldTransform(const Mat4x4& /*world*/)
{}

PtInputLayout DirectXRenderer::createInputLayout(std::byte* shader_data, const uint64_t data_size,
	PtInputLayoutDesc* layout_elements, const uint64_t element_count)
{
	return (PtInputLayout)this->_creator->newInputLayout(shader_data, data_size, layout_elements,
		element_count);
}

void DirectXRenderer::destroyInputLayout(PtInputLayout layout)
{
	reinterpret_cast<ID3D11InputLayout*>(layout)->Release();
}

PtSamplerState DirectXRenderer::createSamplerState(const PtAddressOverscanMode overscan_mode)
{
	return (PtSamplerState)this->_creator->newSamplerState(overscan_mode);
}

void DirectXRenderer::destroySamplerState(PtSamplerState sampler_state)
{
	reinterpret_cast<ID3D11SamplerState*>(sampler_state)->Release();
}

PtTexture DirectXRenderer::createTexture(std::byte* texture_data, const uint64_t data_size)
{
	return (PtTexture)this->_creator->newTexture(texture_data, data_size);
}

void DirectXRenderer::destroyTexture(PtTexture texture)
{
	reinterpret_cast<ID3D11ShaderResourceView*>(texture)->Release();
}

PtVertexBuffer DirectXRenderer::createVertexBuffer(const graphics::Vertex* vertices,
	const uint64_t vertex_count)
{
	return (PtVertexBuffer)this->_creator->newVertexBuffer(vertices, vertex_count);
}

void DirectXRenderer::destroyVertexBuffer(PtVertexBuffer buffer)
{
	reinterpret_cast<ID3D11Buffer*>(buffer)->Release();
}

PtIndexBuffer DirectXRenderer::createIndexBuffer(const uint32_t* indices,
	const uint64_t index_count)
{
	return (PtIndexBuffer)this->_creator->newIndexBuffer(indices, index_count);
}

void DirectXRenderer::destroyIndexBuffer(PtIndexBuffer buffer)
{
	reinterpret_cast<ID3D11Buffer*>(buffer)->Release();
}

PtTextMetrics DirectXRenderer::measureText(const char* message, const char* font_family,
	const uint16_t point_size)
{
	if (!this->_text_renderer->loadFont(font_family) ||
		!this->_text_renderer->setFontSize(point_size))
	{
		return {message, {0, 0}, 0, 0};
	}

	return this->_text_renderer->measureText(message);
}

std::shared_ptr<TextureResource> DirectXRenderer::rasterizeText(const char* message,
	const char* font_family, const uint16_t point_size)
{
	auto texture = (PtTexture)this->_creator->newTexture(message, font_family, point_size);
	if (texture == nullptr)
	{
		// log error
		return nullptr;
	}

	auto sampler_state = this->createSamplerState(PtAddressOverscanMode::Clamp);

	PtTextureData texture_data {};
	texture_data.resource_id = message;
	texture_data.store_id = "internal";
	texture_data.size = 0;
	texture_data.texture = texture;
	texture_data.sampler_state = sampler_state;

	return std::make_shared<TextureResource>(&texture_data);
}

std::shared_ptr<IVertexShader> DirectXRenderer::loadVertexShader(std::string path)
{
	return std::make_shared<DirectXVertexShader>(this->shared_from_this(), this->_cache.lock(),
		path);
}

std::shared_ptr<IPixelShader> DirectXRenderer::loadPixelShader(std::string path,
	std::string texture)
{
	return std::make_shared<DirectXPixelShader>(this->shared_from_this(), this->_cache.lock(), path,
		texture);
}

// std::shared_ptr<ITexture> DirectXRenderer::loadTexture(std::string path)
// {
// 	return std::make_shared<DirectXTexture>(this->shared_from_this(), this->_cache.lock(),
// path);
// }

bool DirectXRenderer::doesFormatSupport(DXGI_FORMAT format,
	D3D11_FORMAT_SUPPORT resource_type) const
{
	UINT supported_types = 0;
	auto hr = this->_device->CheckFormatSupport(format, &supported_types);
	return SUCCEEDED(hr) && static_cast<bool>(supported_types & resource_type);
}
