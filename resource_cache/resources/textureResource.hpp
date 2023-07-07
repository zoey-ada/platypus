#pragma once

#include <renderer/graphics.hpp>
#include <utilities/common/ptExtent.hpp>

#include "resource.hpp"

struct PtTextureData: public PtResourceData
{
	PtTexture texture;
	PtSamplerState sampler_state;
	platypus::Extent dimensions;
};

class TextureResource: public Resource
{
public:
	explicit TextureResource(PtTextureData* resource_data);
	virtual ~TextureResource();

	[[nodiscard]] ResourceType type() const override { return ResourceType::Texture; }

	[[nodiscard]] PtTexture getTexture() const { return this->_texture; }
	[[nodiscard]] PtSamplerState getSamplerState() const { return this->_sampler_state; }
	[[nodiscard]] platypus::Extent getDimensions() const { return this->_dimensions; }

private:
	PtSamplerState _sampler_state {nullptr};
	PtTexture _texture {nullptr};
	platypus::Extent _dimensions {};
};
