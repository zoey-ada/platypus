#pragma once

#include <string>

#include "platypusException.hpp"

class CreationException: public PlatypusException
{
public:
	CreationException(std::string object_type, std::string message)
		: _object_type(std::move(object_type)), _message(std::move(message)) {}

	std::string _object_type;
	std::string _message;
};
