#include "sceneNode.hpp"

#include "../scene.hpp"
#include "sceneNodeProperties.hpp"

SceneNode::SceneNode(PtSceneNodeData* node_data)
{
	this->_properties->_name = node_data->name;
	this->_properties->_entity_id = node_data->entity_id;
	this->_properties->_render_pass = node_data->render_pass;
	this->_properties->_ambient = Color::white;
	this->_properties->_diffuse = node_data->diffuse_color;
	this->setTransform(node_data->to, node_data->from);
}

SceneNode::SceneNode(PtSceneNodeData node_data): SceneNode(&node_data)
{
	this->_properties->_name = std::move(node_data.name);
	this->_properties->_entity_id = node_data.entity_id;
	this->_properties->_render_pass = node_data.render_pass;
	this->_properties->_ambient = Color::white;
	this->_properties->_diffuse = std::move(node_data.diffuse_color);
	this->setTransform(std::move(node_data.to), std::move(node_data.from));
}

bool SceneNode::initialize(const std::shared_ptr<Scene>& scene)
{
	bool success = true;

	for (auto child : this->_children)
		success &= child->initialize(scene);

	return success;
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

bool SceneNode::preRender(const std::shared_ptr<Scene>& scene)
{
	scene->pushAndSetMatrix(this->_properties->toWorld());
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

bool SceneNode::postRender(const std::shared_ptr<Scene>& scene)
{
	scene->popMatrix();
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

void SceneNode::setTransform(const std::shared_ptr<Mat4x4>& to_world,
	const std::shared_ptr<Mat4x4> from_world)
{
	this->_properties->_to_world = *to_world;

	if (from_world == nullptr)
		this->_properties->_from_world = this->_properties->_to_world.inverse();
	else
		this->_properties->_from_world = *from_world;
}