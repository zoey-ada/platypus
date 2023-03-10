#include "transformComponent3d.hpp"

#include <platypus_proto/entity.hpp>

#include <events/events/inputEvent.hpp>
#include <events/iEventManager.hpp>
#include <serviceProvider.hpp>

bool TransformComponent3d::initialize(const std::shared_ptr<Message>& data)
{
	this->_transform_data = std::dynamic_pointer_cast<platypus::TransformComponent_3d>(data);
	this->updateTransform();
	return (this->_transform_data != nullptr);
}

void TransformComponent3d::postInitialize()
{}

Vec3 TransformComponent3d::getPosition() const
{
	return Vec3(this->_transform_data->position_x(), this->_transform_data->position_y(),
		this->_transform_data->position_z());
}

void TransformComponent3d::setPosition(const Vec3& new_position)
{
	this->_transform_data->set_position_x(new_position.x);
	this->_transform_data->set_position_y(new_position.y);
	this->_transform_data->set_position_z(new_position.z);
	this->updateTransform();
}

void TransformComponent3d::setPosition(const float x, const float y, const float z)
{
	this->_transform_data->set_position_x(x);
	this->_transform_data->set_position_y(y);
	this->_transform_data->set_position_z(z);
	this->updateTransform();
}

Vec3 TransformComponent3d::getRotation() const
{
	return Vec3(this->_transform_data->rotation_x(), this->_transform_data->rotation_y(),
		this->_transform_data->rotation_z());
}

void TransformComponent3d::setRotation(const Vec3& new_rotation)
{
	this->_transform_data->set_scale_x(new_rotation.x);
	this->_transform_data->set_scale_y(new_rotation.y);
	this->_transform_data->set_scale_z(new_rotation.z);
	this->updateTransform();
}

void TransformComponent3d::setRotation(const float x, const float y, const float z)
{
	this->_transform_data->set_rotation_x(x);
	this->_transform_data->set_rotation_y(y);
	this->_transform_data->set_rotation_z(z);
	this->updateTransform();
}

Vec3 TransformComponent3d::getScale() const
{
	return Vec3(this->_transform_data->scale_x(), this->_transform_data->scale_y(),
		this->_transform_data->scale_z());
}

void TransformComponent3d::setScale(const Vec3& new_scale)
{
	this->_transform_data->set_scale_x(new_scale.x);
	this->_transform_data->set_scale_y(new_scale.y);
	this->_transform_data->set_scale_z(new_scale.z);
	this->updateTransform();
}

void TransformComponent3d::setScale(const float x, const float y, const float z)
{
	this->_transform_data->set_scale_x(x);
	this->_transform_data->set_scale_y(y);
	this->_transform_data->set_scale_z(z);
	this->updateTransform();
}

void TransformComponent3d::registerEventSinks()
{
	registerEventSink("input-event", TransformComponent3d::onRotateY);
}

void TransformComponent3d::onRotateY(std::shared_ptr<IEvent> /*event*/)
{
	this->_transform_data->set_rotation_y((float)(this->_transform_data->rotation_y() + 0.196349));
	this->updateTransform();
}

void TransformComponent3d::updateTransform()
{
	Mat4x4 new_transform;
	new_transform.scale(this->getScale());
	new_transform.rotateX(this->_transform_data->rotation_x());
	new_transform.rotateY(this->_transform_data->rotation_y());
	new_transform.rotateZ(this->_transform_data->rotation_z());
	new_transform.setPosition(this->getPosition());
	this->_transform = std::move(new_transform);
}

std::shared_ptr<EntityComponent> createTransformComponent3d()
{
	return std::make_shared<TransformComponent3d>();
}
