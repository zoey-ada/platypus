#include "sceneNode.hpp"

#include "../scene.hpp"
#include "alphaSceneNode.hpp"
#include "cameraNode.hpp"
#include "sceneNodeProperties.hpp"

SceneNode::SceneNode(PtSceneNodeData* node_data)
{
	this->_properties->_name = node_data->name;
	this->_properties->_entity_id = node_data->entity_id;
	this->_properties->_render_pass = node_data->render_pass;
	this->_properties->material().setAmbient(Color::white);
	this->_properties->material().setDiffuse(node_data->diffuse_color);
	this->setTransform(node_data->to, node_data->from);
}

SceneNode::SceneNode(PtSceneNodeData node_data): SceneNode(&node_data)
{
	this->_properties->_name = std::move(node_data.name);
	this->_properties->_entity_id = node_data.entity_id;
	this->_properties->_render_pass = node_data.render_pass;
	this->_properties->material().setAmbient(Color::white);
	this->_properties->material().setDiffuse(std::move(node_data.diffuse_color));
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
				const auto& alpha = child->properties()->material().getAlpha();
				if (alpha == 1.0f)
				{
					child->render(scene);
				}
				else if (alpha != 0.0f)
				{
					std::shared_ptr<AlphaSceneNode> alpha_scene_node =
						std::make_shared<AlphaSceneNode>();
					assert(alpha_scene_node);
					alpha_scene_node->node = child;
					alpha_scene_node->to_world = scene->getTopMatrix();

					Vec4 world_position(alpha_scene_node->to_world.getPosition());
					Mat4x4 from_world = scene->getCamera()->properties()->fromWorld();
					Vec4 screen_position = from_world.transform(world_position);
					alpha_scene_node->screen_z = screen_position.z;

					scene->addAlphaSceneNode(alpha_scene_node);
				}
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

bool SceneNode::removeChild(const EntityId id)
{
	for (auto iter = this->_children.begin(); iter != this->_children.end(); ++iter)
	{
		// find and remove node
		if ((*iter)->properties()->entityId() == id)
		{
			this->_children.erase(iter);
			return true;
		}
		else
		{
			if ((*iter)->removeChild(id))
			{
				return true;
			}
		}
	}

	return false;
}

void SceneNode::setTransform(const Mat4x4* to_world, const Mat4x4* from_world)
{
	this->_properties->_to_world = *to_world;

	if (from_world == nullptr)
		this->_properties->_from_world = this->_properties->_to_world.inverse();
	else
		this->_properties->_from_world = *from_world;
}
