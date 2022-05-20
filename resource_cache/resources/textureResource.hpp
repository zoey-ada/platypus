#pragma once

#include <d3d11.h>

#include <utilities/math/mathTypes.hpp>

#include "resource.hpp"

namespace graphics
{

using SamplerState = ID3D11SamplerState;
using Texture = ID3D11ShaderResourceView;

};

class TextureResource: public Resource
{
public:
	explicit TextureResource(std::string name, uint8_t* buffer, uint64_t size,
		std::shared_ptr<IResourceStore> store, std::shared_ptr<ResourceCache> cache,
		std::shared_ptr<graphics::Texture> texture,
		std::shared_ptr<graphics::SamplerState> sampler_state);

	virtual ~TextureResource() = default;

	[[nodiscard]] ResourceType type() const override { return ResourceType::Texture; }

	[[nodiscard]] std::shared_ptr<graphics::Texture> getTexture() const { return this->_texture; }
	[[nodiscard]] std::shared_ptr<graphics::SamplerState> getSamplerState() const
	{
		return this->_sampler_state;
	}

	[[nodiscard]] Vec2 getDimensions() const;

private:
	std::shared_ptr<graphics::SamplerState> _sampler_state {};
	std::shared_ptr<graphics::Texture> _texture {};
};
