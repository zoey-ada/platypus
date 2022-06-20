#pragma once

#include <platypus_proto/common.hpp>

#include "../time/utils.hpp"

float resolveFraction(const platypus::Fraction& frame_rate);
float resolveAndInvertFraction(const platypus::Fraction& frame_rate);

Milliseconds frametimeFromFrameRate(const platypus::Fraction& frame_rate);
Milliseconds frametimeFromFrameRate(const float frame_rate);
