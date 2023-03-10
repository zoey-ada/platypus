#pragma once

#include <memory>
#include <string>

#include <platypus_proto/settings.hpp>

#include <platform/window/iWindow.hpp>

class BaseGameLogic;
class IAudioSystem;
class IClock;
class IEventManager;
class ILoggingSystem;
class InputManager;
class IPhysicsSystem;
class IPlatform;
class IRenderer;
class ResourceCache;

class Platypus
{
public:
	explicit Platypus(const char* appName);
	explicit Platypus(const char* appName, std::shared_ptr<IClock> clock);
	virtual ~Platypus() = default;

protected:
	bool initialize();
	int run();
	void deinitialize();

	virtual std::shared_ptr<BaseGameLogic> createLogicAndView() = 0;

	void LoadSettings();
	platypus::Settings DefaultSettings();

	platypus::Settings _settings;
	std::shared_ptr<IRenderer> _renderer {nullptr};
	std::shared_ptr<ResourceCache> _cache {nullptr};

private:
	std::shared_ptr<IClock> _clock {nullptr};
	std::shared_ptr<ILoggingSystem> _logging {nullptr};
	std::shared_ptr<IPlatform> _platform {nullptr};
	std::shared_ptr<IWindow> _window {nullptr};
	std::shared_ptr<BaseGameLogic> _logic {nullptr};
	std::shared_ptr<InputManager> _input_manager {nullptr};
	std::shared_ptr<IEventManager> _event_manager {nullptr};
	std::shared_ptr<IPhysicsSystem> _physics {nullptr};
	std::shared_ptr<IAudioSystem> _audio {nullptr};

	[[nodiscard]] UpdateFunction getUpdateFunction() const;
	[[nodiscard]] RenderFunction getRenderFunction() const;
};
