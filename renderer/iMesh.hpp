#pragma once

#include <memory>

class IRenderer;
class Vec2;

namespace graphics
{
struct Vertex;
};

class IMesh
{
public:
	virtual ~IMesh() = default;

	virtual void render(const std::shared_ptr<IRenderer>& renderer) = 0;

	virtual uint64_t getVertexCount() const = 0;
	virtual void updateVertices(const graphics::Vertex* vertices, uint64_t count) = 0;
	virtual void updateVertexTextures(const Vec2* texture_coordinates, uint64_t count) = 0;
};
