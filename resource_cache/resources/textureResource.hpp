#pragma once

#include <renderer/graphics.hpp>
#include <utilities/common/ptExtent.hpp>

#include "resource.hpp"

namespace platypus
{

struct TextureData: public ResourceData
{
	graphics::TextureResource texture;
	graphics::SamplerState sampler_state;
	Extent dimensions;
	bool has_alpha;
};

class TextureResource: public Resource
{
public:
	explicit TextureResource(TextureData* resource_data);
	virtual ~TextureResource();

	ResourceType type() const override { return ResourceType::Texture; }

	graphics::TextureResource getTexture() const { return this->_texture; }
	graphics::SamplerState getSamplerState() const { return this->_sampler_state; }
	Extent getDimensions() const { return this->_dimensions; }
	bool hasAlpha() const { return this->_has_alpha; }

private:
	graphics::SamplerState _sampler_state {nullptr};
	graphics::TextureResource _texture {nullptr};
	Extent _dimensions;
	bool _has_alpha;
};
};
