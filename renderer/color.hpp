#pragma once

#include <array>
#include <string>

#include <utilities/math/mathTypes.hpp>

class Color
{
public:
	Color()
		: Color(1.0f, 1.0f, 1.0f)
	{}

	explicit Color(const std::array<float, 4> rgba)
		: Color(rgba[0], rgba[1], rgba[2], rgba[3])
	{}

	explicit Color(const float r, const float g, const float b, const float a = 1.0f)
		: _r(r), _g(g), _b(b), _a(a)
	{}

	explicit Color(const uint16_t r, const uint16_t g, const uint16_t b, const uint16_t a = 255)
		: _r(intToFloat(r)), _g(intToFloat(g)), _b(intToFloat(b)), _a(intToFloat(a))
	{}

	explicit Color(const Vec4& rgba)
		: Color(rgba.x, rgba.y, rgba.z, rgba.w)
	{}

	[[nodiscard]] std::array<float, 4> toFloat() const;
	[[nodiscard]] Vec4 toVec4() const;

	[[nodiscard]] static Color fromHex(const std::string& hex_code);
	[[nodiscard]] static float intToFloat(const uint16_t value);
	[[nodiscard]] static uint16_t floatToInt(const float& value);

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
