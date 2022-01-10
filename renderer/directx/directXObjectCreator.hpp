#pragma once

#include <memory>

#include <d3d11.h>

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

private:
	std::shared_ptr<DirectXRenderer> _renderer;
};
