#pragma once

#include <utilities/math/mathTypes.hpp>

float dampenValue(const float& value, const float& dampening_magnitude);
Vec3 dampenForce(const Vec3& force, const Vec3& dampening_magnitude);

float clampValueMagnitude(const float& value, const float& max_magnitude);
Vec3 clampForceMagnitude(const Vec3& force, const Vec3& max_magnitude);
