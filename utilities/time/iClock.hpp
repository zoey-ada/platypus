#pragma once

#include "utils.hpp"

class IClock
{
public:
	virtual ~IClock() = default;

	virtual Milliseconds getCurrentTime() = 0;

	virtual std::string getCurrentTimestamp() = 0;
};
