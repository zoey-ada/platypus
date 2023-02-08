#include "rigidBodyObject.hpp"

#include <numeric>

#include <entities/components/transformComponent2d.hpp>
#include <entities/components/transformComponent3d.hpp>
#include <utilities/logging/logger.hpp>

#include "utils.hpp"

RigidBodyObject::RigidBodyObject(std::shared_ptr<Entity> entity, Vec3 max_velocity, Vec3 friction,
	float mass, float restitution, bool is_solid)
	: _entity(entity),
	  _max_linear_velocity(max_velocity),
	  _friction(friction),
	  _mass(mass),
	  _restitution(restitution),
	  _is_solid(is_solid)
{
	this->setMass(mass);

	auto transform_comp_3d =
		entity->getComponent<TransformComponent3d>("transform_component_3d").lock();

	if (transform_comp_3d != nullptr)
	{
		this->_position = transform_comp_3d->getPosition();
	}
	else
	{
		auto transform_comp_2d =
			entity->getComponent<TransformComponent2d>("transform_component_2d").lock();

		if (transform_comp_2d != nullptr)
		{
			auto position = transform_comp_2d->getPosition();
			this->_position = Vec3(position.x, position.y, transform_comp_2d->getDepth());
		}
	}
}

RigidBodyObject::~RigidBodyObject()
{
	if (this->_shape != nullptr)
		delete this->_shape;
	this->_shape = nullptr;
}

void RigidBodyObject::step(const Milliseconds delta)
{
	const float seconds_delta = static_cast<float>(delta) / milliseconds_in_second;
	this->applyLinearForce(seconds_delta);
	this->applyAngularForce(seconds_delta);
	// logInfo("vel x: " + std::to_string(this->_linear_velocity.x) +
	// 		", vel y: " + std::to_string(this->_linear_velocity.y),
	// 	"physics");

	this->updateEntityTransform();
}

void RigidBodyObject::setShape(IShape* shape)
{
	if (this->_shape != nullptr)
		delete this->_shape;

	this->_shape = shape;
}

void RigidBodyObject::applyLinearForce(const float& seconds_delta)
{
	auto linear_acceleration = this->_net_linear_force * this->getInverseMass();

	this->_linear_velocity += linear_acceleration * seconds_delta;
	this->_linear_velocity = dampenForce(this->_linear_velocity, (this->_friction * seconds_delta));
	this->_linear_velocity =
		clampForceMagnitude(this->_linear_velocity, this->_max_linear_velocity);

	this->_position += this->_linear_velocity * seconds_delta;
	this->_net_linear_force = Vec3();
}

void RigidBodyObject::applyAngularForce(const float& seconds_delta)
{
	auto angular_acceleration =
		this->_net_torque / this->_shape->calculateMomentOfInertia(this->_mass);

	this->_angular_velocity += angular_acceleration * seconds_delta;
	this->_angular_velocity = dampenValue(this->_angular_velocity, this->_angular_friction);
	this->_angular_velocity =
		clampValueMagnitude(this->_angular_velocity, this->_max_angular_velocity);

	this->_angle += this->_angular_velocity * seconds_delta;
	this->_net_torque = 0.0f;
}

void RigidBodyObject::updateEntityTransform() const
{
	auto transform =
		this->_entity.lock()->getComponent<TransformComponent2d>("transform_component_2d").lock();
	transform->setPosition(this->_position.x, this->_position.y);
	transform->setDepth(this->_position.z);
}
