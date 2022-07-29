#include "collisions.hpp"

#include "../rigidBodyObject.hpp"

const float difference_threshold = 0.000001f;

Collision::Collision(RigidBodyObject* obj_a, RigidBodyObject* obj_b): _obj_a(obj_a), _obj_b(obj_b)
{
	float total_mass = this->_obj_a->getMass() + this->_obj_b->getMass();
	this->_ratio_a = this->_obj_a->getMass() / total_mass;
	this->_ratio_b = this->_obj_b->getMass() / total_mass;
}

RectangleRectangleCollision::RectangleRectangleCollision(RigidBodyObject* obj_a,
	RigidBodyObject* obj_b, float overlap_x, float overlap_y)
	: Collision(obj_a, obj_b), _overlap_x(overlap_x), _overlap_y(overlap_y)
{}

void RectangleRectangleCollision::resolveOverlap()
{
	// only move things if they are overlaping by at least the difference_threshold
	if (this->_overlap_x > this->_overlap_y && this->_overlap_y != 0.0f)
	{
		// smaller y overlap
		float adjustment_a = this->_ratio_a * this->_overlap_y;

		if (this->_obj_a->getVelocity().y > 0)
			adjustment_a *= -1;

		this->_obj_a->adjustPosition(Vec3(0, adjustment_a));

		float adjustment_b = this->_ratio_b * this->_overlap_y;

		if (this->_obj_b->getVelocity().y > 0)
			adjustment_b *= -1;

		this->_obj_b->adjustPosition(Vec3(0, adjustment_b));
	}
	else
	{
		// smaller x overlap
		float adjustment_a = this->_ratio_a * this->_overlap_x;

		if (this->_obj_a->getVelocity().x > 0)
			adjustment_a *= -1;

		this->_obj_a->adjustPosition(Vec3(adjustment_a));

		float adjustment_b = this->_ratio_b * this->_overlap_x;

		if (this->_obj_b->getVelocity().x > 0)
			adjustment_b *= -1;

		this->_obj_b->adjustPosition(Vec3(adjustment_b));
	}
}

void RectangleRectangleCollision::resolveForces()
{
	// reactionary forces
	auto velocity_a = this->_obj_a->getVelocity();
	auto velocity_b = this->_obj_b->getVelocity();

	if (this->_overlap_x > this->_overlap_y && this->_overlap_y != 0.0f)
	{
		velocity_a.y = 0.0f;
		velocity_b.y = 0.0f;
	}
	else
	{
		velocity_a.x = 0.0f;
		velocity_b.x = 0.0f;
	}

	this->_obj_a->setVelocity(velocity_a);
	this->_obj_b->setVelocity(velocity_b);
}

bool RectangleRectangleCollision::passesThreshold()
{
	return abs(this->_overlap_x) > difference_threshold ||
		abs(this->_overlap_y) > difference_threshold;
}

void RectangleRectangleCollision::resolve()
{
	if (this->passesThreshold())
	{
		this->resolveOverlap();
		this->resolveForces();
	}
	// this->obj_a->applyForce({InvalidForceId, adjustment * ratio_a, 0});
	// this->obj_b->applyForce({InvalidForceId, adjustment * ratio_b * -1, 0});
};
