#pragma once

#include <memory>
#include <type_traits>

class IEventManager;
class IPlatform;
class IWindow;
class IRenderer;
class IPhysicsSystem;

#define registrableService(service_type)                                         \
                                                                                 \
public:                                                                          \
	static void register##service_type(std::shared_ptr<I##service_type> service) \
	{                                                                            \
		ServiceProvider::_##service_type = service;                              \
	}                                                                            \
                                                                                 \
	static void unregister##service_type()                                       \
	{                                                                            \
		ServiceProvider::_##service_type.reset();                                \
	}                                                                            \
                                                                                 \
	static std::shared_ptr<I##service_type> get##service_type()                  \
	{                                                                            \
		return ServiceProvider::_##service_type;                                 \
	}                                                                            \
                                                                                 \
private:                                                                         \
	inline static std::shared_ptr<I##service_type> _##service_type = nullptr;

class ServiceProvider
{
	registrableService(Platform);
	registrableService(Window);
	registrableService(EventManager);
	registrableService(Renderer);
	registrableService(PhysicsSystem);

public:
	inline static void unregisterAllServices()
	{
		ServiceProvider::_Platform.reset();
		ServiceProvider::_Window.reset();
		ServiceProvider::_EventManager.reset();
		ServiceProvider::_Renderer.reset();
	}
};
