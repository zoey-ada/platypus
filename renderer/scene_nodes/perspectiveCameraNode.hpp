#pragma once

#include <utilities/common/ptExtent.hpp>
#include <utilities/math/mathTypes.hpp>

#include "cameraNode.hpp"
#include "sceneNode.hpp"

namespace platypus
{

struct PerspectiveCameraNodeData
{
	SceneNodeData* base_node_data;
	Frustum frustum;
	Vec4 offset_vector;
	std::weak_ptr<SceneNode> target;
	bool _debug_mode {false};
};

class PerspectiveCameraNode: public CameraNode
{
public:
	explicit PerspectiveCameraNode(PerspectiveCameraNodeData* data);
	explicit PerspectiveCameraNode(PerspectiveCameraNodeData data);
	virtual ~PerspectiveCameraNode() = default;

	bool initialize(const std::shared_ptr<Scene>& scene) override;
	bool reinitialize(const std::shared_ptr<Scene>& scene) override;
	void deinitialize(const std::shared_ptr<Scene>& scene) override;

	bool render(const std::shared_ptr<Scene>& scene) override;
	bool isVisible(const std::shared_ptr<Scene>& scene) const override;

	bool setViewTransform(const std::shared_ptr<Scene>& scene) override;

	inline const Frustum& getFrustum() const { return this->_frustum; }

	inline void setTarget(const std::shared_ptr<SceneNode>& target) override
	{
		this->_target = target;
	}
	inline void clearTarget() override { this->_target.reset(); }
	inline std::weak_ptr<SceneNode> getTarget() const override { return this->_target; }

	Mat4x4 getWorldViewProjection(const std::shared_ptr<Scene>& scene) const override;

	inline Mat4x4 getProjection() const override { return this->_projection; }
	inline Mat4x4 getView() const override { return this->_view; }

	void setCameraOffset(const Vec4& camera_offset) { this->_camera_offset_vector = camera_offset; }

private:
	Frustum _frustum;
	Vec4 _camera_offset_vector;
	std::weak_ptr<SceneNode> _target;
	bool _debug_mode {false};

	Mat4x4 _projection;
	Mat4x4 _view;
	// bool _active;
};

}
