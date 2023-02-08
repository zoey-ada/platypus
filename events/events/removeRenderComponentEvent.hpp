#pragma once

#include <memory>

#include <entities/entity.hpp>

#include "iEvent.hpp"

class RemoveRenderComponentEvent: public IEvent
{
public:
	explicit RemoveRenderComponentEvent(EntityId entity_id): _entity_id(entity_id) {}
	virtual ~RemoveRenderComponentEvent() = default;

	const char* getType() override { return "remove-render-component-event"; }

	EntityId _entity_id {InvalidEntityId};
};
