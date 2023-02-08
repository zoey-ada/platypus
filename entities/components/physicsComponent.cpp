#include "physicsComponent.hpp"

#include <platypus_proto/entity.hpp>

#include <events/events/newPhysicsComponentEvent.hpp>
#include <events/events/removePhysicsComponentEvent.hpp>
#include <events/iEventManager.hpp>
#include <physics/rigidBodyObject.hpp>
#include <physics/shapes/iShape.hpp>
#include <physics/shapes/shapeFactory.hpp>
#include <serviceProvider.hpp>

#include "transformComponent2d.hpp"
#include "transformComponent3d.hpp"

PhysicsComponent::~PhysicsComponent()
{
	auto event = std::make_shared<RemovePhysicsComponentEvent>(this->_owner->getId());
	ServiceProvider::getEventManager()->triggerEvent(event);

	if (this->_physics_object != nullptr)
	{
		delete this->_physics_object;
		this->_physics_object = nullptr;
	}
}

bool PhysicsComponent::initialize(const std::shared_ptr<Message>& data)
{
	this->_physics_data = std::dynamic_pointer_cast<platypus::PhysicsComponent>(data);
	return this->_physics_data != nullptr;
}

void PhysicsComponent::postInitialize()
{
	auto event = std::make_shared<NewPhysicsComponentEvent>(this->getPhysicsObject());
	ServiceProvider::getEventManager()->triggerEvent(event);
}

void PhysicsComponent::update(const Milliseconds /*delta*/)
{}

RigidBodyObject* PhysicsComponent::getPhysicsObject()
{
	if (this->_physics_object != nullptr)
	{
		return this->_physics_object;
	}

	this->_physics_object =
		new (std::nothrow) RigidBodyObject(this->_owner, Vec3(this->_physics_data->max_velocity()),
			Vec3(this->_physics_data->friction()), this->_physics_data->mass(),
			this->_physics_data->restitution(), this->_physics_data->is_solid());

	this->_physics_object->setShape(ShapeFactory::createShape(this->_physics_data));
	return this->_physics_object;
}

std::shared_ptr<EntityComponent> createPhysicsComponent()
{
	return std::make_shared<PhysicsComponent>();
}
