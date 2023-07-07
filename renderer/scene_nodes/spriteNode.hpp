#pragma once

#include <memory>
#include <optional>

#include <utilities/common/ptExtent.hpp>

#include "sceneNode.hpp"

class IEvent;
class IMesh;
class IPixelShader;
class IVertexShader;
class Scene;

namespace platypus
{

struct SpriteNodeData
{
	PtSceneNodeData* base_node_data;
	const char* vetex_shader_path;
	const char* pixel_shader_path;
	const char* texture_path;
	Extent sprite_dimensions;
	std::optional<Vec2> intial_sprite;
};

class SpriteNode: public SceneNode, public std::enable_shared_from_this<SpriteNode>
{
public:
	SpriteNode(SpriteNodeData* data);
	SpriteNode(SpriteNodeData data);

	virtual ~SpriteNode();

	bool initialize(const std::shared_ptr<Scene>& scene) override;
	bool reinitialize(const std::shared_ptr<Scene>& scene) override;
	void deinitialize(const std::shared_ptr<Scene>& scene) override;

	// bool onLostDevice(std::shared_ptr<Scene> /*scene*/) override { return true; }
	bool render(const std::shared_ptr<Scene>& scene) override;

	void setSprite(const Vec2& coordinate);
	void calculateSpriteOffsets(const Extent& texture_dimensions);

private:
	std::string _vertex_shader_resource_id;
	std::string _pixel_shader_resource_id;
	std::string _texture_resource_id;
	std::string _mesh_resource_id;

	std::unique_ptr<IVertexShader> _vertex_shader;
	std::unique_ptr<IPixelShader> _pixel_shader;
	std::unique_ptr<IMesh> _mesh;

	Extent _sprite_dimensions {0, 0};
	Vec2 _sprite_offsets {0, 0};
	Vec2 _current_sprite {0, 0};
};
};
