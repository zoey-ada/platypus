#pragma once

#include <memory>
#include <vector>

#include "../color.hpp"
#include "iSceneNode.hpp"
#include "renderPass.hpp"

class SceneNodeProperties;

using SceneNodeList = std::vector<std::shared_ptr<ISceneNode>>;

struct PtSceneNodeData
{
	std::string name;
	EntityId entity_id = InvalidEntityId;
	RenderPass render_pass = RenderPass::Entity;
	Color diffuse_color = Color::white;
	const Mat4x4* to = Mat4x4::identity();
	const Mat4x4* from = nullptr;
};

class SceneNode: public ISceneNode
{
	friend class Scene;

public:
	explicit SceneNode(PtSceneNodeData* node_data);

	virtual ~SceneNode() = default;

	std::shared_ptr<SceneNodeProperties> properties() const override { return _properties; }

	bool initialize(const std::shared_ptr<Scene>& scene) override;
	bool reinitialize(const std::shared_ptr<Scene>& scene) override;
	void deinitialize(const std::shared_ptr<Scene>& scene) override;

	bool onUpdate(const std::shared_ptr<Scene>& scene, const Milliseconds delta) override;
	[[nodiscard]] bool isVisible(const std::shared_ptr<Scene>& scene) const override;

	bool preRender(const std::shared_ptr<Scene>& scene) override;
	bool render(const std::shared_ptr<Scene>& /*scene*/) override { return true; }
	bool renderChildren(const std::shared_ptr<Scene>& scene) override;
	bool postRender(const std::shared_ptr<Scene>& scene) override;

	bool addChild(const std::shared_ptr<ISceneNode>& child) override;
	bool removeChild(const EntityId id) override;

	void setTransform(const Mat4x4* to_world, const Mat4x4* from_world = nullptr);

protected:
	SceneNodeList _children {};
	std::shared_ptr<SceneNode> _parent {nullptr};

	SceneNode(PtSceneNodeData node_data);

private:
	std::shared_ptr<SceneNodeProperties> _properties {std::make_shared<SceneNodeProperties>()};
};
