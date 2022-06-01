#pragma once

#include <memory>
#include <type_traits>

class IEventManager;
class IPlatform;
class IWindow;

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
	registrableService(EventManager);

public:
	inline static void unregisterAllServices()
	{
		ServiceProvider::_EventManager.reset();
	}
};
