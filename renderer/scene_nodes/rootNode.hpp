#pragma once

#include "sceneNode.hpp"

class Scene;

class RootNode: public SceneNode
{
public:
	RootNode();
	virtual ~RootNode() = default;

	bool addChild(const std::shared_ptr<ISceneNode>& child) override;
	bool renderChildren(const std::shared_ptr<Scene>& scene) override;
	bool isVisible(const std::shared_ptr<Scene>& /*scene*/) const override { return true; }
};
