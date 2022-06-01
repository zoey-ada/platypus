#pragma once

#include "inputType.hpp"

using InputState = union
{
	bool pressed;
	float analog_value;
};

struct Input
{
	InputType type;
	bool is_analog;
	InputState current_state;
	InputState previous_state;
};
