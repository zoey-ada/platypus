#pragma once

#include "entityComponent.hpp"

namespace platypus
{
class RenderComponent_3d;
}

class SceneNode;

class RenderComponent3d: public EntityComponent
{
public:
	RenderComponent3d() = default;
	virtual ~RenderComponent3d();

	[[nodiscard]] bool initialize(const std::shared_ptr<Message>& data) override;
	void postInitialize() override;

	void update(const Milliseconds delta) override;
	[[nodiscard]] const char* name() const override { return "render_component_3d"; }

	[[nodiscard]] std::shared_ptr<SceneNode> getSceneNode();

private:
	std::shared_ptr<platypus::RenderComponent_3d> _render_data;
	std::shared_ptr<SceneNode> _scene_node;
};

[[nodiscard]] std::shared_ptr<EntityComponent> createRenderComponent3d();
