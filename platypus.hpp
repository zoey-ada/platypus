#pragma once

#include <memory>
#include <string>

#include <application_layer/window/iWindow.hpp>
#include <platypus_proto/settings.hpp>

class BaseGameLogic;
class IEventManager;
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
	std::shared_ptr<IEventManager> _event_manager;

private:
	[[nodiscard]] UpdateFunction getUpdateFunction() const;
	[[nodiscard]] RenderFunction getRenderFunction() const;

	std::unique_ptr<IWindow> _window;
	std::shared_ptr<BaseGameLogic> _logic;
};
