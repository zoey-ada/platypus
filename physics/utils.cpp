#include "utils.hpp"

#include <cmath>

float dampenValue(const float& value, const float& dampening_magnitude)
{
	if (value == 0.0f)
		return value;

	bool is_positive = value > 0.0f;
	float dampened_value = abs(value) - dampening_magnitude;

	if (is_positive)
		return std::max(dampened_value, 0.0f);
	else
		return std::min(-dampened_value, 0.0f);
}

Vec3 dampenForce(const Vec3& force, const Vec3& dampening_magnitude)
{
	return Vec3 {
		dampenValue(force.x, dampening_magnitude.x),
		dampenValue(force.y, dampening_magnitude.y),
		dampenValue(force.z, dampening_magnitude.z),
	};
}

float clampValueMagnitude(const float& value, const float& max_magnitude)
{
	return std::max(std::min(value, max_magnitude), -max_magnitude);
}

Vec3 clampForceMagnitude(const Vec3& force, const Vec3& max_magnitude)
{
	return Vec3 {
		clampValueMagnitude(force.x, max_magnitude.x),
		clampValueMagnitude(force.y, max_magnitude.y),
		clampValueMagnitude(force.z, max_magnitude.z),
	};
}
