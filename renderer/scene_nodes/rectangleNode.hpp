#pragma once

#include <memory>

#include "sceneNode.hpp"

class IEvent;
class IMesh;
class IPixelShader;
class IVertexShader;
class Scene;

struct PtRectangleNodeData
{
	const char* texture_path;
	const char* pixel_shader_path;
	const char* vetex_shader_path;
};

class RectangleNode: public SceneNode, public std::enable_shared_from_this<RectangleNode>
{
public:
	RectangleNode(PtSceneNodeData* base_node_data, PtRectangleNodeData* rect_node_data);

	virtual ~RectangleNode();

	bool initialize(const std::shared_ptr<Scene>& scene) override;
	bool reinitialize(const std::shared_ptr<Scene>& scene) override;
	void deinitialize(const std::shared_ptr<Scene>& scene) override;

	// bool onLostDevice(std::shared_ptr<Scene> /*scene*/) override { return true; }
	bool render(const std::shared_ptr<Scene>& scene) override;

private:
	std::string _vertex_shader_resource_id;
	std::string _pixel_shader_resource_id;
	std::string _texture_resource_id;
	std::string _mesh_resource_id;

	std::unique_ptr<IVertexShader> _vertex_shader;
	std::unique_ptr<IPixelShader> _pixel_shader;
	std::unique_ptr<IMesh> _mesh;
};
