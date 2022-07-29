#include "shapeFactory.hpp"

#include <platypus_proto/common.hpp>
#include <platypus_proto/entity.hpp>

#include "rectangleShape.hpp"

namespace
{

IShape* create2dShape(const platypus::Shape_2d& shape_data)
{
	if (shape_data.has_rectangle_data())
	{
		return createRectangleShape(shape_data.rectangle_data());
	}

	return nullptr;
}

IShape* create3dShape(const platypus::Shape_3d& /*shape_data*/)
{
	return nullptr;
}
}

IShape* ShapeFactory::createShape(const std::shared_ptr<platypus::PhysicsComponent> physics_data)
{
	if (physics_data->has_shape_2d())
	{
		return create2dShape(physics_data->shape_2d());
	}
	else if (physics_data->has_shape_3d())
	{
		return create3dShape(physics_data->shape_3d());
	}

	return nullptr;
}
