#pragma once

class Collision;
class RectangleShape;
class RigidBodyObject;

class IShape
{
public:
	virtual ~IShape() = default;

	virtual Collision* testCollision(const IShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const = 0;

	virtual Collision* testCollision(const RectangleShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const = 0;
};
