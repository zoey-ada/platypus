#pragma once

#include "iClock.hpp"

class SystemClock: public IClock
{
public:
	SystemClock() = default;

	Milliseconds getCurrentTime() override;

	std::string getCurrentTimestamp() override;
};
