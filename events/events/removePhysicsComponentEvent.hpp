#pragma once

#include <memory>

#include <entities/entity.hpp>

#include "iEvent.hpp"

class RemovePhysicsComponentEvent: public IEvent
{
public:
	explicit RemovePhysicsComponentEvent(EntityId entity_id): _entity_id(entity_id) {}
	virtual ~RemovePhysicsComponentEvent() = default;

	const char* getType() override { return "remove-physics-component-event"; }

	EntityId _entity_id {InvalidEntityId};
};
