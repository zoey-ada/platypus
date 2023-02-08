#pragma once

#include <memory>

class IShape;

namespace platypus
{
class PhysicsComponent;
}

class ShapeFactory
{
public:
	static IShape* createShape(const std::shared_ptr<platypus::PhysicsComponent> physics_data);
};
