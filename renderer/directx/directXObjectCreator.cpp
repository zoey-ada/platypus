#include "directXObjectCreator.hpp"

#include <exceptions/creationException.hpp>

#include "directXRenderer.hpp"

bool DirectXObjectCreator::initialize(const std::shared_ptr<DirectXRenderer>& renderer)
{
	this->_renderer = renderer;
	return this->_renderer != nullptr;
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
		throw CreationException("vertex shader", "D3D device was unable to create vertex shader object.");

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
		throw CreationException("pixel shader", "D3D device was unable to create pixel shader object.");

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
	auto hr = device->CreateInputLayout(input_element_description,
		number_of_elements, shader_bytecode->GetBufferPointer(),
		shader_bytecode->GetBufferSize(), &input_layout);

	if (FAILED(hr))
		throw CreationException("input layout", "D3D device was unable to create input layout object.");

	return input_layout;
}
