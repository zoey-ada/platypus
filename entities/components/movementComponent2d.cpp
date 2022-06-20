#include "movementComponent2d.hpp"

#include <platypus_proto/entity.hpp>

#include <events/events/inputEvent.hpp>
#include <events/iEventManager.hpp>
#include <serviceProvider.hpp>

bool MovementComponent2d::initialize(const std::shared_ptr<Message>& data)
{
	this->_movement_data = std::dynamic_pointer_cast<platypus::MovementComponent_2d>(data);
	this->registerEventSinks();
	return true;
}

void MovementComponent2d::postInitialize()
{}

void MovementComponent2d::update(const Milliseconds delta)
{
	float delta_factor = delta / 16.6667f;

	if (this->_is_moving_x || this->_velocity_x != 0.0f)
	{
		if (this->_is_moving_x)
		{
			this->_velocity_x += this->_movement_data->acceleration_x() * delta_factor;
		}
		else
		{
			this->_velocity_x += this->_movement_data->deceleration_x() * delta_factor;
		}

		if (this->_velocity_x != 0)
		{
			if (this->_is_moving_x)
			{
				if (this->_movement_data->acceleration_x() > 0)
				{
					this->_velocity_x =
						std::min(this->_velocity_x, this->_movement_data->max_velocity_x());
				}
				else
				{
					this->_velocity_x =
						std::max(this->_velocity_x, -this->_movement_data->max_velocity_x());
				}
			}
			else
			{
				if (this->_movement_data->acceleration_x() > 0)
				{
					this->_velocity_x = std::min(this->_velocity_x, 0.0f);
				}
				else
				{
					this->_velocity_x = std::max(this->_velocity_x, 0.0f);
				}
			}
		}
	}

	if (this->_is_moving_y || this->_velocity_y != 0.0f)
	{
		if (this->_is_moving_y)
		{
			this->_velocity_y += this->_movement_data->acceleration_y() * delta_factor;
		}
		else
		{
			this->_velocity_y += this->_movement_data->deceleration_y() * delta_factor;
		}

		if (this->_velocity_y != 0)
		{
			if (this->_is_moving_y)
			{
				if (this->_movement_data->acceleration_y() > 0)
				{
					this->_velocity_y =
						std::min(this->_velocity_y, this->_movement_data->max_velocity_y());
				}
				else
				{
					this->_velocity_y =
						std::max(this->_velocity_y, -this->_movement_data->max_velocity_y());
				}
			}
			else
			{
				if (this->_movement_data->acceleration_y() > 0)
				{
					this->_velocity_y = std::min(this->_velocity_y, 0.0f);
				}
				else
				{
					this->_velocity_y = std::max(this->_velocity_y, 0.0f);
				}
			}
		}
	}
}

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
		if (input_event->_action == InputActionType::Start)
		{
			this->_movement_data->set_acceleration_y(abs(this->_movement_data->acceleration_y()));
			this->_movement_data->set_deceleration_y(abs(this->_movement_data->deceleration_y()));
			this->_is_moving_y = true;
		}
		else if (input_event->_action == InputActionType::End)
		{
			this->_movement_data->set_acceleration_y(-abs(this->_movement_data->acceleration_y()));
			this->_movement_data->set_deceleration_y(-abs(this->_movement_data->deceleration_y()));
			this->_is_moving_y = false;
		}
	}
	else if (input_event->_command_name.compare("move-down") == 0)
	{
		if (input_event->_action == InputActionType::Start)
		{
			this->_movement_data->set_acceleration_y(-abs(this->_movement_data->acceleration_y()));
			this->_movement_data->set_deceleration_y(-abs(this->_movement_data->deceleration_y()));
			this->_is_moving_y = true;
		}
		else if (input_event->_action == InputActionType::End)
		{
			this->_movement_data->set_acceleration_y(abs(this->_movement_data->acceleration_y()));
			this->_movement_data->set_deceleration_y(abs(this->_movement_data->deceleration_y()));
			this->_is_moving_y = false;
		}
	}
	else if (input_event->_command_name.compare("move-left") == 0)
	{
		if (input_event->_action == InputActionType::Start)
		{
			this->_movement_data->set_acceleration_x(-abs(this->_movement_data->acceleration_x()));
			this->_movement_data->set_deceleration_x(-abs(this->_movement_data->deceleration_x()));
			this->_is_moving_x = true;
		}
		else if (input_event->_action == InputActionType::End)
		{
			this->_movement_data->set_acceleration_x(abs(this->_movement_data->acceleration_x()));
			this->_movement_data->set_deceleration_x(abs(this->_movement_data->deceleration_x()));
			this->_is_moving_x = false;
		}
	}
	else if (input_event->_command_name.compare("move-right") == 0)
	{
		if (input_event->_action == InputActionType::Start)
		{
			this->_movement_data->set_acceleration_x(abs(this->_movement_data->acceleration_x()));
			this->_movement_data->set_deceleration_x(abs(this->_movement_data->deceleration_x()));
			this->_is_moving_x = true;
		}
		else if (input_event->_action == InputActionType::End)
		{
			this->_movement_data->set_acceleration_x(-abs(this->_movement_data->acceleration_x()));
			this->_movement_data->set_deceleration_x(-abs(this->_movement_data->deceleration_x()));
			this->_is_moving_x = false;
		}
	}
}

std::shared_ptr<EntityComponent> createMovementComponent2d()
{
	return std::make_shared<MovementComponent2d>();
}
