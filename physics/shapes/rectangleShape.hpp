#pragma once

#include <utilities/math/mathTypes.hpp>

#include "iShape.hpp"

struct RectangleShapeData
{
	Vec2 center;
	float left;
	float right;
	float top;
	float bottom;
	float width;
	float height;
};

class RectangleShape: public IShape
{
public:
	explicit RectangleShape(const platypus::Rectangle& rectangle_data);
	virtual ~RectangleShape() = default;

	RectangleShapeData getShapeData(const Vec2& position) const;

	RectangleShapeData getShapeData(const Vec3& position) const
	{
		return this->getShapeData(Vec2(position.x, position.y));
	}

	Collision* testCollision(const IShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const override;

	Collision* testCollision(const RectangleShape* other, RigidBodyObject* obj_a,
		RigidBodyObject* obj_b) const override;

private:
	Vec2 _offset;
	float _height;
	float _width;
};

IShape* createRectangleShape(const platypus::Rectangle& rectangle_data);
