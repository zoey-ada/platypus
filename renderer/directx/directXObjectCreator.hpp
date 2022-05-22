#pragma once

#include <memory>

#include <d3d11.h>
#include <wincodec.h>

class DirectXRenderer;

class DirectXObjectCreator
{
public:
	DirectXObjectCreator() = default;
	virtual ~DirectXObjectCreator() = default;

	bool initialize(const std::shared_ptr<DirectXRenderer>& renderer);

	[[nodiscard]] ID3D11VertexShader* newVertexShader(ID3DBlob* shader_bytecode) const;

	[[nodiscard]] ID3D11PixelShader* newPixelShader(ID3DBlob* shader_bytecode) const;

	[[nodiscard]] ID3D11InputLayout* newInputLayout(ID3DBlob* shader_bytecode,
		const D3D11_INPUT_ELEMENT_DESC input_element_description[],
		const unsigned int number_of_elements) const;

	[[nodiscard]] ID3D11Buffer* newBuffer(const D3D11_BUFFER_DESC& buffer_desc) const;
	[[nodiscard]] ID3D11Buffer* newBuffer(const D3D11_BUFFER_DESC& buffer_desc,
		const D3D11_SUBRESOURCE_DATA& buffer_data) const;

	[[nodiscard]] ID3D11ShaderResourceView* newTexture(const D3D11_TEXTURE2D_DESC& texture_desc,
		const D3D11_SUBRESOURCE_DATA texture_data) const;

	[[nodiscard]] ID3D11SamplerState* newSamplerState(const D3D11_SAMPLER_DESC& sampler_desc) const;

	[[nodiscard]] IWICStream* newWicStream() const;
	[[nodiscard]] IWICFormatConverter* newWicFormatConverter() const;
	[[nodiscard]] IWICBitmapDecoder* newWicBitmapDecoder(IWICStream* stream) const;
	[[nodiscard]] IWICComponentInfo* newWicComponentInfo(
		const WICPixelFormatGUID& pixel_format) const;

private:
	std::shared_ptr<DirectXRenderer> _renderer {nullptr};
	std::shared_ptr<IWICImagingFactory> _wic_factory {nullptr};
};
