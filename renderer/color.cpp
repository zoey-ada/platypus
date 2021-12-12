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

Color Color::fromHex(const std::string& hex_code)
{
	if (hex_code.at(0) == '#')
	{
		if (hex_code.length() == 4)
		{
			char temp[3] = { hex_code[1], hex_code[1], '\0' };
			uint16_t r = static_cast<uint16_t>(strtoul(temp, nullptr, 16));

			temp[0] = temp[1] = hex_code[2];
			uint16_t g = static_cast<uint16_t>(strtoul(temp, nullptr, 16));

			temp[0] = temp[1] = hex_code[3];
			uint16_t b = static_cast<uint16_t>(strtoul(temp, nullptr, 16));

			return Color(r, g, b);
		}
		else if (hex_code.length() == 7)
		{
			char temp[3] = { hex_code[1], hex_code[2], '\0' };
			uint16_t r = static_cast<uint16_t>(strtoul(temp, nullptr, 16));

			temp[0] = hex_code[3];
			temp[1] = hex_code[4];
			uint16_t g = static_cast<uint16_t>(strtoul(temp, nullptr, 16));

			temp[0] = hex_code[5];
			temp[1] = hex_code[6];
			uint16_t b = static_cast<uint16_t>(strtoul(temp, nullptr, 16));

			return Color(r, g, b);
		}
	}

	return Color::black;
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
