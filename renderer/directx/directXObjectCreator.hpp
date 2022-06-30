#pragma once

#include <memory>

#include <d3d11.h>
#include <wincodec.h>

#include "../graphics.hpp"
#include "../ptAddressOverscanMode.hpp"

class DirectXRenderer;
class WicTextureLoader;
struct PtInputLayoutDesc;

class DirectXObjectCreator
{
public:
	DirectXObjectCreator() = default;
	virtual ~DirectXObjectCreator() = default;

	bool initialize(const std::shared_ptr<DirectXRenderer>& renderer);

	[[nodiscard]] ID3D11InputLayout* newInputLayout(ID3DBlob* shader_bytecode,
		const D3D11_INPUT_ELEMENT_DESC input_element_description[],
		const uint64_t number_of_elements) const;
	[[nodiscard]] ID3D11InputLayout* newInputLayout(std::byte* shader_data,
		const uint64_t data_size, const PtInputLayoutDesc* layout_elements,
		const uint64_t element_count) const;

	[[nodiscard]] ID3D11Buffer* newBuffer(const D3D11_BUFFER_DESC& buffer_desc) const;
	[[nodiscard]] ID3D11Buffer* newBuffer(const D3D11_BUFFER_DESC& buffer_desc,
		const D3D11_SUBRESOURCE_DATA& buffer_data) const;

	[[nodiscard]] ID3D11Buffer* newVertexBuffer(const graphics::Vertex* vertices,
		const uint64_t vertex_count) const;

	[[nodiscard]] ID3D11Buffer* newIndexBuffer(const uint32_t* indices,
		const uint64_t index_count) const;

	[[nodiscard]] ID3D11ShaderResourceView* newTexture(std::byte* texture_data,
		const uint64_t data_size);
	[[nodiscard]] ID3D11ShaderResourceView* newTexture(const D3D11_TEXTURE2D_DESC& texture_desc,
		const D3D11_SUBRESOURCE_DATA texture_data) const;
	[[nodiscard]] ID3D11ShaderResourceView* newTexture(const char* message, const char* font_family,
		const uint16_t point_size);

	[[nodiscard]] ID3D11SamplerState* newSamplerState(const D3D11_SAMPLER_DESC& sampler_desc) const;
	[[nodiscard]] ID3D11SamplerState* newSamplerState(
		const PtAddressOverscanMode overscan_mode) const;

private:
	std::shared_ptr<DirectXRenderer> _renderer {nullptr};
	std::shared_ptr<WicTextureLoader> _texture_loader {nullptr};
};
