#pragma once

#include <memory>

#include "sceneNode.hpp"

class IPixelShader;
class IRenderer;
class IVertexShader;
class ResourceCache;
class Scene;

struct PtTextNodeDesc
{
	const char* pixel_shader_path;
	const char* vertex_shader_path;
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

	void setText(const char* new_text);
	void generateTexture(const std::shared_ptr<ResourceCache>& cache,
		const std::shared_ptr<IRenderer>& renderer);

private:
	bool _texture_is_out_of_date {false};
	std::string _message;
	std::string _pixel_shader_path;
	std::string _vertex_shader_path;
	std::shared_ptr<IPixelShader> _pixel_shader;
	std::shared_ptr<IVertexShader> _vertex_shader;
};
