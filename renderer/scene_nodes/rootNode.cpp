#include "rootNode.hpp"

#include "../scene.hpp"

RootNode::RootNode(): SceneNode(this->getSceneNodeData())
{
	this->_children.reserve(static_cast<size_t>(RenderPass::Last));

	PtSceneNodeData static_data {};
	static_data.name = "static-nodes";
	static_data.render_pass = RenderPass::Static;
	auto static_nodes = std::make_shared<SceneNode>(&static_data);
	this->_children.push_back(static_nodes);

	PtSceneNodeData entity_data {};
	entity_data.name = "entity-nodes";
	entity_data.render_pass = RenderPass::Entity;
	auto entity_nodes = std::make_shared<SceneNode>(&entity_data);
	this->_children.push_back(entity_nodes);

	PtSceneNodeData sky_data {};
	sky_data.name = "sky-nodes";
	sky_data.render_pass = RenderPass::Sky;
	auto sky_nodes = std::make_shared<SceneNode>(&sky_data);
	this->_children.push_back(sky_nodes);

	PtSceneNodeData invisible_data {};
	invisible_data.name = "invisible-nodes";
	invisible_data.render_pass = RenderPass::NotRendered;
	auto invisible_nodes = std::make_shared<SceneNode>(&invisible_data);
	this->_children.push_back(invisible_nodes);
}

bool RootNode::addChild(const std::shared_ptr<ISceneNode>& child)
{
	auto pass = child->properties()->renderPass();
	auto index = static_cast<unsigned int>(pass);

	if (index >= this->_children.size() || !this->_children[index])
	{
		return false;
	}

	return this->_children[index]->addChild(child);
}

bool RootNode::renderChildren(const std::shared_ptr<Scene>& scene)
{
	bool success = true;

	for (auto pass = static_cast<int>(RenderPass::First); pass < static_cast<int>(RenderPass::Last);
		 ++pass)
	{
		switch (pass)
		{
		case static_cast<int>(RenderPass::Static):
			[[fallthrough]];
		case static_cast<int>(RenderPass::Entity):
			[[fallthrough]];
		case static_cast<int>(RenderPass::UserInterface):
			success = success ? this->_children[pass]->renderChildren(scene) : false;
			break;

		case static_cast<int>(RenderPass::Sky):
			// setup for sky pass
			success = success ? this->_children[pass]->renderChildren(scene) : false;
			break;

		default:
			break;
		}
	}

	return success;
}
