#pragma once

#include "../settings.hpp"
#include "color.hpp"

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual const bool initialize(const RendererSettings& settings) = 0;
	virtual void shutdown() = 0;

	virtual const bool preRender() = 0;
	virtual const bool postRender() = 0;

	virtual void setBackgroundColor(const Color& backgroundColor) = 0;
};
