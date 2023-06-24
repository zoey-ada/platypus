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

	void pause() override { this->_is_paused = true; }
	void resume() override { this->_is_paused = false; }

	void addRigidObject(RigidBodyObject* object) override;
	void removeRigidObject(const EntityId entity_id) override;

	void setGravity(const Vec3& gravity) { this->_gravity = gravity; }

	std::vector<Collision*> detectCollisions();
	void resolveCollisions(std::vector<Collision*> collisions);

	void registerEventSinks();
	void onNewPhysicsComponent(std::shared_ptr<IEvent> event);
	void onRemovePhysicsComponent(std::shared_ptr<IEvent> event);

private:
	std::map<EntityId, RigidBodyObject*> _objects;

	// Vec3 _gravity {0, -9.81f, 0};
	Vec3 _gravity {0, 0, 0};

	bool _is_paused {false};
};
