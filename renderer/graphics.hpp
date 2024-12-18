#pragma once

#include <vector>

#include <utilities/common/ptExtent.hpp>
#include <utilities/math/mathTypes.hpp>

class _PtInputLayout;

class _PtComputeShader;
class _PtDomainShader;
class _PtGeometryShader;
class _PtHullShader;
class _PtPixelShader;
class _PtVertexShader;

using PtComputeShader = _PtComputeShader*;
using PtDomainShader = _PtDomainShader*;
using PtGeometryShader = _PtGeometryShader*;
using PtHullShader = _PtHullShader*;
using PtPixelShader = _PtPixelShader*;
using PtVertexShader = _PtVertexShader*;

using PtInputLayout = _PtInputLayout*;

enum class PtInputFormat
{
	Vec3_32bit_float,
	Vec2_32bit_float,
	Vec1_32bit_float
};

struct PtInputLayoutDesc
{
	const char* name;
	uint32_t index;
	PtInputFormat format;
};

namespace platypus
{

struct TextMetrics
{
	const char* message;
	Extent size;
	uint32_t max_ascender;
	uint32_t max_descender;
};

namespace graphics
{

class _TextureResource;
using TextureResource = _TextureResource*;

class _SamplerState;
using SamplerState = _SamplerState*;

class _ConstantBuffer;
using ConstantBuffer = _ConstantBuffer*;

class _VertexBuffer;
using VertexBuffer = _VertexBuffer*;

class _IndexBuffer;
using IndexBuffer = _IndexBuffer*;

enum class PrimitiveType
{
	TriangleList,
	TriangleStrip,
	Invalid
};

struct Texture
{
	TextureResource texture_resource;
	Extent dimensions;
	bool has_alpha;
};

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
std::vector<DrawableVertex> drawable(const Vertex* vertices, const uint64_t vertex_count);

struct ConstantBuffer_Material
{
	DirectX::XMVECTOR _diffuseObjectColor;
	bool _hasTexture;
};

}

}
