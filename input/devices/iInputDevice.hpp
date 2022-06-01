#pragma once

#include <list>
#include <map>

#include "../input.hpp"
#include "../inputType.hpp"

class IInputDevice
{
public:
	virtual bool initialize() = 0;
	virtual void deinitialize() = 0;

	virtual void registerInputs(std::list<InputType> input_types) = 0;
	virtual std::map<InputType, Input>& readState() = 0;
	virtual void updateState() = 0;
};
