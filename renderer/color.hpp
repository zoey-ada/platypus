#pragma once

#include <array>
#include <string>

class Color
{
public:
	Color()
		: Color(1.0f, 1.0f, 1.0f)
	{ }

	explicit Color(const float rgba[4])
		: Color(rgba[0], rgba[1], rgba[2], rgba[3])
	{ }

	explicit Color(const float r, const float g, const float b, const float a = 1.0f)
		: _r(r), _g(g), _b(b), _a(a)
	{ }

	explicit Color(const uint16_t r, const uint16_t g, const uint16_t b, const uint16_t a = 255)
		: _r(static_cast<float>(r)/255.0f), _g(static_cast<float>(g)/255.0f),
		_b(static_cast<float>(b)/255.0f), _a(static_cast<float>(a)/255.0f)
	{ }

	[[nodiscard]] std::array<float, 4> toFloat() const;

	[[nodiscard]] static Color fromHex(const std::string& hex_code);

	static const Color white;
	static const Color black;

	static const Color red;
	static const Color green;
	static const Color blue;

	static const Color cyan;
	static const Color magenta;
	static const Color yellow;

	static const Color grey25;
	static const Color grey40;
	static const Color grey65;

	static const Color transparent;

private:
	float _r;
	float _g;
	float _b;
	float _a;
};
