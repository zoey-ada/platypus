#pragma once

#include <memory>

#include "sceneNode.hpp"

class IPixelShader;
class IVertexShader;
class Scene;

struct PtTextNodeDesc
{
	const char* pixel_shader_path;
	const char* vetex_shader_path;
	const char* message;
	PtSceneNodeData* base_node_data;
};

class TextNode: public SceneNode, public std::enable_shared_from_this<TextNode>
{
public:
	TextNode(PtTextNodeDesc* text_node_desc);
	virtual ~TextNode() = default;

	bool initialize(const std::shared_ptr<Scene>& scene) override;
	bool render(const std::shared_ptr<Scene>& scene) override;

private:
	std::string _message;
	std::string _pixel_shader_path;
	std::string _vertex_shader_path;
	std::shared_ptr<IPixelShader> _pixel_shader;
	std::shared_ptr<IVertexShader> _vertex_shader;
};
