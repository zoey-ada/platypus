#pragma once

#include <functional>
#include <string>

#include <utilities/time.hpp>

class IEvent;

using DelegateId = uint32_t;
const DelegateId InvalidDelegateId = 0;

using EventId = uint64_t;
const EventId InvalidEventId = 0;

using EventDelegate = std::function<void(std::shared_ptr<IEvent>)>;
// void (Scene::*)(std::shared_ptr<IEvent> event
using std::placeholders::_1;
template<class T>
EventDelegate fromMemberFunc(void (T::*member_func)(std::shared_ptr<IEvent>), T* obj)
{
	return std::bind(member_func, obj, _1);
}

class IEventManager
{
public:
	virtual ~IEventManager() = default;

	virtual DelegateId _registerEventSink(const std::string& event_type,
		const EventDelegate& delegate) = 0;
	virtual void removeEventSink(const std::string& event_type, const DelegateId id) = 0;

	virtual void triggerEvent(const std::shared_ptr<IEvent>& event) = 0;
	virtual EventId queueEvent(const std::shared_ptr<IEvent>& event) = 0;
	virtual void abortEvent(const EventId id) = 0;

	virtual void update(const Milliseconds execution_timeout = Infinity) = 0;
};

/**
 * @brief Registers a member function as an event sink for the given event type.
 * @param event_type (std::string) event type
 * @param func (T::function) member function
 */
#define registerEventSink(event_type, func) \
	ServiceProvider::getEventManager()->_registerEventSink(event_type, fromMemberFunc(&func, this))
