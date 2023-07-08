#pragma once

#include <string>

#include "../iMesh.hpp"

class MeshResource;

namespace platypus
{
class IResourceCache;
};

class DirectXMesh: public IMesh
{
public:
	DirectXMesh(std::shared_ptr<platypus::IResourceCache> cache, const std::string& resource_id);
	virtual ~DirectXMesh() = default;

	void render(const std::shared_ptr<IRenderer>& renderer) override;

	uint64_t getVertexCount() const override;
	void updateVertices(const graphics::Vertex* vertices, uint64_t count) override;
	void updateVertexTextures(const Vec2* texture_coordinates, uint64_t count) override;

private:
	std::string _mesh_resource_id;
	std::weak_ptr<MeshResource> _mesh;
};
