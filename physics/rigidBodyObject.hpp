#pragma once

#include <map>

#include <entities/entity.hpp>
#include <utilities/math/mathTypes.hpp>

#include "force.hpp"
#include "shapes/iShape.hpp"

class RigidBodyObject
{
public:
	RigidBodyObject(std::shared_ptr<Entity> entity, Vec3 max_velocity, Vec3 friction, float mass);
	RigidBodyObject(std::shared_ptr<Entity> entity, Vec2 max_velocity, Vec2 friction, float mass);
	virtual ~RigidBodyObject();

	void update(const Milliseconds delta);

	Vec3 caluculateNetForce(const Milliseconds delta);
	void step(const Milliseconds delta, const Vec3& force);
	void updateEntityTransform() const;

	EntityId entityId() const { return this->_entity.lock()->getId(); }

	ForceId applyForce(Force force);
	void removeForce(ForceId id);
	void removeAllForces() { this->_forces.clear(); }

	void setFriction(Vec3 friction) { this->_friction = friction; }
	void setFriction(Vec2 friction) { this->_friction = Vec3(friction.x, friction.y, 0.0f); }

	void setShape(IShape* shape);
	IShape* getShape() const { return this->_shape; }

	Vec3 getPosition() const { return this->_position; }
	void adjustPosition(Vec3 adjustment) { this->_position = this->_position + adjustment; }

	Vec3 getVelocity() const { return this->_velocity; }
	void setVelocity(const Vec3 velocity) { this->_velocity = velocity; }

	Vec3 getMaxVelocity() const { return this->_max_velocity; }
	void setMaxVelocity(const Vec3 max_velocity) { this->_max_velocity = max_velocity; }

	float getMass() const { return this->_mass; }
	void setMass(const float mass) { this->_mass = mass; }

	// void setMaterial(const char* material);

private:
	std::weak_ptr<Entity> _entity;

	IShape* _shape {nullptr};

	ForceId _last_force_id = InvalidForceId;
	Vec3 _position {0, 0, 0};
	Vec3 _velocity {0, 0, 0};
	std::map<ForceId, Force> _forces;

	Vec3 _max_velocity {0, 0, 0};
	Vec3 _friction {0, 0, 0};
	float _mass {0};
};
