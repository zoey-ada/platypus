#include "rectangleShape.hpp"

#include "../rigidBodyObject.hpp"
#include "collisions.hpp"

RectangleShape::RectangleShape(const platypus::Rectangle& rectangle_data)
	: _offset(Vec2(rectangle_data.center().x(), rectangle_data.center().y())),
	  _height(rectangle_data.dimensions().height()),
	  _width(rectangle_data.dimensions().width())
{}

RectangleShapeData RectangleShape::getShapeData(const Vec2& position) const
{
	RectangleShapeData shape_data;

	Vec2 physics_position = position + this->_offset;

	// TODO: remove this after updating rendering dimensions to use center instead of top left
	float x_mid = physics_position.x + (this->_width / 2);
	float y_mid = physics_position.y + (this->_height / 2);
	shape_data.center = Vec2(x_mid, y_mid);

	shape_data.left = physics_position.x;
	shape_data.right = physics_position.x + this->_width;
	shape_data.top = physics_position.y;
	shape_data.bottom = physics_position.y + this->_height;
	shape_data.width = this->_width;
	shape_data.height = this->_height;

	return shape_data;
}

Collision* RectangleShape::testCollision(const IShape* other, RigidBodyObject* obj_a,
	RigidBodyObject* obj_b) const
{
	return other->testCollision(this, obj_b, obj_a);
}

Collision* RectangleShape::testCollision(const RectangleShape* other, RigidBodyObject* obj_a,
	RigidBodyObject* obj_b) const
{
	auto position_a = obj_a->getPosition();
	auto a_data = this->getShapeData(position_a);

	auto position_b = obj_b->getPosition();
	auto b_data = other->getShapeData(position_b);

	if (a_data.left > b_data.right || b_data.left > a_data.right)
		return nullptr;
	if (a_data.top > b_data.bottom || b_data.top > a_data.bottom)
		return nullptr;

	auto translation = b_data.center - a_data.center;
	float overlap_x = ((a_data.width + b_data.width) / 2) - abs(translation.x);
	float overlap_y = ((a_data.height + b_data.height) / 2) - abs(translation.y);

	if (overlap_x == 0.0f || overlap_y == 0.0f)
		return nullptr;

	// If one object has fully consumed a side of the other, the overlap value no longer has meaning
	if (overlap_x > a_data.width || overlap_x > b_data.width)
		overlap_x = 0;

	if (overlap_y > a_data.height || overlap_y > b_data.height)
		overlap_y = 0;

	return new (std::nothrow) RectangleRectangleCollision(obj_a, obj_b, overlap_x, overlap_y);
}

IShape* createRectangleShape(const platypus::Rectangle& rectangle_data)
{
	return new (std::nothrow) RectangleShape(rectangle_data);
}
