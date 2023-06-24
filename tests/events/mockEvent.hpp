#pragma once

#include <memory>

#include <events/events/iEvent.hpp>

class MockEvent: public IEvent
{
public:
	explicit MockEvent() = default;
	virtual ~MockEvent() = default;

	const char* getType() override { return "mock-event"; }
};

class OtherMockEvent: public IEvent
{
public:
	explicit OtherMockEvent() = default;
	virtual ~OtherMockEvent() = default;

	const char* getType() override { return "other-mock-event"; }
};
