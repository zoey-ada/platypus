#pragma once

#include <d3d11.h>

#include "resource.hpp"

namespace graphics
{
using VertexShader = ID3D11VertexShader;
using InputLayout = ID3D11InputLayout;
};

class VertexShaderResource: public Resource
{
public:
	explicit VertexShaderResource(std::string name, uint8_t* buffer, uint64_t size,
		std::shared_ptr<IResourceStore> store, std::shared_ptr<ResourceCache> cache,
		std::shared_ptr<graphics::VertexShader> shader,
		std::shared_ptr<graphics::InputLayout> input_layout);

	virtual ~VertexShaderResource() = default;

	[[nodiscard]] ResourceType type() const override { return ResourceType::VertexShader; }

	[[nodiscard]] std::shared_ptr<graphics::VertexShader> getShader() const
	{
		return this->_shader;
	}
	[[nodiscard]] std::shared_ptr<graphics::InputLayout> getInputLayout() const
	{
		return this->_input_layout;
	}

private:
	std::shared_ptr<graphics::VertexShader> _shader;
	std::shared_ptr<graphics::InputLayout> _input_layout;
};
