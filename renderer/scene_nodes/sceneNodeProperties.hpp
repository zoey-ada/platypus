#pragma once

#include <memory>
#include <string>

#include <entities/entity.hpp>

#include "renderPass.hpp"

class SceneNodeProperties
{
	friend class SceneNode;

public:
	[[nodiscard]] const EntityId& entityId() const { return _entity_id; }
	[[nodiscard]] const std::string& name() const { return _name; }
	[[nodiscard]] const RenderPass& renderPass() const { return _render_pass; }

private:
	EntityId _entity_id = InvalidEntityId;
	std::string _name;
	RenderPass _render_pass = RenderPass::NotRendered;
};
