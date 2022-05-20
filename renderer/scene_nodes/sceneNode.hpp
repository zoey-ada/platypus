#pragma once

#include <memory>
#include <vector>

#include "../color.hpp"
#include "iSceneNode.hpp"
#include "renderPass.hpp"

class SceneNodeProperties;

using SceneNodeList = std::vector<std::shared_ptr<ISceneNode>>;

class SceneNode: public ISceneNode
{
	friend class Scene;

public:
	explicit SceneNode(const std::string& name, EntityId entity_id = InvalidEntityId,
		RenderPass render_pass = RenderPass::Entity,
		std::shared_ptr<Mat4x4> to = Mat4x4::identity(), Color diffuse_color = Color::white,
		std::shared_ptr<Mat4x4> from = nullptr);

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

	void setTransform(const std::shared_ptr<Mat4x4>& to_world,
		const std::shared_ptr<Mat4x4> from_world);

protected:
	SceneNodeList _children {};
	std::shared_ptr<SceneNode> _parent {nullptr};

private:
	std::shared_ptr<SceneNodeProperties> _properties {std::make_shared<SceneNodeProperties>()};
};
