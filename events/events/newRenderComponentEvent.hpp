#pragma once

#include <memory>

#include <entities/entity.hpp>

#include "iEvent.hpp"

class ISceneNode;

class NewRenderComponentEvent: public IEvent
{
public:
	explicit NewRenderComponentEvent(EntityId entity_id, std::shared_ptr<ISceneNode> scene_node)
		: _entity_id(entity_id), _scene_node(scene_node)
	{}
	virtual ~NewRenderComponentEvent() = default;

	const char* getType() override { return "new-render-component-event"; }

	EntityId _entity_id {InvalidEntityId};
	std::shared_ptr<ISceneNode> _scene_node;
};
