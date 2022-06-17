#include "renderComponent3d.hpp"

#include <platypus_proto/entity.hpp>

#include <events/events/newRenderComponentEvent.hpp>
#include <events/iEventManager.hpp>
#include <renderer/scene_nodes/meshNode.hpp>
#include <serviceProvider.hpp>

#include "transformComponent3d.hpp"

bool RenderComponent3d::initialize(const std::shared_ptr<Message>& data)
{
	this->_render_data = std::dynamic_pointer_cast<platypus::RenderComponent_3d>(data);
	return this->_render_data != nullptr;
}

void RenderComponent3d::postInitialize()
{
	auto event =
		std::make_shared<NewRenderComponentEvent>(this->_owner->getId(), this->getSceneNode());
	ServiceProvider::getEventManager()->triggerEvent(event);
}

void RenderComponent3d::update(const Milliseconds /*delta*/)
{
	auto transform = _owner->getComponent<TransformComponent3d>("transform_component_3d").lock();
	if (transform != nullptr)
	{
		this->_scene_node->properties()->toWorld(transform->getTransform());
	}
}

std::shared_ptr<SceneNode> RenderComponent3d::getSceneNode()
{
	if (this->_scene_node != nullptr)
	{
		return this->_scene_node;
	}

	auto transform = _owner->getComponent<TransformComponent3d>("transform_component_3d").lock();
	if (transform != nullptr)
	{
		PtSceneNodeData node_data {};
		node_data.render_pass = RenderPass::Entity;
		node_data.entity_id = _owner->getId();
		node_data.name = "mesh";
		node_data.to = &transform->getTransform();

		PtMeshNodeData mesh_node_data {};
		mesh_node_data.mesh_path = this->_render_data->mesh().c_str();
		mesh_node_data.texture_path = this->_render_data->texture().c_str();
		mesh_node_data.pixel_shader_path = this->_render_data->pixel_shader().c_str();
		mesh_node_data.vetex_shader_path = this->_render_data->vertex_shader().c_str();

		this->_scene_node = std::make_shared<MeshNode>(&node_data, &mesh_node_data);
	}

	return this->_scene_node;
}

std::shared_ptr<EntityComponent> createRenderComponent3d()
{
	return std::make_shared<RenderComponent3d>();
}
