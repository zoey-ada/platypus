#include "rigidBodyObject.hpp"

#include <numeric>

#include <entities/components/transformComponent2d.hpp>
#include <entities/components/transformComponent3d.hpp>
#include <utilities/logging/logger.hpp>

#include "utils.hpp"

RigidBodyObject::RigidBodyObject(std::shared_ptr<Entity> entity, Vec3 max_velocity, Vec3 friction,
	float mass)
	: _entity(entity), _max_velocity(max_velocity), _friction(friction), _mass(mass)
{
	auto transform = entity->getComponent<TransformComponent3d>("transform_component_3d").lock();
	this->_position = transform->getPosition();
}

RigidBodyObject::RigidBodyObject(std::shared_ptr<Entity> entity, Vec2 max_velocity, Vec2 friction,
	float mass)
	: _entity(entity), _max_velocity(max_velocity), _friction(friction), _mass(mass)
{
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
	auto net_force = this->caluculateNetForce(delta);
	this->step(delta, net_force);
	this->updateEntityTransform();
	// logInfo("x: " + std::to_string(this->_position.x) + ", y: " +
	// std::to_string(this->_position.y), 	"physics");
}

Vec3 RigidBodyObject::caluculateNetForce(const Milliseconds delta)
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

	return net_force;
}

void RigidBodyObject::step(const Milliseconds delta, const Vec3& force)
{
	float delta_factor = delta / 16.6667f;  // based on frame time

	this->_velocity = force * delta_factor + this->_velocity;
	this->_velocity = dampenForce(this->_velocity, this->_friction);
	this->_velocity = clampForceMagnitude(this->_velocity, this->_max_velocity);
	this->_position = this->_velocity * delta_factor + this->_position;
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
