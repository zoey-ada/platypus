#include "orthographicCameraNode.hpp"

#include "../scene.hpp"

namespace platypus
{

OrthographicCameraNode::OrthographicCameraNode(OrthographicCameraNodeData* data)
	: CameraNode(data->base_node_data),
	  _dimensions(data->dimensions),
	  _near_z(data->near_z),
	  _far_z(data->far_z),
	  _target(data->target)
{}

OrthographicCameraNode::OrthographicCameraNode(OrthographicCameraNodeData data)
	: CameraNode(data.base_node_data),
	  _dimensions(std::move(data.dimensions)),
	  _near_z(std::move(data.near_z)),
	  _far_z(std::move(data.far_z)),
	  _target(std::move(data.target))
{}

bool OrthographicCameraNode::initialize(const std::shared_ptr<Scene>& /*scene*/)
{
	// this->_projection = matrixOrthographicOffCenterLH(0.0f, 1280.0f, 0.0f, 720.0f, 0.0f,
	// 100.0f);
	this->_projection = matrixOrthographicLH(this->_dimensions.width, this->_dimensions.height,
		this->_near_z, this->_far_z);

	return true;
}

bool OrthographicCameraNode::reinitialize(const std::shared_ptr<Scene>& /*scene*/)
{
	return true;
}

void OrthographicCameraNode::deinitialize(const std::shared_ptr<Scene>& /*scene*/)
{}

bool OrthographicCameraNode::render(const std::shared_ptr<Scene>& /* scene */)
{
	return true;
}

bool OrthographicCameraNode::isVisible(const std::shared_ptr<Scene>& /* scene */) const
{
	return true;
}

bool OrthographicCameraNode::setViewTransform(const std::shared_ptr<Scene>& /* scene */)
{
	// If there is a target, make sure to keep it in view
	auto target = this->_target.lock();

	if (target != nullptr)
	{
		auto pos = target->properties()->toWorld().getPosition();
		Mat4x4 cam_pos(this->properties()->toWorld());
		cam_pos.setPosition({pos.x, pos.y, 0});
		this->setTransform(&cam_pos);
	}

	return true;
}

Mat4x4 OrthographicCameraNode::getWorldViewProjection(const std::shared_ptr<Scene>& scene) const
{
	auto world = scene->getTopMatrix();
	auto view = this->properties()->fromWorld();
	auto world_view = world * view;
	return world_view * this->_projection;
}

}
