#include "directXObjectCreator.hpp"

#include <exceptions/creationException.hpp>
#include <utilities/safeDelete.hpp>

#include "directXRenderer.hpp"

bool DirectXObjectCreator::initialize(const std::shared_ptr<DirectXRenderer>& renderer)
{
	if (renderer == nullptr)
		return false;

	this->_renderer = renderer;

	IWICImagingFactory* factory = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		__uuidof(IWICImagingFactory), reinterpret_cast<LPVOID*>(&factory));

	if (FAILED(hr))
	{
		return false;
	}

	this->_wic_factory = toSharedPtr(&factory);

	return true;
}

ID3D11VertexShader* DirectXObjectCreator::newVertexShader(ID3DBlob* shader_bytecode) const
{
	auto* device = this->_renderer->_device;
	if (device == nullptr)
		throw CreationException("vertex shader", "Unable to aquire D3D device.");

	ID3D11VertexShader* shader = nullptr;
	auto hr = device->CreateVertexShader(shader_bytecode->GetBufferPointer(),
		shader_bytecode->GetBufferSize(), nullptr, &shader);

	if (FAILED(hr))
		throw CreationException("vertex shader",
			"D3D device was unable to create vertex shader object.");

	return shader;
}

ID3D11PixelShader* DirectXObjectCreator::newPixelShader(ID3DBlob* shader_bytecode) const
{
	auto* device = this->_renderer->_device;
	if (device == nullptr)
		throw CreationException("pixel shader", "Unable to aquire D3D device.");

	ID3D11PixelShader* shader = nullptr;
	auto hr = device->CreatePixelShader(shader_bytecode->GetBufferPointer(),
		shader_bytecode->GetBufferSize(), nullptr, &shader);

	if (FAILED(hr))
		throw CreationException("pixel shader",
			"D3D device was unable to create pixel shader object.");

	return shader;
}

ID3D11InputLayout* DirectXObjectCreator::newInputLayout(ID3DBlob* shader_bytecode,
	const D3D11_INPUT_ELEMENT_DESC input_element_description[],
	const unsigned int number_of_elements) const
{
	auto* device = this->_renderer->_device;
	if (device == nullptr)
		throw CreationException("input layout", "Unable to aquire D3D device.");

	ID3D11InputLayout* input_layout = nullptr;
	auto hr = device->CreateInputLayout(input_element_description, number_of_elements,
		shader_bytecode->GetBufferPointer(), shader_bytecode->GetBufferSize(), &input_layout);

	if (FAILED(hr))
		throw CreationException("input layout",
			"D3D device was unable to create input layout object.");

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

ID3D11ShaderResourceView* DirectXObjectCreator::newTexture(const D3D11_TEXTURE2D_DESC& texture_desc,
	const D3D11_SUBRESOURCE_DATA texture_data) const
{
	auto* device = this->_renderer->_device;
	if (device == nullptr)
		throw CreationException("buffer", "Unable to aquire D3D device.");

	ID3D11Texture2D* texture = nullptr;
	auto hr = device->CreateTexture2D(&texture_desc, &texture_data, &texture);

	if (FAILED(hr))
		throw CreationException("texture", "D3D device was unable to create texture object.");

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_res_desc = {};
	shader_res_desc.Format = texture_desc.Format;
	shader_res_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_res_desc.Texture2D.MipLevels = 1;
	// shader_res_desc.Texture2D.MipLevels = (autogen) ? -1 : 1;

	ID3D11ShaderResourceView* texture_view = nullptr;
	hr = device->CreateShaderResourceView(texture, &shader_res_desc, &texture_view);

	if (FAILED(hr))
		throw CreationException("texture", "D3D device was unable to create texture view object.");

	return texture_view;
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

IWICStream* DirectXObjectCreator::newWicStream() const
{
	if (this->_wic_factory == nullptr)
		throw CreationException("wic_stream", "Unable to aquire WIC factory.");

	IWICStream* stream = nullptr;
	auto hr = this->_wic_factory->CreateStream(&stream);

	if (FAILED(hr))
		throw CreationException("wic_stream",
			"WIC factory was unable to create WIC stream object.");

	return stream;
}

IWICBitmapDecoder* DirectXObjectCreator::newWicBitmapDecoder(IWICStream* stream) const
{
	if (this->_wic_factory == nullptr)
		throw CreationException("wic_bitmap_decoder", "Unable to aquire WIC factory.");

	IWICBitmapDecoder* decoder = nullptr;
	auto hr = this->_wic_factory->CreateDecoderFromStream(stream, 0,
		WICDecodeOptions::WICDecodeMetadataCacheOnDemand, &decoder);

	if (FAILED(hr))
		throw CreationException("wic_bitmap_decoder",
			"WIC factory was unable to create WIC bitmap decoder object.");

	return decoder;
}

IWICComponentInfo* DirectXObjectCreator::newWicComponentInfo(
	const WICPixelFormatGUID& pixel_format) const
{
	if (this->_wic_factory == nullptr)
		throw CreationException("wic_component_info", "Unable to aquire WIC factory.");

	IWICComponentInfo* component_info = nullptr;
	auto hr = this->_wic_factory->CreateComponentInfo(pixel_format, &component_info);

	if (FAILED(hr))
		throw CreationException("wic_component_info",
			"WIC factory was unable to create WIC component info object.");

	return component_info;
}

IWICFormatConverter* DirectXObjectCreator::newWicFormatConverter() const
{
	if (this->_wic_factory == nullptr)
		throw CreationException("wic_stream", "Unable to aquire WIC factory.");

	IWICFormatConverter* converter = nullptr;
	auto hr = this->_wic_factory->CreateFormatConverter(&converter);

	if (FAILED(hr))
		throw CreationException("wic_format_converter",
			"WIC factory was unable to create WIC format converter object.");

	return converter;
}
