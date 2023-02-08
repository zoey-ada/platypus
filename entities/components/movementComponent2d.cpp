#include "movementComponent2d.hpp"

#include <platypus_proto/entity.hpp>

#include <events/events/inputEvent.hpp>
#include <events/iEventManager.hpp>
#include <physics/rigidBodyObject.hpp>
#include <serviceProvider.hpp>

#include "physicsComponent.hpp"

bool MovementComponent2d::initialize(const std::shared_ptr<Message>& data)
{
	this->_movement_data = std::dynamic_pointer_cast<platypus::MovementComponent_2d>(data);
	this->registerEventSinks();
	return true;
}

void MovementComponent2d::postInitialize()
{}

void MovementComponent2d::update(const Milliseconds /*delta*/)
{}

Vec2 MovementComponent2d::getAcceleration() const
{
	return Vec2 {this->_movement_data->acceleration_x(), this->_movement_data->acceleration_y()};
}

void MovementComponent2d::setAcceleration(const Vec2& new_acceleration)
{
	this->_movement_data->set_acceleration_x(new_acceleration.x);
	this->_movement_data->set_acceleration_y(new_acceleration.y);
}

float MovementComponent2d::getAccelerationX() const
{
	return this->_movement_data->acceleration_x();
}

void MovementComponent2d::setAccelerationX(const float new_acceleration)
{
	this->_movement_data->set_acceleration_x(new_acceleration);
}

float MovementComponent2d::getAccelerationY() const
{
	return this->_movement_data->acceleration_y();
}

void MovementComponent2d::setAccelerationY(const float new_acceleration)
{
	this->_movement_data->set_acceleration_y(new_acceleration);
}

Vec2 MovementComponent2d::getMaxVelocity() const
{
	return Vec2 {this->_movement_data->max_velocity_x(), this->_movement_data->max_velocity_y()};
}

void MovementComponent2d::setMaxVelocity(const Vec2& new_max_velocity)
{
	this->_movement_data->set_max_velocity_x(new_max_velocity.x);
	this->_movement_data->set_max_velocity_y(new_max_velocity.y);
}

float MovementComponent2d::getMaxVelocityX() const
{
	return this->_movement_data->max_velocity_x();
}

void MovementComponent2d::setMaxVelocityX(const float new_max_velocity)
{
	this->_movement_data->set_max_velocity_x(new_max_velocity);
}

float MovementComponent2d::getMaxVelocityY() const
{
	return this->_movement_data->max_velocity_y();
}

void MovementComponent2d::setMaxVelocityY(const float new_max_velocity)
{
	this->_movement_data->set_max_velocity_y(new_max_velocity);
}
void MovementComponent2d::registerEventSinks()
{
	registerEventSink("input-event", MovementComponent2d::onInput);
}

void MovementComponent2d::onInput(std::shared_ptr<IEvent> event)
{
	auto input_event = dynamic_pointer_cast<InputEvent>(event);
	// if (input_event->_entity_id != this->_owner->getId())
	// {
	// 	return;
	// }

	if (input_event->_command_name.compare("move-up") == 0)
	{
		if (input_event->_action == InputActionType::Start ||
			input_event->_action == InputActionType::Update)
		{
			auto force = Vec3(0.0f, this->_movement_data->acceleration_y(), 0.0f);

			auto physics_component =
				this->_owner->getComponent<PhysicsComponent>("physics_component").lock();
			physics_component->getPhysicsObject()->applyForce(force);
		}
	}
	else if (input_event->_command_name.compare("move-down") == 0)
	{
		if (input_event->_action == InputActionType::Start ||
			input_event->_action == InputActionType::Update)
		{
			auto force = Vec3(0.0f, -this->_movement_data->acceleration_y(), 0.0f);

			auto physics_component =
				this->_owner->getComponent<PhysicsComponent>("physics_component").lock();
			physics_component->getPhysicsObject()->applyForce(force);
		}
	}
	else if (input_event->_command_name.compare("move-left") == 0)
	{
		if (input_event->_action == InputActionType::Start ||
			input_event->_action == InputActionType::Update)
		{
			auto force = Vec3(-this->_movement_data->acceleration_x(), 0.0f, 0.0f);

			auto physics_component =
				this->_owner->getComponent<PhysicsComponent>("physics_component").lock();
			physics_component->getPhysicsObject()->applyForce(force);
		}
	}
	else if (input_event->_command_name.compare("move-right") == 0)
	{
		if (input_event->_action == InputActionType::Start ||
			input_event->_action == InputActionType::Update)
		{
			auto force = Vec3(this->_movement_data->acceleration_x(), 0.0f, 0.0f);

			auto physics_component =
				this->_owner->getComponent<PhysicsComponent>("physics_component").lock();
			physics_component->getPhysicsObject()->applyForce(force);
		}
	}
}

std::shared_ptr<EntityComponent> createMovementComponent2d()
{
	return std::make_shared<MovementComponent2d>();
}
