#pragma once

#include <memory>

#include "sceneNode.hpp"

class IPixelShader;
class IRenderer;
class IVertexShader;
class ResourceCache;
class Scene;

struct ID3D11Buffer;
class DirectXRenderer;

class MeshNode: public SceneNode, public std::enable_shared_from_this<MeshNode>
{
public:
	MeshNode(const std::string& name, const std::string& mesh_path,
		std::shared_ptr<IRenderer> renderer, std::shared_ptr<ResourceCache> cache,
		EntityId entity_id = InvalidEntityId, RenderPass render_pass = RenderPass::Entity,
		std::shared_ptr<Mat4x4> to = Mat4x4::identity(), Color diffuse_color = Color::white);

	virtual ~MeshNode() = default;

	// bool OnRestore(std::shared_ptr<Scene> scene) override;
	// bool OnLostDevice(std::shared_ptr<Scene> /*scene*/) override { return true; }
	bool render(const std::shared_ptr<Scene>& scene) override;

private:
	std::string _meshFilename;
	std::shared_ptr<IPixelShader> _pixelShader;
	std::shared_ptr<IVertexShader> _vertexShader;

	std::shared_ptr<ID3D11Buffer> _vertexBuffer {nullptr};

	void createRectangle(std::shared_ptr<ResourceCache> cache, std::string name,
		const uint32_t height, const uint32_t width, std::shared_ptr<DirectXRenderer> renderer);
};
