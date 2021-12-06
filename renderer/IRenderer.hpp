#pragma once

#include "../settings.hpp"
#include "color.hpp"

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual bool initialize(const RendererSettings& settings) = 0;
	virtual void shutdown() = 0;

	virtual bool preRender() = 0;
	virtual bool postRender() = 0;

	virtual void setBackgroundColor(const Color& backgroundColor) = 0;
};
