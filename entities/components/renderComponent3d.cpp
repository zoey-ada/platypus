#include "renderComponent3d.hpp"

#include <events/events/newRenderComponentEvent.hpp>
#include <events/iEventManager.hpp>
#include <platypus_proto/entity.hpp>
#include <renderer/scene_nodes/meshNode.hpp>
#include <serviceProvider.hpp>

#include "positionComponent.hpp"

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

std::shared_ptr<SceneNode> RenderComponent3d::getSceneNode()
{
	auto position_matrix = Mat4x4::identity();

	auto weak_position = _owner->getComponent<PositionComponent>("position_component");
	if (!weak_position.expired())
	{
		position_matrix->setPosition(weak_position.lock()->getPosition());
	}

	PtSceneNodeData node_data {};
	node_data.render_pass = RenderPass::Entity;
	node_data.entity_id = _owner->getId();
	node_data.name = "mesh";
	node_data.to = position_matrix;

	PtMeshNodeData mesh_node_data {};
	mesh_node_data.mesh_path = this->_render_data->mesh().c_str();
	mesh_node_data.texture_path = this->_render_data->texture().c_str();
	mesh_node_data.pixel_shader_path = this->_render_data->pixel_shader().c_str();
	mesh_node_data.vetex_shader_path = this->_render_data->vertex_shader().c_str();

	return std::make_shared<MeshNode>(&node_data, &mesh_node_data);
}

std::shared_ptr<EntityComponent> CreateRenderComponent()
{
	return std::make_shared<RenderComponent3d>();
}
