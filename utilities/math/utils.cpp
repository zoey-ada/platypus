#include "utils.hpp"

float resolveFraction(const platypus::Fraction& frame_rate)
{
	return static_cast<float>(frame_rate.numerator()) / frame_rate.denominator();
}

float resolveAndInvertFraction(const platypus::Fraction& frame_rate)
{
	return static_cast<float>(frame_rate.denominator()) / frame_rate.numerator();
}
