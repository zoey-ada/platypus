#pragma once

#include <memory>
#include <string>

#include <application_layer/window/iWindow.hpp>
#include <platypus_proto/settings.hpp>

class BaseGameLogic;
class IEventManager;
class InputManager;
class IPlatform;
class IRenderer;
class ResourceCache;

class Platypus
{
public:
	explicit Platypus(const std::string& appName);
	virtual ~Platypus() = default;

protected:
	bool initialize();
	int run();
	void deinitialize();

	virtual std::shared_ptr<BaseGameLogic> createLogicAndView() = 0;

	void LoadSettings();
	platypus::Settings DefaultSettings();

	platypus::Settings _settings;
	std::shared_ptr<IRenderer> _renderer;
	std::shared_ptr<ResourceCache> _cache;

private:
	std::shared_ptr<IPlatform> _platform;
	std::shared_ptr<IWindow> _window;
	std::shared_ptr<BaseGameLogic> _logic;
	std::shared_ptr<InputManager> _input_manager;
	std::shared_ptr<IEventManager> _event_manager;

	[[nodiscard]] UpdateFunction getUpdateFunction() const;
	[[nodiscard]] RenderFunction getRenderFunction() const;
};
