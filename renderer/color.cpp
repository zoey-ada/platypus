#include "color.hpp"

std::array<float, 4> Color::toFloat() const
{
	return {
		this->_r,
		this->_g,
		this->_b,
		this->_a
	};
}

const Color Color::white = Color(1.0f, 1.0f, 1.0f);
const Color Color::black = Color(0.0f, 0.0f, 0.0f);

const Color Color::red = Color(1.0f, 0.0f, 0.0f);
const Color Color::green = Color(0.0f, 1.0f, 0.0f);
const Color Color::blue = Color(0.0f, 0.0f, 1.0f);

const Color Color::cyan = Color(0.0f, 1.0f, 1.0f);
const Color Color::magenta = Color(1.0f, 0.0f, 1.0f);
const Color Color::yellow = Color(1.0f, 1.0f, 0.0f);

const Color Color::grey25 = Color(0.25f, 0.25f, 0.25f);
const Color Color::grey40 = Color(0.4f, 0.4f, 0.4f);
const Color Color::grey65 = Color(0.65f, 0.65f, 0.65f);

const Color Color::transparent = Color(0.0f, 0.0f, 0.0f, 0.0f);
