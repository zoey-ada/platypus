#pragma once

#include "../settings.hpp"
#include "IRenderer.hpp"

#include <memory>

class IWindow;

class RendererFactory
{
public:
	RendererFactory() = default;
	virtual ~RendererFactory() = default;

	static std::shared_ptr<IRenderer> createRenderer(const IWindow* window, const RendererSettings& settings);
};
