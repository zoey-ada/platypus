#include "positionComponent.hpp"

#include <platypus_proto/entity.hpp>

bool PositionComponent::initialize(const std::shared_ptr<Message>& data)
{
	this->_position_data = std::dynamic_pointer_cast<platypus::PositionComponent>(data);
	return (this->_position_data != nullptr);
}

void PositionComponent::postInitialize()
{}

Vec3 PositionComponent::getPosition() const
{
	return Vec3(this->_position_data->x(), this->_position_data->y(), this->_position_data->z());
}

void PositionComponent::setPosition(const Vec3& new_position)
{
	this->_position_data->set_x(new_position.x);
	this->_position_data->set_y(new_position.y);
	this->_position_data->set_z(new_position.z);
}

void PositionComponent::setPosition(const float x, const float y, const float z)
{
	this->_position_data->set_x(x);
	this->_position_data->set_y(y);
	this->_position_data->set_z(z);
}

std::shared_ptr<EntityComponent> createPositionComponent()
{
	return std::make_shared<PositionComponent>();
}
