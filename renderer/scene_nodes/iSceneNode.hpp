#pragma once

#include <memory>

#include <entities/entity.hpp>
#include <utilities/time/utils.hpp>

#include "sceneNodeProperties.hpp"

class Scene;

class ISceneNode
{
public:
	virtual ~ISceneNode() = default;

	virtual std::shared_ptr<SceneNodeProperties> properties() const = 0;

	[[nodiscard]] virtual bool initialize(const std::shared_ptr<Scene>& scene) = 0;

	virtual bool onUpdate(const std::shared_ptr<Scene>& scene, const Milliseconds delta) = 0;
	[[nodiscard]] virtual bool isVisible(const std::shared_ptr<Scene>& scene) const = 0;

	virtual bool preRender(const std::shared_ptr<Scene>& scene) = 0;
	virtual bool render(const std::shared_ptr<Scene>& scene) = 0;
	virtual bool renderChildren(const std::shared_ptr<Scene>& scene) = 0;
	virtual bool postRender(const std::shared_ptr<Scene>& scene) = 0;

	virtual bool addChild(const std::shared_ptr<ISceneNode>& child) = 0;
	virtual bool removeChild(const EntityId id) = 0;
};
