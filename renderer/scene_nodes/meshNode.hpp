#pragma once

#include <memory>

#include "sceneNode.hpp"

class IPixelShader;
class IRenderer;
class IVertexShader;
class Scene;

struct ID3D11Buffer;
class DirectXRenderer;

struct PtMeshNodeData
{
	const char* mesh_path;
	const char* texture_path;
	const char* pixel_shader_path;
	const char* vetex_shader_path;
};

class MeshNode: public SceneNode, public std::enable_shared_from_this<MeshNode>
{
public:
	MeshNode(PtSceneNodeData* base_node_data, PtMeshNodeData* mesh_node_data);

	virtual ~MeshNode() = default;

	[[nodiscard]] bool initialize(const std::shared_ptr<Scene>& scene) override;
	// bool OnLostDevice(std::shared_ptr<Scene> /*scene*/) override { return true; }
	bool render(const std::shared_ptr<Scene>& scene) override;

private:
	std::string _mesh_path;
	std::string _texture_path;
	std::string _pixel_shader_path;
	std::string _vertex_shader_path;
	std::shared_ptr<IPixelShader> _pixel_shader;
	std::shared_ptr<IVertexShader> _vertex_shader;

	std::shared_ptr<ID3D11Buffer> _vertexBuffer;
};
