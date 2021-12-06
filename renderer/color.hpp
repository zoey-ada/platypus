#pragma once

#include <array>

class Color
{
public:
	Color()
		: Color(1.0f, 1.0f, 1.0f)
	{ }

	explicit Color(const float rgba[4])
		: Color(rgba[0], rgba[1], rgba[2], rgba[3])
	{ }

	Color(const float r, const float g, const float b, const float a = 1.0)
		: _r(r), _g(g), _b(b), _a(a)
	{ }

	[[nodiscard]] std::array<float, 4> toFloat() const;

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
