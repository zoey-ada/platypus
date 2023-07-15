#pragma once

#include <memory>

class IRenderer;
class Scene;
class SceneNode;
class Vec2;
class Vec3;

namespace platypus
{
namespace graphics
{
struct DrawableVertex;
struct Vertex;
}
}

class IMesh
{
public:
	virtual ~IMesh() = default;

	virtual bool initialize() = 0;
	virtual bool reinitialize() = 0;
	virtual void deinitialize() = 0;

	virtual bool setupRender(const std::shared_ptr<Scene>& scene,
		const std::shared_ptr<SceneNode>& node) = 0;
	virtual void render(const std::shared_ptr<IRenderer>& renderer) = 0;

	virtual uint64_t getVertexCount() const = 0;
	virtual void updateVertices(const platypus::graphics::Vertex* vertices, uint64_t count) = 0;
	virtual void updateVertices(const platypus::graphics::DrawableVertex* vertices,
		uint64_t count) = 0;
	virtual void updateCoords(const Vec3* coords, uint64_t count) = 0;
	virtual void updateNormals(const Vec3* normals, uint64_t count) = 0;
	virtual void updateTextureCoords(const Vec2* texture_coords, uint64_t count) = 0;
};
