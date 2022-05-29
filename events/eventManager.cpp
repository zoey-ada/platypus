#include "eventManager.hpp"

#include "events/iEvent.hpp"

EventManager::EventManager(const uint32_t number_of_queues): _number_of_queues(number_of_queues)
{
	this->_queues.resize(this->_number_of_queues);
	for (int i = 0; i < this->_number_of_queues; ++i)
	{
		this->_queues.push_back(std::list<std::shared_ptr<IEvent>>());
	}
}

DelegateId EventManager::_registerEventSink(const std::string& event_type,
	const EventDelegate& delegate)
{
	auto& delegate_list = this->_event_delegates[event_type];
	delegate_list.push_back(std::make_pair(++this->_last_delegate_id, delegate));
	return this->_last_delegate_id;
}

void EventManager::removeEventSink(const std::string& event_type, const DelegateId id)
{
	auto delegate_list = this->_event_delegates.find(event_type);
	if (delegate_list != this->_event_delegates.end())
	{
		for (auto iter = delegate_list->second.begin(); iter != delegate_list->second.end(); ++iter)
		{
			if (iter->first == id)
			{
				delegate_list->second.erase(iter);
				break;
			}
		}
	}
}

void EventManager::triggerEvent(const std::shared_ptr<IEvent>& event)
{
	auto delegate_list = this->_event_delegates.find(event->getType());
	if (delegate_list != this->_event_delegates.end())
	{
		for (const auto delegate : delegate_list->second)
		{
			delegate.second(event);
		}
	}
}

EventId EventManager::queueEvent(const std::shared_ptr<IEvent>& /*event*/)
{
	return ++this->_last_event_id;
}

void EventManager::abortEvent(const EventId /*id*/)
{}

void EventManager::update(const Milliseconds /*execution_timeout*/)
{}
