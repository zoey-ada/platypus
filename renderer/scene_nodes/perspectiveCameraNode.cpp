#include "perspectiveCameraNode.hpp"

#include "../scene.hpp"

namespace platypus
{

PerspectiveCameraNode::PerspectiveCameraNode(PerspectiveCameraNodeData* data)
	: CameraNode(data->base_node_data),
	  _frustum(data->frustum),
	  _camera_offset_vector(data->offset_vector),
	  _target(data->target),
	  _debug_mode(data->_debug_mode)
{}

PerspectiveCameraNode::PerspectiveCameraNode(PerspectiveCameraNodeData data)
	: CameraNode(data.base_node_data),
	  _frustum(std::move(data.frustum)),
	  _camera_offset_vector(std::move(data.offset_vector)),
	  _target(std::move(data.target)),
	  _debug_mode(data._debug_mode)
{}

bool PerspectiveCameraNode::initialize(const std::shared_ptr<Scene>& /*scene*/)
{
	this->_projection =
		matrixPerspectiveFovLH(this->_frustum.getFieldOfView(), this->_frustum.getAspect(),
			this->_frustum.getNearDistance(), this->_frustum.getFarDistance());

	Mat4x4 camera_pos;
	camera_pos.translate(static_cast<Vec3>(this->_camera_offset_vector));
	this->setTransform(&camera_pos);

	return true;
}

bool PerspectiveCameraNode::reinitialize(const std::shared_ptr<Scene>& /*scene*/)
{
	return true;
}

void PerspectiveCameraNode::deinitialize(const std::shared_ptr<Scene>& /*scene*/)
{}

bool PerspectiveCameraNode::render(const std::shared_ptr<Scene>& /* scene */)
{
	if (this->_debug_mode)
		this->_frustum.render();

	return true;
}

bool PerspectiveCameraNode::isVisible(const std::shared_ptr<Scene>& /* scene */) const
{
	return true;
}

bool PerspectiveCameraNode::setViewTransform(const std::shared_ptr<Scene>& /* scene */)
{
	// If there is a target, make sure to keep it in view
	auto target = this->_target.lock();

	if (target != nullptr)
	{
		Mat4x4 target_location(target->properties()->toWorld());
		target_location.translate(static_cast<Vec3>(this->_camera_offset_vector));
		this->setTransform(&target_location);
	}

	return true;
}

Mat4x4 PerspectiveCameraNode::getWorldViewProjection(const std::shared_ptr<Scene>& scene) const
{
	auto world = scene->getTopMatrix();
	auto view = this->properties()->fromWorld();
	auto world_view = world * view;
	return world_view * this->_projection;
}

}
