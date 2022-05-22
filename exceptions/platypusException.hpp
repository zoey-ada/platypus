#pragma once

#include <exception>

class PlatypusException: public std::exception
{
public:
	virtual ~PlatypusException() = default;
};
