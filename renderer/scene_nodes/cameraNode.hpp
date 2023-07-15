#pragma once

#include <memory>

#include <utilities/math/mathTypes.hpp>

#include "sceneNode.hpp"

class Scene;
class SceneNode;

using SceneNodeData = PtSceneNodeData;

namespace platypus
{

class CameraNode: public SceneNode
{
public:
	CameraNode(SceneNodeData* node_data): SceneNode(node_data) {}
	CameraNode(SceneNodeData node_data): SceneNode(node_data) {}
	virtual ~CameraNode() = default;

	virtual bool setViewTransform(const std::shared_ptr<Scene>& scene) = 0;

	virtual void setTarget(const std::shared_ptr<SceneNode>& target) = 0;
	virtual void clearTarget() = 0;
	virtual std::weak_ptr<SceneNode> getTarget() const = 0;

	virtual Mat4x4 getWorldViewProjection(const std::shared_ptr<Scene>& scene) const = 0;

	virtual Mat4x4 getProjection() const = 0;
	virtual Mat4x4 getView() const = 0;
};
}
