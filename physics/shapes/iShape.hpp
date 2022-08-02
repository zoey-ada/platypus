#pragma once

class Collision;
class CircleShape;
class RectangleShape;
class RigidBodyObject;

class IShape
{
public:
	virtual ~IShape() = default;

	virtual float calculateMomentOfInertia(const float mass) const = 0;

	virtual Collision* testCollision(const IShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const = 0;

	virtual Collision* testCollision(const RectangleShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const = 0;

	virtual Collision* testCollision(const CircleShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const = 0;
};
