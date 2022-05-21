#pragma once

#include <renderer/graphics.hpp>
#include <utilities/math/mathTypes.hpp>

#include "resource.hpp"

struct PtTextureData
{
	PtTexture texture;
	PtSamplerState sampler_state;
};

class TextureResource: public Resource
{
public:
	explicit TextureResource(PtResourceData* resource_data, PtTextureData* texture_data);
	virtual ~TextureResource() = default;

	[[nodiscard]] ResourceType type() const override { return ResourceType::Texture; }

	[[nodiscard]] PtTexture getTexture() const { return this->_texture; }
	[[nodiscard]] PtSamplerState getSamplerState() const { return this->_sampler_state; }

	[[nodiscard]] Vec2 getDimensions() const;

private:
	PtSamplerState _sampler_state {nullptr};
	PtTexture _texture {nullptr};
};
