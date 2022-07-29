#pragma once

#include <map>

#include "iPhysicsSystem.hpp"
#include "rigidBodyObject.hpp"

class Collision;
class IEvent;

class PhysicsSystem: public IPhysicsSystem
{
public:
	PhysicsSystem() = default;
	virtual ~PhysicsSystem() = default;

	bool initialize() override;
	void deinitialize() override;

	void update(const Milliseconds delta) override;

	void addRigidObject(RigidBodyObject* object) override;
	void removeRigidObject(const EntityId entity_id) override;

	std::vector<Collision*> detectCollisions();
	void resolveCollisions(std::vector<Collision*> collisions);

	void registerEventSinks();
	void onNewPhysicsComponent(std::shared_ptr<IEvent> event);
	void onRemovePhysicsComponent(std::shared_ptr<IEvent> event);

private:
	std::map<EntityId, RigidBodyObject*> _objects;

	Vec3 _gravity {0, -9.81f, 0};
};
