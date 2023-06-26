#include "eventManager.hpp"

#include "events/iEvent.hpp"

DelegateId EventManager::_registerEventSink(const std::string& event_type,
	const EventDelegate& delegate)
{
	this->_logging->verbose("event", "Registering event sink for: " + event_type);
	auto& delegate_list = this->_event_delegates[event_type];
	delegate_list.emplace_back(++this->_last_delegate_id, delegate);
	return this->_last_delegate_id;
}

void EventManager::removeEventSink(const std::string& event_type, const DelegateId id)
{
	// If we are not currently doing something with the delegate list, handle the call immediately.
	// Else queue the unregister to happen during the next update.
	if (this->_event_delegates_mutex.try_lock())
	{
		this->removeEventSinkImpl(event_type, id);
		_event_delegates_mutex.unlock();
	}
	else
	{
		this->_logging->verbose("event",
			"Queuing " + event_type + ":" + std::to_string(id) +
				" event sink for later unregistering");
		this->_delegate_removal_queue.emplace_back(event_type, id);
	}
}

void EventManager::triggerEvent(const std::shared_ptr<IEvent>& event)
{
	// If we are not currently doing something with the delegate list, handle the call immediately.
	// Else queue the event to happen during the next update.
	if (this->_event_delegates_mutex.try_lock())
	{
		this->triggerEventImpl(event);
		this->_event_delegates_mutex.unlock();
	}
	else
	{
		this->_logging->verbose("event",
			"Queuing " + std::string(event->getType()) + " event for later execution");
		this->_event_queue.push_back(event);
	}
}

EventId EventManager::queueEvent(const std::shared_ptr<IEvent>& /*event*/)
{
	return ++this->_last_event_id;
}

void EventManager::abortEvent(const EventId /*id*/)
{}

void EventManager::update(const Milliseconds /*execution_timeout*/)
{
	const std::scoped_lock lock(this->_event_delegates_mutex);
	this->handleQueuedRemovals();
	this->triggerQueuedEvents();
}

// MUST BE PERFORMED WHILE THE EVENT DELEGATE LIST IS LOCKED!!!
void EventManager::triggerEventImpl(const std::shared_ptr<IEvent>& event)
{
	this->_logging->verbose("event", "triggering " + std::string(event->getType()));
	auto delegate_list = this->_event_delegates.find(event->getType());
	if (delegate_list != this->_event_delegates.end())
	{
		for (const auto& delegate : delegate_list->second)
		{
			delegate.second(event);
		}
	}
}

void EventManager::triggerQueuedEvents()
{
	for (auto iter = _event_queue.begin(); iter != _event_queue.end(); iter = _event_queue.begin())
	{
		this->triggerEventImpl(*iter);
		_event_queue.erase(iter);
	}
}

// MUST BE PERFORMED WHILE THE EVENT DELEGATE LIST IS LOCKED!!!
void EventManager::removeEventSinkImpl(const std::string& event_type, const DelegateId id)
{
	auto delegate_list = this->_event_delegates.find(event_type);
	if (delegate_list != this->_event_delegates.end())
	{
		for (auto iter = delegate_list->second.begin(); iter != delegate_list->second.end(); ++iter)
		{
			if (iter->first == id)
			{
				this->_logging->verbose("event",
					"Unregistering event sink " + std::to_string(id) + " for: " + event_type);
				delegate_list->second.erase(iter);
				break;
			}
		}
	}
}

void EventManager::handleQueuedRemovals()
{
	for (const auto& unregister : this->_delegate_removal_queue)
	{
		this->removeEventSinkImpl(unregister.first, unregister.second);
	}
}
