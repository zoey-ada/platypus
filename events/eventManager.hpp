#pragma once

#include <array>
#include <cstdint>
#include <list>
#include <map>
#include <memory>

#include <platypus_proto/settings.hpp>

#include "iEventManager.hpp"

class EventManager: public IEventManager, public std::enable_shared_from_this<EventManager>
{
public:
	explicit EventManager(const uint32_t number_of_queues = 2);
	virtual ~EventManager() = default;

	DelegateId _registerEventSink(const std::string& event_type,
		const EventDelegate& delegate) override;
	void removeEventSink(const std::string& event_type, const DelegateId id) override;

	void triggerEvent(const std::shared_ptr<IEvent>& event) override;
	EventId queueEvent(const std::shared_ptr<IEvent>& event) override;
	void abortEvent(const EventId id) override;

	void update(const Milliseconds execution_timeout = Infinity) override;

private:
	using DelegateData = std::pair<DelegateId, EventDelegate>;
	using DelegateList = std::list<DelegateData>;
	using EventDelegateMap = std::map<std::string, DelegateList>;
	using EventQueue = std::list<std::shared_ptr<IEvent>>;

	EventDelegateMap _event_delegates;
	std::list<EventQueue> _queues;
	int _number_of_queues {2};
	int _active_queue {0};
	int _last_delegate_id {InvalidDelegateId};
	int _last_event_id {InvalidEventId};
};
