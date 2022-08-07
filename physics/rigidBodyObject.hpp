#pragma once

#include <map>

#include <entities/entity.hpp>
#include <utilities/logging/logger.hpp>
#include <utilities/math/mathTypes.hpp>

#include "shapes/iShape.hpp"

class RigidBodyObject
{
public:
	RigidBodyObject(std::shared_ptr<Entity> entity, Vec3 max_velocity, Vec3 friction, float mass,
		float restitution, bool is_solid);

	virtual ~RigidBodyObject();

	void step(const Milliseconds delta);

	EntityId entityId() const { return this->_entity.lock()->getId(); }
	std::weak_ptr<Entity> getEntity() const { return this->_entity; }

	void applyForce(const Vec3& force)
	{
		this->_net_linear_force += force;
		// logInfo("add force: x: " + std::to_string(force.x) + ", y: " + std::to_string(force.y),
		// 	"physics");
	};
	void removeAllLinearForce() { this->_net_linear_force = Vec3(); }

	void applyTorque(const float torque) { this->_net_torque += torque; };
	void removeAllTorque() { this->_net_torque = 0.0f; }

	void setFriction(Vec3 friction) { this->_friction = friction; }
	void setFriction(Vec2 friction) { this->_friction = Vec3(friction.x, friction.y, 0.0f); }

	void setShape(IShape* shape);
	IShape* getShape() const { return this->_shape; }

	Vec3 getPosition() const { return this->_position; }
	void adjustPosition(Vec3 adjustment) { this->_position = this->_position + adjustment; }

	Vec3 getLinearVelocity() const { return this->_linear_velocity; }
	void setLinearVelocity(const Vec3 velocity) { this->_linear_velocity = velocity; }

	float getAngularVelocity() const { return this->_angular_velocity; }
	void setAngularVelocity(const float velocity) { this->_angular_velocity = velocity; }

	Vec3 getMaxLinearVelocity() const { return this->_max_linear_velocity; }
	void setMaxLinearVelocity(const Vec3 max_velocity)
	{
		this->_max_linear_velocity = max_velocity;
	}

	float getMass() const { return this->_mass; }
	float getInverseMass() const { return this->_inverse_mass; }
	void setMass(const float mass)
	{
		this->_mass = mass;
		this->calculateInverseMass();
	}
	void calculateInverseMass()
	{
		this->_inverse_mass = this->_mass == 0.0f ? 0.0f : 1.0f / this->_mass;
	}

	float getRestitution() const { return this->_restitution; }
	void setRestitution(const float restitution) { this->_restitution = restitution; }

	bool isSolid() const { return this->_is_solid; }
	void setIsSolid(const bool is_solid) { this->_is_solid = is_solid; }

	// void setMaterial(const char* material);

private:
	std::weak_ptr<Entity> _entity;

	IShape* _shape {nullptr};

	Vec3 _net_linear_force {0.0f, 0.0f, 0.0f};
	float _net_torque {0.0f};

	Vec3 _position {0.0f, 0.0f, 0.0f};
	Vec3 _linear_velocity {0.0f, 0.0f, 0.0f};

	// 2d
	float _angle {0.0f};
	float _angular_velocity {0.0f};

	float _mass {0.0f};
	float _inverse_mass {0.0f};
	float _restitution {0.0f};
	float _moment_of_inertia {0.0f};

	Vec3 _max_linear_velocity {0.0f, 0.0f, 0.0f};
	float _max_angular_velocity {0.0f};

	Vec3 _friction {0.0f, 0.0f, 0.0f};
	float _angular_friction {0.0f};
	bool _is_solid {true};

	void applyLinearForce(const float& seconds_delta);
	void applyAngularForce(const float& seconds_delta);
	void updateEntityTransform() const;
};
