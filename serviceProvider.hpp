#pragma once

#include <memory>
#include <type_traits>

class IEventManager;
class IPlatform;
class IWindow;
class IRenderer;
class IPhysicsSystem;
class IAudioSystem;
class ILoggingSystem;
class InputManager;
class IEntityManager;
class IResourceCache;

class ServiceProvider
{
public:
	static void registerPlatform(std::shared_ptr<IPlatform> service)
	{
		ServiceProvider::_platform = service;
	}
	static void unregisterPlatform() { ServiceProvider::_platform.reset(); }
	static std::shared_ptr<IPlatform> getPlatform() { return ServiceProvider::_platform; }

	static void registerWindow(std::shared_ptr<IWindow> service)
	{
		ServiceProvider::_window = service;
	}
	static void unregisterWindow() { ServiceProvider::_window.reset(); }
	static std::shared_ptr<IWindow> getWindow() { return ServiceProvider::_window; }

	static void registerEventManager(std::shared_ptr<IEventManager> service)
	{
		ServiceProvider::_event_manager = service;
	}
	static void unregisterEventManager() { ServiceProvider::_event_manager.reset(); }
	static std::shared_ptr<IEventManager> getEventManager()
	{
		return ServiceProvider::_event_manager;
	}

	static void registerRenderer(std::shared_ptr<IRenderer> service)
	{
		ServiceProvider::_renderer = service;
	}
	static void unregisterRenderer() { ServiceProvider::_renderer.reset(); }
	static std::shared_ptr<IRenderer> getRenderer() { return ServiceProvider::_renderer; }

	static void registerPhysicsSystem(std::shared_ptr<IPhysicsSystem> service)
	{
		ServiceProvider::_physics_system = service;
	}
	static void unregisterPhysicsSystem() { ServiceProvider::_physics_system.reset(); }
	static std::shared_ptr<IPhysicsSystem> getPhysicsSystem()
	{
		return ServiceProvider::_physics_system;
	}

	static void registerAudioSystem(std::shared_ptr<IAudioSystem> service)
	{
		ServiceProvider::_audio_system = service;
	}
	static void unregisterAudioSystem() { ServiceProvider::_audio_system.reset(); }
	static std::shared_ptr<IAudioSystem> getAudioSystem() { return ServiceProvider::_audio_system; }

	static void registerLoggingSystem(std::shared_ptr<ILoggingSystem> service)
	{
		ServiceProvider::_logging_system = service;
	}
	static void unregisterLoggingSystem() { ServiceProvider::_logging_system.reset(); }
	static std::shared_ptr<ILoggingSystem> getLoggingSystem()
	{
		return ServiceProvider::_logging_system;
	}

	static void registerInputManager(std::shared_ptr<InputManager> service)
	{
		ServiceProvider::_input_manager = service;
	}
	static void unregisterInputManager() { ServiceProvider::_input_manager.reset(); }
	static std::shared_ptr<InputManager> getInputManager()
	{
		return ServiceProvider::_input_manager;
	}

	static void registerEntityManager(std::shared_ptr<IEntityManager> service)
	{
		ServiceProvider::_entity_manager = service;
	}
	static void unregisterEntityManager() { ServiceProvider::_entity_manager.reset(); }
	static std::shared_ptr<IEntityManager> getEntityManager()
	{
		return ServiceProvider::_entity_manager;
	}

	static void registerResourceCache(std::shared_ptr<IResourceCache> service)
	{
		ServiceProvider::_resource_cache = service;
	}
	static void unregisterResourceCache() { ServiceProvider::_resource_cache.reset(); }
	static std::shared_ptr<IResourceCache> getResourceCache()
	{
		return ServiceProvider::_resource_cache;
	}

	inline static void unregisterAllServices()
	{
		ServiceProvider::unregisterPlatform();
		ServiceProvider::unregisterWindow();
		ServiceProvider::unregisterEventManager();
		ServiceProvider::unregisterRenderer();
		ServiceProvider::unregisterPhysicsSystem();
		ServiceProvider::unregisterAudioSystem();
		ServiceProvider::unregisterLoggingSystem();
		ServiceProvider::unregisterInputManager();
		ServiceProvider::unregisterEntityManager();
		ServiceProvider::unregisterResourceCache();
	}

private:
	inline static std::shared_ptr<IPlatform> _platform = nullptr;
	inline static std::shared_ptr<IWindow> _window = nullptr;
	inline static std::shared_ptr<IEventManager> _event_manager = nullptr;
	inline static std::shared_ptr<IRenderer> _renderer = nullptr;
	inline static std::shared_ptr<IPhysicsSystem> _physics_system = nullptr;
	inline static std::shared_ptr<IAudioSystem> _audio_system = nullptr;
	inline static std::shared_ptr<ILoggingSystem> _logging_system = nullptr;
	inline static std::shared_ptr<InputManager> _input_manager = nullptr;
	inline static std::shared_ptr<IEntityManager> _entity_manager = nullptr;
	inline static std::shared_ptr<IResourceCache> _resource_cache = nullptr;
};
