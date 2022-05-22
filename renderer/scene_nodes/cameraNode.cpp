#include "cameraNode.hpp"

#include "../scene.hpp"

CameraNode::CameraNode(const std::shared_ptr<Mat4x4>& /*to*/, const Frustum& frustum)
	: SceneNode("camera", InvalidEntityId, RenderPass::First),
	  _frustum(frustum),
	  //   _active(true),
	  _debug_camera(false),
	  _target(nullptr),
	  _camera_offset_vector(0.0f, 1.0f, -10.0f, 0.0f)
{
	this->_frustum.setAspect(static_cast<float>(1920) / 1080);
	this->_projection =
		matrixPerspectiveFovLH(this->_frustum.getFieldOfView(), this->_frustum.getAspect(),
			this->_frustum.getNearDistance(), this->_frustum.getFarDistance());
	// this->_ortho = matrixOrthographicOffCenterLH(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	//  this->_ortho = XMMatrixOrthographicLH(1280.0f, 720.0f, 0.0f, 1.0f);

	// return true;
}

bool CameraNode::render(const std::shared_ptr<Scene>& /* scene */)
{
	if (this->_debug_camera)
		this->_frustum.render();

	return true;
}

bool CameraNode::isVisible(const std::shared_ptr<Scene>& /* scene */) const
{
	return true;
}

bool CameraNode::setViewTransform(const std::shared_ptr<Scene>& /* scene */)
{
	// If there is a target, make sure to keep it in view
	if (this->_target)
	{
		auto matrix = this->properties()->toWorld();
		auto& at = this->_camera_offset_vector;
		auto at_world = matrix.transform(at);
		Vec3 position = matrix.getPosition() + static_cast<Vec3>(at_world);
		matrix.setPosition(position);
		this->setTransform(std::make_shared<Mat4x4>(matrix), nullptr);
	}

	return true;
}

Mat4x4 CameraNode::getWorldViewProjection(const std::shared_ptr<Scene>& scene) const
{
	auto world = scene->getTopMatrix();
	auto& view = this->properties()->fromWorld();
	auto world_view = (*world) * view;
	return world_view * this->_projection;
}
