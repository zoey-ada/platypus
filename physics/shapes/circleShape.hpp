#pragma once

#include <utilities/math/mathTypes.hpp>

#include "iShape.hpp"

struct CircleShapeData
{
	Vec2 center;
	float radius;
};

class CircleShape: public IShape
{
public:
	explicit CircleShape(const platypus::Circle& circle_data);
	virtual ~CircleShape() = default;

	CircleShapeData getShapeData(const Vec2& position) const;

	CircleShapeData getShapeData(const Vec3& position) const
	{
		return this->getShapeData(Vec2(position.x, position.y));
	}

	float calculateMomentOfInertia(const float mass) const override
	{
		return this->_moment_of_inertia_without_mass * mass;
	}

	Collision* testCollision(const IShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const override;

	Collision* testCollision(const RectangleShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const override;

	Collision* testCollision(const CircleShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const override;

private:
	Vec2 _offset;
	float _radius;
	float _moment_of_inertia_without_mass;
};

IShape* createCircleShape(const platypus::Circle& circle_data);
