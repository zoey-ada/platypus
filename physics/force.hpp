#pragma once

#include <utilities/math/mathTypes.hpp>
#include <utilities/time/utils.hpp>

using ForceId = uint64_t;
const ForceId InvalidForceId = 0;

struct Force
{
	ForceId id;
	Vec3 vector;
	Milliseconds timeout = 0;
};
