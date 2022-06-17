#include "scene.hpp"

#include <events/events/newRenderComponentEvent.hpp>
#include <events/iEventManager.hpp>
#include <resource_cache/resourceCache.hpp>
#include <serviceProvider.hpp>
#include <utilities/logging/logger.hpp>
#include <utilities/time.hpp>

#include "iRenderer.hpp"
#include "scene_nodes/alphaSceneNode.hpp"
#include "scene_nodes/cameraNode.hpp"
#include "scene_nodes/rootNode.hpp"

Scene::Scene(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ResourceCache> cache)
	: _root(std::make_shared<RootNode>()), _renderer(renderer), _cache(cache)
{
	this->_matrix_stack.push(std::move(Mat4x4()));
	// hook up event manager
}

[[nodiscard]] bool Scene::initialize()
{
	this->registerEventSinks();

	if (this->_root == nullptr)
		return true;

	return this->_root->initialize(this->shared_from_this());
}

bool Scene::onRender()
{
	if (this->_root && this->_camera)
	{
		auto scene = this->shared_from_this();
		if (!this->_camera->setViewTransform(scene))
		{
			logWarning("failed to set the camera's view transform", "render");
			return false;
		}

		// setup lights

		if (!this->_root->preRender(scene))
		{
			logWarning("failed to pre-render scene root", "render");
			return false;
		}

		if (!this->_root->render(scene))
		{
			logWarning("failed to render scene root", "render");
			return false;
		}

		if (!this->_root->renderChildren(scene))
		{
			logWarning("failed to render scene root's children", "render");
			return false;
		}

		if (!this->_root->postRender(scene))
		{
			logWarning("failed to post-render scene root", "render");
			return false;
		}
	}

	this->renderAlphaPass();
	return true;
}

bool Scene::onUpdate(const Milliseconds delta)
{
	if (this->_root == nullptr)
		return true;

	return this->_root->onUpdate(this->shared_from_this(), delta);
}

bool Scene::addChild(EntityId id, std::shared_ptr<ISceneNode> child)
{
	if (id != InvalidEntityId)
		this->_entity_map[id] = child;

	// setup light garbage

	return child->initialize(this->shared_from_this()) && _root->addChild(child);
}

bool Scene::removeChild(EntityId id)
{
	if (id == InvalidEntityId)
		return false;

	// remove from lights

	this->_entity_map.erase(id);
	return this->_root->removeChild(id);
}

void Scene::pushAndSetMatrix(const Mat4x4& to_world)
{
	auto product = (this->_matrix_stack.top()) * to_world;
	this->_matrix_stack.push(std::move(product));
	this->_renderer->setWorldTransform(this->_matrix_stack.top());
}

void Scene::pushAndSetMatrix(const std::shared_ptr<Mat4x4>& to_world)
{
	auto product = (this->_matrix_stack.top()) * *to_world;
	this->_matrix_stack.push(Mat4x4(product));
	this->_renderer->setWorldTransform(this->_matrix_stack.top());
}

void Scene::popMatrix()
{
	this->_matrix_stack.pop();
	this->_renderer->setWorldTransform(this->_matrix_stack.top());
}

const Mat4x4& Scene::getTopMatrix() const
{
	return this->_matrix_stack.top();
}

void Scene::registerEventSinks()
{
	registerEventSink("new-render-component-event", Scene::onNewRenderComponent);
}

void Scene::onNewRenderComponent(std::shared_ptr<IEvent> event)
{
	auto render_event = std::dynamic_pointer_cast<NewRenderComponentEvent>(event);
	if (!this->addChild(render_event->_entity_id, render_event->_scene_node))
	{
		// log error
	}
}

void Scene::renderAlphaPass()
{
	auto alpha_pass = this->_renderer->prepareAlphaPass();

	this->_alpha_scene_nodes.sort();

	while (!this->_alpha_scene_nodes.empty())
	{
		auto iter = this->_alpha_scene_nodes.rbegin();
		this->pushAndSetMatrix((*iter)->to_world);
		(*iter)->node->render(this->shared_from_this());
		this->popMatrix();
		this->_alpha_scene_nodes.pop_back();
	}
}
