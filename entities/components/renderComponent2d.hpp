#pragma once

#include <optional>

#include <platypus_proto/entity.hpp>

#include "entityComponent.hpp"

namespace platypus
{
class RenderComponent_2d;
}

class SceneNode;

class RenderComponent2d: public EntityComponent
{
public:
	RenderComponent2d() = default;
	virtual ~RenderComponent2d();

	[[nodiscard]] bool initialize(const std::shared_ptr<Message>& data) override;
	void postInitialize() override;

	void update(const Milliseconds delta) override;
	[[nodiscard]] const char* name() const override { return "render_component_2d"; }

	[[nodiscard]] std::shared_ptr<SceneNode> getSceneNode();

	void playAnimation(const std::string& animation);
	void stopAnimation();

private:
	std::shared_ptr<platypus::RenderComponent_2d> _render_data;
	std::shared_ptr<SceneNode> _scene_node;

	std::string _current_animation;
	uint32_t _current_animation_frame = 0;
	Milliseconds _animation_frame_time = 0;

	std::optional<platypus::SpriteAnimation> getAnimation(std::string action) const;
	void updateAnimationFrame(const Milliseconds delta);
	void updateSprite();
};

[[nodiscard]] std::shared_ptr<EntityComponent> createRenderComponent2d();
