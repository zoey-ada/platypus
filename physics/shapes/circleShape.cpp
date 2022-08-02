#include "circleShape.hpp"

#include "../rigidBodyObject.hpp"
#include "collisions.hpp"

CircleShape::CircleShape(const platypus::Circle& circle_data)
	: _offset(Vec2(circle_data.center().x(), circle_data.center().y())),
	  _radius(circle_data.radius())
{}

CircleShapeData CircleShape::getShapeData(const Vec2& position) const
{
	CircleShapeData shape_data;

	Vec2 physics_position = position + this->_offset;

	// TODO: remove this after updating rendering dimensions to use center instead of top left
	float x_mid = physics_position.x + this->_radius;
	float y_mid = physics_position.y + this->_radius;
	shape_data.center = Vec2(x_mid, y_mid);

	shape_data.radius = this->_radius;

	return shape_data;
}

Collision* CircleShape::testCollision(const IShape* other, RigidBodyObject* obj_a,
	RigidBodyObject* obj_b) const
{
	return other->testCollision(this, obj_b, obj_a);
}

Collision* CircleShape::testCollision(const RectangleShape* /*other*/, RigidBodyObject* /*obj_a*/,
	RigidBodyObject* /*obj_b*/) const
{
	return nullptr;
}

Collision* CircleShape::testCollision(const CircleShape* other, RigidBodyObject* obj_a,
	RigidBodyObject* obj_b) const
{
	auto position_a = obj_a->getPosition();
	auto a_data = this->getShapeData(position_a);

	auto position_b = obj_b->getPosition();
	auto b_data = other->getShapeData(position_b);

	auto translation = Vec3(b_data.center) - Vec3(a_data.center);
	float translation_distance_squared =
		translation.x * translation.x + translation.y * translation.y;

	float radius_sum = this->_radius + other->_radius;
	float radius_sum_squared = radius_sum * radius_sum;

	if (translation_distance_squared > radius_sum_squared)
	{
		return nullptr;
	}

	Manifold manifold {};
	manifold.obj_a = obj_a;
	manifold.obj_b = obj_b;

	if (translation_distance_squared == 0)
	{
		// centers are in the same location
		manifold.penetration = std::min(this->_radius, other->_radius);
		manifold.normal = Vec3(1.0f, 0.0f, 0.0f);  // could really be in any direction
	}
	else
	{
		float translation_distance = sqrt(translation_distance_squared);
		manifold.penetration = radius_sum - translation_distance;
		manifold.normal = translation / translation_distance;
	}

	auto collision = new (std::nothrow) Collision(manifold);
	return collision;
}

IShape* createCircleShape(const platypus::Circle& circle_data)
{
	return new (std::nothrow) CircleShape(circle_data);
}
