#pragma once

#include <array>
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <mutex>

#include <platypus_proto/settings.hpp>

#include <utilities/logging/iLoggingSystem.hpp>

#include "iEventManager.hpp"

class EventManager: public IEventManager, public std::enable_shared_from_this<EventManager>
{
public:
	explicit EventManager(const std::shared_ptr<ILoggingSystem>& logging): _logging(logging) {}
	virtual ~EventManager() = default;

	DelegateId _registerEventSink(const std::string& event_type,
		const EventDelegate& delegate) override;
	void removeEventSink(const std::string& event_type, const DelegateId id) override;

	void triggerEvent(const std::shared_ptr<IEvent>& event) override;
	EventId queueEvent(const std::shared_ptr<IEvent>& event) override;
	void abortEvent(const EventId id) override;

	void update(const Milliseconds execution_timeout = Infinity) override;

private:
	std::shared_ptr<ILoggingSystem> _logging {nullptr};

	using DelegateData = std::pair<DelegateId, EventDelegate>;
	using DelegateList = std::list<DelegateData>;
	using EventDelegateMap = std::map<std::string, DelegateList>;

	EventDelegateMap _event_delegates;
	std::mutex _event_delegates_mutex;
	std::list<std::pair<std::string, DelegateId>> _delegate_removal_queue;

	using EventQueue = std::list<std::shared_ptr<IEvent>>;
	EventQueue _event_queue;

	int _last_delegate_id {InvalidDelegateId};
	int _last_event_id {InvalidEventId};

	void triggerEventImpl(const std::shared_ptr<IEvent>& event);
	void triggerQueuedEvents();

	void removeEventSinkImpl(const std::string& event_type, const DelegateId id);
	void handleQueuedRemovals();
};
