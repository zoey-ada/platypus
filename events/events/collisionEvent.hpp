#pragma once

#include <memory>

#include <entities/entity.hpp>

#include "iEvent.hpp"

class CollisionEvent: public IEvent
{
public:
	explicit CollisionEvent(std::weak_ptr<Entity> entity_a, std::weak_ptr<Entity> entity_b)
		: _entity_a(entity_a), _entity_b(entity_b)
	{
		auto a = this->_entity_a.lock();
		if (a != nullptr)
			this->_entity_a_id = a->getId();

		auto b = this->_entity_b.lock();
		if (b != nullptr)
			this->_entity_b_id = b->getId();
	}
	virtual ~CollisionEvent() = default;

	const char* getType() override { return "collision-event"; }

	std::weak_ptr<Entity> _entity_a;
	std::weak_ptr<Entity> _entity_b;

	EntityId _entity_a_id {InvalidEntityId};
	EntityId _entity_b_id {InvalidEntityId};
};

inline bool isCollisionBetween(const CollisionEvent* collision, EntityId a, EntityId b)
{
	if ((collision->_entity_a_id == a && collision->_entity_b_id == b) ||
		(collision->_entity_a_id == b && collision->_entity_b_id == a))
		return true;

	return false;
}

inline bool isCollisionBetween(const std::shared_ptr<CollisionEvent> collision, EntityId a,
	EntityId b)
{
	return isCollisionBetween(collision.get(), a, b);
}
