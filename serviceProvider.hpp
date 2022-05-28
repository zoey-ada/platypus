#pragma once

#include <memory>
#include <type_traits>

class IEventManager;

class ServiceProvider
{
public:
	template<class T>
	static void registerService(std::shared_ptr<T> service)
	{
		if (std::is_same<T, IEventManager>::value)
		{
			ServiceProvider::_event_manager = service;
		}
	}

	template<class T>
	static std::shared_ptr<T> getService()
	{
		if (std::is_same<T, IEventManager>::value)
		{
			return ServiceProvider::_event_manager;
		}
	}

private:
	static std::shared_ptr<IEventManager> _event_manager;
};
