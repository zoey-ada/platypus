#pragma once

#include <utilities/time/iClock.hpp>

class MockClock: public IClock
{
public:
	virtual ~MockClock() = default;

	Milliseconds getCurrentTime() override { return this->current_time; }

	std::string getCurrentTimestamp() override { return this->current_timestamp; }

	Milliseconds current_time {0};
	std::string current_timestamp;
};
