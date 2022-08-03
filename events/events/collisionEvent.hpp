#pragma once

#include <memory>

#include "iEvent.hpp"

class Entity;

class CollisionEvent: public IEvent
{
public:
	explicit CollisionEvent(std::weak_ptr<Entity> entity_a, std::weak_ptr<Entity> entity_b)
		: _entity_a(entity_a), _entity_b(entity_b)
	{}
	virtual ~CollisionEvent() = default;

	const char* getType() override { return "collision-event"; }

	std::weak_ptr<Entity> _entity_a;
	std::weak_ptr<Entity> _entity_b;
};
