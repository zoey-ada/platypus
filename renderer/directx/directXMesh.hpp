#pragma once

#include <string>

#include "../iMesh.hpp"

namespace platypus
{
class IResourceCache;
class MeshResource;
class TextureResource;
};

class DirectXMesh: public IMesh
{
public:
	DirectXMesh(std::shared_ptr<platypus::IResourceCache> cache, const std::string& resource_id);
	virtual ~DirectXMesh() = default;

	bool initialize() override { return true; }
	bool reinitialize() override { return true; }
	void deinitialize() override { return; }

	bool setupRender(const std::shared_ptr<Scene>& scene,
		const std::shared_ptr<SceneNode>& node) override;
	void render(const std::shared_ptr<IRenderer>& renderer) override;

	uint64_t getVertexCount() const override;
	void updateVertices(const platypus::graphics::Vertex* vertices, uint64_t count) override;
	void updateVertices(const platypus::graphics::DrawableVertex* vertices,
		uint64_t count) override;
	void updateCoords(const Vec3* coords, uint64_t count) override;
	void updateNormals(const Vec3* normals, uint64_t count) override;
	void updateTextureCoords(const Vec2* texture_coords, uint64_t count) override;

private:
	std::string _mesh_resource_id;
	std::weak_ptr<platypus::MeshResource> _mesh;
	std::weak_ptr<platypus::TextureResource> _texture;

	void updateVertexBuffer();
	// void setMaterial(const Material& material) const;
};
