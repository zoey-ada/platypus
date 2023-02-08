#pragma once

#include <entities/entity.hpp>
#include <utilities/math/mathTypes.hpp>
#include <utilities/time/utils.hpp>

struct Object
{
	Vec3 position;
	Vec3 velocity;
	Vec3 force;
	float mass;
};

class RigidBodyObject;

class IPhysicsSystem
{
public:
	virtual ~IPhysicsSystem() = default;

	virtual bool initialize() = 0;
	virtual void deinitialize() = 0;

	virtual void update(const Milliseconds delta) = 0;

	virtual void addRigidObject(RigidBodyObject* object) = 0;
	virtual void removeRigidObject(const EntityId entity_id) = 0;
};
