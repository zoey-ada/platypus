#include "scene.hpp"

#include <utilities/time.hpp>

#include "scene_nodes/rootNode.hpp"
#include "iRenderer.hpp"

Scene::Scene(std::shared_ptr<IRenderer> renderer)
: _root(std::make_shared<RootNode>()), _renderer(renderer), _entity_map()
{
	// hook up some shit
}

bool Scene::onRender()
{
	if (this->_root /*&& this->_camera*/)
	{
		auto scene = this->shared_from_this();
		// setup camera and lights
		if (this->_root->preRender(scene))
		{
			this->_root->render(scene);
			this->_root->renderChildren(scene);
			this->_root->postRender(scene);
		}
	}

	// render alpha pass
	return true;
}

bool Scene::onUpdate(const Milliseconds delta)
{
	if (!this->_root)
		return true;

	return this->_root->onUpdate(this->shared_from_this(), delta);
}

bool Scene::addSceneNode(EntityId id, std::shared_ptr<ISceneNode> child)
{
	if (id != InvalidEntityId)
		this->_entity_map[id] = child;

	// setup light garbage

	// child->onRestore(this->shared_from_this())
	return _root->addChild(child);
}

bool Scene::removeChild(EntityId id)
{
	if (id == InvalidEntityId)
		return false;

	// remove from lights

	this->_entity_map.erase(id);
	return this->_root->removeChild(id);
}
