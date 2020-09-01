#pragma once

#include <application_layer/window/IWindow.h>
#include "settings.h"

#include <memory>
#include <string>

class IRenderer;

class Platypus
{
public:
	explicit Platypus(const std::string& appName);
	virtual ~Platypus() = default;

	//protected:
	bool initialize();
	int run();
	void shutdown();

private:
	UpdateFunction getUpdateFunction() const;
	RenderFunction getRenderFunction() const;

	EngineSettings _settings;
	std::unique_ptr<IWindow> _window;
	std::shared_ptr<IRenderer> _renderer;
};
