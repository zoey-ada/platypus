#include "physicsSystem.hpp"

#include <platypus_proto/entity.hpp>

#include <events/events/newPhysicsComponentEvent.hpp>
#include <events/events/removePhysicsComponentEvent.hpp>
#include <events/iEventManager.hpp>
#include <serviceProvider.hpp>

#include "shapes/collisions.hpp"
#include "shapes/iShape.hpp"

bool PhysicsSystem::initialize()
{
	this->registerEventSinks();
	return true;
}

void PhysicsSystem::deinitialize()
{
	this->_objects.clear();
}

void PhysicsSystem::update(const Milliseconds delta)
{
	// either we were debugging, or we are dropping some serious frames
	if (delta > 500)
		return;

	for (auto [id, object] : this->_objects)
	{
		if (this->_gravity != Vec3())
		{
			object->applyForce(this->_gravity);
		}

		object->step(delta);
	}

	auto collisions = this->detectCollisions();
	this->resolveCollisions(collisions);
}

void PhysicsSystem::addRigidObject(RigidBodyObject* object)
{
	this->_objects[object->entityId()] = object;
}

void PhysicsSystem::removeRigidObject(const EntityId entity_id)
{
	if (this->_objects.contains(entity_id))
	{
		this->_objects.erase(entity_id);
	}
}

std::vector<Collision*> PhysicsSystem::detectCollisions()
{
	std::vector<Collision*> collisions;

	for (auto [obj_a_id, obj_a] : this->_objects)
	{
		for (auto [obj_b_id, obj_b] : this->_objects)
		{
			if (obj_a == obj_b)
				break;

			auto* collision = obj_a->getShape()->testCollision(obj_b->getShape(), obj_a, obj_b);
			if (collision != nullptr)
			{
				collisions.push_back(collision);
			}
		}
	}

	return collisions;
}

void PhysicsSystem::resolveCollisions(std::vector<Collision*> collisions)
{
	for (auto collision : collisions)
	{
		collision->resolve();
		delete collision;
		collision = nullptr;
	}

	collisions.clear();
}

void PhysicsSystem::registerEventSinks()
{
	registerEventSink("new-physics-component-event", PhysicsSystem::onNewPhysicsComponent);
	registerEventSink("remove-physics-component-event", PhysicsSystem::onRemovePhysicsComponent);
}

void PhysicsSystem::onNewPhysicsComponent(std::shared_ptr<IEvent> event)
{
	auto physics_event = std::dynamic_pointer_cast<NewPhysicsComponentEvent>(event);
	this->addRigidObject(physics_event->_rigid_object);
}

void PhysicsSystem::onRemovePhysicsComponent(std::shared_ptr<IEvent> event)
{
	auto physics_event = std::dynamic_pointer_cast<RemovePhysicsComponentEvent>(event);
	this->removeRigidObject(physics_event->_entity_id);
}
