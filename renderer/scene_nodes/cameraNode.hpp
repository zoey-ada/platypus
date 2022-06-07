#pragma once

#include <utilities/math/mathTypes.hpp>

#include "sceneNode.hpp"

class CameraNode: public SceneNode
{
public:
	explicit CameraNode(const Mat4x4* to, const Frustum& frustum);
	virtual ~CameraNode() = default;

	[[nodiscard]] bool render(const std::shared_ptr<Scene>& scene) override;
	[[nodiscard]] bool isVisible(const std::shared_ptr<Scene>& scene) const override;

	[[nodiscard]] bool setViewTransform(const std::shared_ptr<Scene>& scene);

	[[nodiscard]] const Frustum& getFrustum() const { return this->_frustum; }

	void setTarget(const std::shared_ptr<SceneNode>& target) { this->_target = target; }
	void clearTarget() { this->_target = nullptr; }
	[[nodiscard]] std::shared_ptr<SceneNode> getTarget() const { return this->_target; }

	[[nodiscard]] Mat4x4 getWorldViewProjection(const std::shared_ptr<Scene>& scene) const;

	[[nodiscard]] Mat4x4 getProjection() const { return this->_projection; }
	[[nodiscard]] Mat4x4 getOrtho() const { return this->_ortho; }
	[[nodiscard]] Mat4x4 getView() const { return this->_view; }

	void setCameraOffset(const Vec4& camera_offset) { this->_camera_offset_vector = camera_offset; }

private:
	Frustum _frustum;
	Mat4x4 _projection;
	Mat4x4 _ortho;
	Mat4x4 _view;
	// bool _active;
	bool _debug_camera;
	std::shared_ptr<SceneNode> _target;
	Vec4 _camera_offset_vector;

	PtSceneNodeData getSceneNodeData()
	{
		PtSceneNodeData node_data {};
		node_data.name = "camera";
		node_data.render_pass = RenderPass::First;
		return node_data;
	}
};
