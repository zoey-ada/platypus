#include "sceneNode.hpp"

SceneNode::SceneNode(const std::string& name, EntityId entity_id, RenderPass render_pass)
: _children(), _parent(), _properties(std::make_shared<SceneNodeProperties>())
{
	_properties->_name = name;
	_properties->_entity_id = entity_id;
	_properties->_render_pass = render_pass;
}

bool SceneNode::onUpdate(const std::shared_ptr<Scene>& scene, const Milliseconds delta)
{
	bool success = true;

	for (auto child : this->_children)
		success &= child->onUpdate(scene, delta);

	return success;
}

bool SceneNode::isVisible(const std::shared_ptr<Scene>& /*scene*/) const
{
	// determine if scene node is visible by camera frustum
	return true;
}

bool SceneNode::preRender(const std::shared_ptr<Scene>& /*scene*/)
{
	// adjust location
	return true;
}

bool SceneNode::renderChildren(const std::shared_ptr<Scene>& scene)
{
	for (auto child : this->_children)
	{
		if (child->preRender(scene))
		{
			if (child->isVisible(scene))
			{
				// handle alpha nodes
				child->render(scene);
			}

			child->renderChildren(scene);
		}

		child->postRender(scene);
	}

	return true;
}

bool SceneNode::postRender(const std::shared_ptr<Scene>& /*scene*/)
{
	// un-adjust location
	return true;
}

bool SceneNode::addChild(const std::shared_ptr<ISceneNode>& child)
{
	this->_children.push_back(child);

	// potentially adjust bounding box

	return true;
}

bool SceneNode::removeChild(const EntityId /*id*/)
{
	for (auto iter = this->_children.begin(); iter != this->_children.end(); ++iter)
	{
		// find and remove node
	}

	return false;
}
