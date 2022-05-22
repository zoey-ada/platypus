#pragma once

#include <application_layer/window/iWindow.hpp>
#include <platypus_proto/settings.hpp>

#include <memory>
#include <string>

class BaseGameLogic;
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
	void shutdown();

	virtual std::shared_ptr<BaseGameLogic> createLogicAndView() = 0;

	void LoadSettings();
	platypus::Settings DefaultSettings();

	platypus::Settings _settings;
	std::shared_ptr<IRenderer> _renderer;
	std::shared_ptr<ResourceCache> _cache;

private:
	[[nodiscard]] UpdateFunction getUpdateFunction() const;
	[[nodiscard]] RenderFunction getRenderFunction() const;

	std::unique_ptr<IWindow> _window;
	std::shared_ptr<BaseGameLogic> _logic;
};
