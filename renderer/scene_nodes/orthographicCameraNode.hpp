#pragma once

#include <utilities/common/ptExtent.hpp>
#include <utilities/math/mathTypes.hpp>

#include "cameraNode.hpp"
#include "sceneNode.hpp"

namespace platypus
{

struct OrthographicCameraNodeData
{
	SceneNodeData* base_node_data;
	Extent dimensions;
	float near_z;
	float far_z;
	std::weak_ptr<SceneNode> target;
};

class OrthographicCameraNode: public CameraNode
{
public:
	explicit OrthographicCameraNode(OrthographicCameraNodeData* data);
	explicit OrthographicCameraNode(OrthographicCameraNodeData data);
	virtual ~OrthographicCameraNode() = default;

	bool initialize(const std::shared_ptr<Scene>& scene) override;
	bool reinitialize(const std::shared_ptr<Scene>& scene) override;
	void deinitialize(const std::shared_ptr<Scene>& scene) override;

	bool render(const std::shared_ptr<Scene>& scene) override;
	bool isVisible(const std::shared_ptr<Scene>& scene) const override;

	bool setViewTransform(const std::shared_ptr<Scene>& scene) override;

	inline void setTarget(const std::shared_ptr<SceneNode>& target) override
	{
		this->_target = target;
	}
	inline void clearTarget() override { this->_target.reset(); }
	inline std::weak_ptr<SceneNode> getTarget() const override { return this->_target; }

	Mat4x4 getWorldViewProjection(const std::shared_ptr<Scene>& scene) const override;

	inline Mat4x4 getProjection() const override { return this->_projection; }
	inline Mat4x4 getView() const override { return this->_view; }

private:
	Extent _dimensions;
	float _near_z;
	float _far_z;
	std::weak_ptr<SceneNode> _target;

	Mat4x4 _projection;
	Mat4x4 _view;
	// bool _active;
};

}
