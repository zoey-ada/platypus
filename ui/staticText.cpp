#include "staticText.hpp"

#include <platypus_proto/entity.hpp>

#include <entities/components/transformComponent3d.hpp>
#include <events/events/newRenderComponentEvent.hpp>
#include <events/iEventManager.hpp>
#include <renderer/iRenderer.hpp>
#include <renderer/scene_nodes/rectangleNode.hpp>
#include <renderer/scene_nodes/textNode.hpp>
#include <serviceProvider.hpp>

StaticText::StaticText(const Vec2& position, const char* text, EntityId entity_id)
	: _text(text), _position(Vec3(position.x, position.y, 50)), _entity_id(entity_id)
{
	this->_transform_component = this->getTransformComponent();
	this->_transform_component->setPosition(Vec3(position.x, position.y, 50));
}

void StaticText::initialize()
{
	auto renderer = ServiceProvider::getRenderer();
	auto text_metrics = renderer->measureText(this->_text.c_str(), "hack.ttf", 48);
	float line_height = static_cast<float>(text_metrics.size.height) / text_metrics.max_ascender;
	float line_width = static_cast<float>(text_metrics.size.width) / text_metrics.max_ascender;

	auto position = this->_position;
	position.y -= static_cast<float>(text_metrics.max_descender) / text_metrics.max_ascender;
	this->_transform_component->setPosition(position);
	this->_transform_component->setScale(line_width, line_height, 1.0);

	auto event = std::make_shared<NewRenderComponentEvent>(this->_entity_id, this->getSceneNode());
	ServiceProvider::getEventManager()->triggerEvent(event);

	this->registerEventSinks();
}

std::shared_ptr<SceneNode> StaticText::getSceneNode()
{
	if (this->_scene_node != nullptr)
	{
		return this->_scene_node;
	}

	PtSceneNodeData node_data {};
	node_data.render_pass = RenderPass::UserInterface;
	node_data.name = "static_text";
	node_data.to = &this->_transform_component->getTransform();

	PtTextNodeDesc text_node_data {};
	text_node_data.pixel_shader_path = "assets.zip/shaders/simple_pixel.cso";
	text_node_data.vertex_shader_path = "assets.zip/shaders/simple_vertex.cso";
	text_node_data.message = this->_text.c_str();
	text_node_data.base_node_data = &node_data;

	this->_scene_node = std::make_shared<TextNode>(&text_node_data);

	return this->_scene_node;
}

std::shared_ptr<TransformComponent3d> StaticText::getTransformComponent()
{
	auto data = std::make_shared<platypus::TransformComponent_3d>();
	data->set_position_x(0.0);
	data->set_position_y(0.0);
	data->set_position_z(50.0);
	data->set_scale_x(1.0);
	data->set_scale_y(1.0);
	data->set_scale_z(1.0);
	data->set_rotation_x(0.0);
	data->set_rotation_y(0.0);
	data->set_rotation_z(0.0);

	auto component = std::make_shared<TransformComponent3d>();
	if (!component->initialize(data))
	{
		// error
	}
	return component;
}
