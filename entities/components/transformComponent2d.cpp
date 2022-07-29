#include "transformComponent2d.hpp"

#include <platypus_proto/entity.hpp>

#include <events/events/inputEvent.hpp>
#include <events/iEventManager.hpp>
#include <serviceProvider.hpp>

bool TransformComponent2d::initialize(const std::shared_ptr<Message>& data)
{
	this->_transform_data = std::dynamic_pointer_cast<platypus::TransformComponent_2d>(data);
	this->updateTransform();
	return (this->_transform_data != nullptr);
}

void TransformComponent2d::postInitialize()
{}

void TransformComponent2d::update(const Milliseconds /*delta*/)
{}

Vec2 TransformComponent2d::getPosition() const
{
	return Vec2(this->_transform_data->position_x(), this->_transform_data->position_y());
}

float TransformComponent2d::getDepth() const
{
	return this->_transform_data->depth();
}

void TransformComponent2d::setPosition(const Vec2& new_position)
{
	this->_transform_data->set_position_x(new_position.x);
	this->_transform_data->set_position_y(new_position.y);
	this->updateTransform();
}

void TransformComponent2d::setPosition(const float x, const float y)
{
	this->_transform_data->set_position_x(x);
	this->_transform_data->set_position_y(y);
	this->updateTransform();
}

void TransformComponent2d::setDepth(const float z)
{
	this->_transform_data->set_depth(z);
	this->updateTransform();
}

Vec2 TransformComponent2d::getScale() const
{
	return Vec2(this->_transform_data->scale_x(), this->_transform_data->scale_y());
}

float TransformComponent2d::getWidth() const
{
	return this->_transform_data->scale_x();
}

float TransformComponent2d::getHeight() const
{
	return this->_transform_data->scale_y();
}

void TransformComponent2d::setScale(const Vec2& new_scale)
{
	this->_transform_data->set_scale_x(new_scale.x);
	this->_transform_data->set_scale_y(new_scale.y);
	this->updateTransform();
}

void TransformComponent2d::setScale(const float width, const float height)
{
	this->_transform_data->set_scale_x(width);
	this->_transform_data->set_scale_y(height);
	this->updateTransform();
}

void TransformComponent2d::setWidth(const float width)
{
	this->_transform_data->set_scale_x(width);
	this->updateTransform();
}

void TransformComponent2d::setHeight(const float height)
{
	this->_transform_data->set_scale_y(height);
	this->updateTransform();
}

float TransformComponent2d::getRotationClockwise() const
{
	return this->_transform_data->rotation_clockwise();
}

void TransformComponent2d::setRotationClockwise(float radians)
{
	this->_transform_data->set_rotation_clockwise(radians);
	this->updateTransform();
}

void TransformComponent2d::updateTransform()
{
	Mat4x4 new_transform;

	new_transform.scale(
		Vec3 {this->_transform_data->scale_x(), this->_transform_data->scale_y(), 1.0f});
	new_transform.rotateZ(this->_transform_data->rotation_clockwise());
	new_transform.setPosition(Vec3 {this->_transform_data->position_x(),
		this->_transform_data->position_y(), this->_transform_data->depth()});

	this->_transform = std::move(new_transform);
}

std::shared_ptr<EntityComponent> createTransformComponent2d()
{
	return std::make_shared<TransformComponent2d>();
}
