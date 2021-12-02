#pragma once

#include <application_layer/window/IWindow.hpp>
#include "settings.hpp"

#include <memory>
#include <string>

class BaseGameLogic;
class IRenderer;

class Platypus
{
public:
	explicit Platypus(const std::string& appName);
	virtual ~Platypus() = default;

protected:
	const bool initialize();
	const int run();
	void shutdown();

	virtual const std::shared_ptr<BaseGameLogic> createLogicAndView() = 0;

	EngineSettings _settings;
	std::shared_ptr<IRenderer> _renderer;

private:
	UpdateFunction getUpdateFunction() const;
	RenderFunction getRenderFunction() const;

	std::unique_ptr<IWindow> _window;
	std::shared_ptr<BaseGameLogic> _logic;
};
