#include "directXObjectCreator.hpp"

#include <vector>

#include <exceptions/creationException.hpp>
#include <utilities/common/safeRelease.hpp>
#include <utilities/logging/logger.hpp>
#include <utilities/safeDelete.hpp>

#include "../graphics.hpp"
#include "../iShaderManager.hpp"
#include "../textRenderer.hpp"
#include "directXRenderer.hpp"
#include "directXShaderLoader.hpp"
#include "wicTextureLoader.hpp"

bool DirectXObjectCreator::initialize(const std::shared_ptr<DirectXRenderer>& renderer)
{
	if (renderer == nullptr)
		return false;

	this->_renderer = renderer;

	this->_texture_loader = std::make_shared<WicTextureLoader>();
	return this->_texture_loader->initialize();
}

ID3D11InputLayout* DirectXObjectCreator::newInputLayout(ID3DBlob* shader_bytecode,
	const D3D11_INPUT_ELEMENT_DESC input_element_description[],
	const uint64_t number_of_elements) const
{
	auto* device = this->_renderer->_device;
	if (device == nullptr)
		throw CreationException("input layout", "Unable to aquire D3D device.");

	ID3D11InputLayout* input_layout = nullptr;
	auto hr = device->CreateInputLayout(input_element_description, (UINT)number_of_elements,
		shader_bytecode->GetBufferPointer(), shader_bytecode->GetBufferSize(), &input_layout);

	if (FAILED(hr))
		throw CreationException("input layout",
			"D3D device was unable to create input layout object.");

	return input_layout;
}

DXGI_FORMAT toDirectXFormat(const PtInputFormat format)
{
	static std::map<PtInputFormat, DXGI_FORMAT> conversions {
		{PtInputFormat::Vec1_32bit_float, DXGI_FORMAT_R32_FLOAT},
		{PtInputFormat::Vec2_32bit_float, DXGI_FORMAT_R32G32_FLOAT},
		{PtInputFormat::Vec3_32bit_float, DXGI_FORMAT_R32G32B32_FLOAT}};

	return conversions.at(format);
}

[[nodiscard]] ID3D11InputLayout* DirectXObjectCreator::newInputLayout(std::byte* shader_data,
	const uint64_t data_size, const PtInputLayoutDesc* layout_elements,
	const uint64_t element_count) const
{
	ID3DBlob* bytecode = nullptr;
	if (!loadShaderFromBuffer(shader_data, data_size, &bytecode))
	{
		logWarning("Unable to load vertex shader data to bytecode", "render");
		return nullptr;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> input_desc(element_count);

	for (unsigned int i = 0; i < element_count; ++i)
	{
		input_desc[i] = {layout_elements[i].name, layout_elements[i].index,
			toDirectXFormat(layout_elements[i].format), 0, D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, 0};
	}

	ID3D11InputLayout* input_layout =
		this->newInputLayout(bytecode, input_desc.data(), element_count);

	safeRelease(&bytecode);
	return input_layout;
}

ID3D11Buffer* DirectXObjectCreator::newBuffer(const D3D11_BUFFER_DESC& buffer_desc) const
{
	auto* device = this->_renderer->_device;
	if (device == nullptr)
		throw CreationException("buffer", "Unable to aquire D3D device.");

	ID3D11Buffer* buffer = nullptr;
	auto hr = device->CreateBuffer(&buffer_desc, nullptr, &buffer);

	if (FAILED(hr))
		throw CreationException("buffer", "D3D device was unable to create buffer object.");

	return buffer;
}

ID3D11Buffer* DirectXObjectCreator::newBuffer(const D3D11_BUFFER_DESC& buffer_desc,
	const D3D11_SUBRESOURCE_DATA& buffer_data) const
{
	auto* device = this->_renderer->_device;
	if (device == nullptr)
		throw CreationException("buffer", "Unable to aquire D3D device.");

	ID3D11Buffer* buffer = nullptr;
	auto hr = device->CreateBuffer(&buffer_desc, &buffer_data, &buffer);

	if (FAILED(hr))
		throw CreationException("buffer", "D3D device was unable to create buffer object.");

	return buffer;
}

[[nodiscard]] ID3D11Buffer* DirectXObjectCreator::newVertexBuffer(const graphics::Vertex* vertices,
	const uint64_t vertex_count) const
{
	D3D11_BUFFER_DESC vertex_buffer_desc {};
	// vertex_buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	vertex_buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.MiscFlags = 0;
	vertex_buffer_desc.ByteWidth =
		static_cast<UINT>(sizeof(graphics::DrawableVertex) * vertex_count);

	D3D11_SUBRESOURCE_DATA vertex_data {};
	auto drawable_verts = drawable(vertices, vertex_count);
	vertex_data.pSysMem = drawable_verts.data();

	auto vertex_buffer = this->newBuffer(vertex_buffer_desc, vertex_data);
	return vertex_buffer;
}

[[nodiscard]] ID3D11Buffer* DirectXObjectCreator::newIndexBuffer(const uint32_t* indices,
	const uint64_t index_count) const
{
	D3D11_BUFFER_DESC index_buffer_desc {};
	index_buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
	index_buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA index_data {};
	index_data.pSysMem = &indices[0];

	auto index_buffer = this->newBuffer(index_buffer_desc, index_data);
	return index_buffer;
}

ID3D11ShaderResourceView* DirectXObjectCreator::newTexture(const D3D11_TEXTURE2D_DESC& texture_desc,
	const D3D11_SUBRESOURCE_DATA texture_data) const
{
	auto* device = this->_renderer->_device;
	if (device == nullptr)
	{
		logWarning("Unable to aquire D3D device to create texture.", "render");
		return nullptr;
	}

	ID3D11Texture2D* texture = nullptr;
	auto hr = device->CreateTexture2D(&texture_desc, &texture_data, &texture);

	if (FAILED(hr))
	{
		logWarning("Unable to create texture 2d", hr, "render");
		return nullptr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_res_desc = {};
	shader_res_desc.Format = texture_desc.Format;
	shader_res_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_res_desc.Texture2D.MipLevels = 1;
	// shader_res_desc.Texture2D.MipLevels = (autogen) ? -1 : 1;

	ID3D11ShaderResourceView* texture_view = nullptr;
	hr = device->CreateShaderResourceView(texture, &shader_res_desc, &texture_view);

	if (FAILED(hr))
	{
		logWarning("Unable to create texture view", hr, "render");
	}

	safeRelease(&texture);
	return texture_view;
}

ID3D11ShaderResourceView* DirectXObjectCreator::newTexture(std::byte* texture_data,
	const uint64_t data_size, platypus::Extent& dimensions)
{
	auto texture =
		this->_texture_loader->loadTexture(texture_data, data_size, this->_renderer, dimensions);
	return texture;
}

ID3D11ShaderResourceView* DirectXObjectCreator::newTexture(const char* message,
	const char* font_family, const uint16_t point_size)
{
	auto text_metrics = this->_renderer->measureText(message, font_family, point_size);
	auto pixel_buffer = this->_renderer->_text_renderer->rasterizeText(message, &text_metrics);

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = static_cast<UINT>(text_metrics.size.width);
	desc.Height = static_cast<UINT>(text_metrics.size.height);
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	uint64_t stride = text_metrics.size.width * 4;
	uint64_t image_data_size = stride * text_metrics.size.height;

	D3D11_SUBRESOURCE_DATA image_data;
	image_data.pSysMem = pixel_buffer;
	image_data.SysMemPitch = static_cast<UINT>(stride);
	image_data.SysMemSlicePitch = static_cast<UINT>(image_data_size);

	auto texture = this->newTexture(desc, image_data);
	return texture;
}

ID3D11SamplerState* DirectXObjectCreator::newSamplerState(
	const D3D11_SAMPLER_DESC& sampler_desc) const
{
	auto* device = this->_renderer->_device;
	if (device == nullptr)
		throw CreationException("sampler_state", "Unable to aquire D3D device.");

	ID3D11SamplerState* sampler_state = nullptr;
	auto hr = device->CreateSamplerState(&sampler_desc, &sampler_state);

	if (FAILED(hr))
		throw CreationException("sampler_state",
			"D3D device was unable to create sampler state object.");

	return sampler_state;
}

D3D11_TEXTURE_ADDRESS_MODE toDxAddressMode(const PtAddressOverscanMode overscan_mode)
{
	switch (overscan_mode)
	{
	case PtAddressOverscanMode::Border:
		return D3D11_TEXTURE_ADDRESS_BORDER;
	case PtAddressOverscanMode::Mirror:
		return D3D11_TEXTURE_ADDRESS_MIRROR;
	case PtAddressOverscanMode::MirrorOnce:
		return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	case PtAddressOverscanMode::Wrap:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	case PtAddressOverscanMode::Clamp:
	default:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	}
}

ID3D11SamplerState* DirectXObjectCreator::newSamplerState(
	const PtAddressOverscanMode overscan_mode) const
{
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	// sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = toDxAddressMode(overscan_mode);
	sampler_desc.AddressV = toDxAddressMode(overscan_mode);
	sampler_desc.AddressW = toDxAddressMode(overscan_mode);
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	auto sampler_state = this->_renderer->create()->newSamplerState(sampler_desc);
	return sampler_state;
}
