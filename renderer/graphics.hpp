#pragma once

#include <vector>

#include <utilities/math/mathTypes.hpp>

class _PtVertexBuffer;
class _PtIndexBuffer;
class _PtPixelShader;
class _PtSamplerState;
class _PtTexture;
class _PtVertexShader;
class _PtInputLayout;

using PtVertexBuffer = _PtVertexBuffer*;
using PtIndexBuffer = _PtIndexBuffer*;
using PtPixelShader = _PtPixelShader*;
using PtSamplerState = _PtSamplerState*;
using PtTexture = _PtTexture*;
using PtVertexShader = _PtVertexShader*;
using PtInputLayout = _PtInputLayout*;

enum class PtPrimitiveType
{
	TriangleList,
	TriangleStrip,
	Invalid
};

namespace graphics
{

struct Vertex
{
	Vec3 coord {};
	Vec3 normal {};
	Vec2 textCoord {};
};

struct DrawableVertex
{
	float coord_x;
	float coord_y;
	float coord_z;

	float norm_x;
	float norm_y;
	float norm_z;

	float text_coord_x;
	float text_coord_y;
};

DrawableVertex drawable(const Vertex& vertex);

std::vector<DrawableVertex> drawable(const std::vector<Vertex>& vertices);

}
