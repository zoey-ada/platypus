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
	auto transform = entity->getComponent<TransformComponent3d>("transform_component_3d").lock();
	this->_position = transform->getPosition();
}

RigidBodyObject::RigidBodyObject(std::shared_ptr<Entity> entity, Vec2 max_velocity, Vec2 friction,
	float mass, float restitution, bool is_solid)
	: _entity(entity),
	  _max_linear_velocity(max_velocity),
	  _friction(friction),
	  _restitution(restitution),
	  _is_solid(is_solid)
{
	this->setMass(mass);
	auto transform = entity->getComponent<TransformComponent2d>("transform_component_2d").lock();
	auto position = transform->getPosition();
	this->_position = Vec3(position.x, position.y, transform->getDepth());
}

RigidBodyObject::~RigidBodyObject()
{
	if (this->_shape != nullptr)
		delete this->_shape;
	this->_shape = nullptr;
}

void RigidBodyObject::update(const Milliseconds delta)
{
	this->caluculateNetForce(delta);
	this->step(delta / 1000.0f);
	this->updateEntityTransform();
	// logInfo("x: " + std::to_string(this->_position.x) + ", y: " +
	// std::to_string(this->_position.y), 	"physics");
}

void RigidBodyObject::caluculateNetForce(const Milliseconds& delta)
{
	Vec3 net_force(0.0f, 0.0f, 0.0f);
	std::list<ForceId> expired_forces;

	for (auto [id, force] : this->_forces)
	{
		net_force += force.vector;

		if (delta >= force.timeout)
		{
			expired_forces.push_back(id);
		}
		else if (force.timeout != Infinity)
		{
			force.timeout -= delta;
		}
	}

	for (const auto id : expired_forces)
		this->_forces.erase(id);

	this->_net_linear_force = net_force;
}

void RigidBodyObject::applyLinearForce(const float& seconds_delta)
{
	auto linear_acceleration = this->_net_linear_force * this->getInverseMass();
	this->_linear_velocity += linear_acceleration * seconds_delta;
	this->_linear_velocity = dampenForce(this->_linear_velocity, (this->_friction * seconds_delta));
	this->_linear_velocity =
		clampForceMagnitude(this->_linear_velocity, this->_max_linear_velocity);

	this->_position += this->_linear_velocity * seconds_delta;
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
}

void RigidBodyObject::step(const float& seconds_delta)
{
	this->applyLinearForce(seconds_delta);
	this->applyAngularForce(seconds_delta);
}

void RigidBodyObject::updateEntityTransform() const
{
	auto transform =
		this->_entity.lock()->getComponent<TransformComponent2d>("transform_component_2d").lock();
	transform->setPosition(this->_position.x, this->_position.y);
	transform->setDepth(this->_position.z);
}

ForceId RigidBodyObject::applyForce(Force force)
{
	this->_last_force_id++;
	this->_forces[this->_last_force_id] = force;
	return this->_last_force_id;
}

void RigidBodyObject::removeForce(ForceId id)
{
	if (this->_forces.contains(id))
		this->_forces.erase(id);
}

void RigidBodyObject::setShape(IShape* shape)
{
	if (this->_shape != nullptr)
		delete this->_shape;

	this->_shape = shape;
}
