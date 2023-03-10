#include "dynamicText.hpp"

#include <entities/components/transformComponent3d.hpp>
#include <events/events/dataChangeEvent.hpp>
#include <events/iEventManager.hpp>
#include <renderer/iRenderer.hpp>
#include <renderer/scene_nodes/textNode.hpp>
#include <serviceProvider.hpp>

DynamicText::DynamicText(const char* data_field_name, const Vec2& position, EntityId entity_id)
	: StaticText(position, "", entity_id), _data_field_name(data_field_name)
{}

void DynamicText::registerEventSinks()
{
	std::string event_type = this->_data_field_name;
	event_type.append("-data-change-event");
	// TODO: this call should be able to handle const funcs
	registerEventSink(event_type, DynamicText::onDataChanged);
}

void DynamicText::onDataChanged(std::shared_ptr<IEvent> event)
{
	this->_text = this->parseNewValue(event);

	auto renderer = ServiceProvider::getRenderer();
	auto text_metrics = renderer->measureText(this->_text.c_str(), "hack.ttf", 48);
	float line_height = static_cast<float>(text_metrics.size.height) / text_metrics.max_ascender;
	float line_width = static_cast<float>(text_metrics.size.width) / text_metrics.max_ascender;

	auto position = this->_position;
	position.y -= static_cast<float>(text_metrics.max_descender) / text_metrics.max_ascender;
	this->_transform_component->setPosition(position);
	this->_transform_component->setScale(line_width, line_height, 1.0);

	this->_scene_node->properties()->toWorld(this->_transform_component->getTransform());
	this->_scene_node->setText(_text.c_str());
}

DynamicIntegerText::DynamicIntegerText(const char* data_field_name, const Vec2& position,
	EntityId entity_id)
	: DynamicText(data_field_name, position, entity_id)
{
	this->_text = "0";
}

std::string DynamicIntegerText::parseNewValue(std::shared_ptr<IEvent> event)
{
	auto data_event = std::static_pointer_cast<DataChangeEvent<int>>(event);
	return std::to_string(data_event->_new_value);
}
