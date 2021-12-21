#pragma once

#include "iSceneNode.hpp"

#include <memory>
#include <vector>

#include "renderPass.hpp"
#include "sceneNodeProperties.hpp"

using SceneNodeList = std::vector<std::shared_ptr<ISceneNode>>;

class SceneNode: public ISceneNode
{
	friend class Scene;

public:
	explicit SceneNode(
		const std::string& name,
		EntityId entity_id = InvalidEntityId,
		RenderPass render_pass = RenderPass::Entity);

	virtual ~SceneNode() = default;

	std::shared_ptr<const SceneNodeProperties> properties() const override { return _properties; }

	bool onUpdate(const std::shared_ptr<Scene>& scene, const Milliseconds delta) override;
	[[nodiscard]] bool isVisible(const std::shared_ptr<Scene>& scene) const override;

	bool preRender(const std::shared_ptr<Scene>& scene) override;
	bool render(const std::shared_ptr<Scene>& /*scene*/) override { return true; }
	bool renderChildren(const std::shared_ptr<Scene>& scene) override;
	bool postRender(const std::shared_ptr<Scene>& scene) override;

	bool addChild(const std::shared_ptr<ISceneNode>& child) override;
	bool removeChild(const EntityId id) override;

protected:
	SceneNodeList _children;
	std::shared_ptr<SceneNode> _parent;
	std::shared_ptr<SceneNodeProperties> _properties;
};
