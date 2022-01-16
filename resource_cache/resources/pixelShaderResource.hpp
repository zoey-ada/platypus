#pragma once

#include "resource.hpp"

#include <d3d11.h>

namespace graphics
{
	using PixelShader = ID3D11PixelShader;
};

class PixelShaderResource: public Resource
{
public:
	explicit PixelShaderResource(std::string name, uint8_t* buffer,
		unsigned int size, std::shared_ptr<IResourceStore> store,
		std::shared_ptr<ResourceCache> cache,
		std::shared_ptr<graphics::PixelShader> shader);

	virtual ~PixelShaderResource() = default;

	ResourceType type() const override { return ResourceType::PixelShader; }

	std::shared_ptr<graphics::PixelShader> getShader() const { return this->_shader; }

private:
	std::shared_ptr<graphics::PixelShader> _shader;
};
