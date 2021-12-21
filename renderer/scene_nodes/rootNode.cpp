#include "rootNode.hpp"

#include "../scene.hpp"

RootNode::RootNode()
	: SceneNode("scene root", InvalidEntityId)
{
	this->_children.reserve(static_cast<size_t>(RenderPass::Last));

	auto static_nodes = std::make_shared<SceneNode>("static-nodes", InvalidEntityId, RenderPass::Static);
	this->_children.push_back(static_nodes);

	auto entity_nodes = std::make_shared<SceneNode>("entity-nodes", InvalidEntityId, RenderPass::Entity);
	this->_children.push_back(entity_nodes);

	auto sky_nodes = std::make_shared<SceneNode>("sky-nodes", InvalidEntityId, RenderPass::Sky);
	this->_children.push_back(sky_nodes);

	auto invisible_nodes = std::make_shared<SceneNode>("invisible-nodes", InvalidEntityId, RenderPass::NotRendered);
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
	for (auto pass = static_cast<int>(RenderPass::First); pass < static_cast<int>(RenderPass::Last); ++pass)
	{
		switch (pass)
		{
		case static_cast<int>(RenderPass::Static):
			[[fallthrough]];
		case static_cast<int>(RenderPass::Entity):
			return this->_children[pass]->renderChildren(scene);

		case static_cast<int>(RenderPass::Sky):
			// setup for sky pass
			return this->_children[pass]->renderChildren(scene);

		default:
			break;
		}
	}

	return true;
}
