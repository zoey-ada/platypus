#include "renderComponent2d.hpp"

#include <platypus_proto/entity.hpp>

#include <events/events/newRenderComponentEvent.hpp>
#include <events/events/removeRenderComponentEvent.hpp>
#include <events/iEventManager.hpp>
#include <renderer/scene_nodes/rectangleNode.hpp>
#include <renderer/scene_nodes/spriteNode.hpp>
#include <serviceProvider.hpp>

#include "transformComponent2d.hpp"

RenderComponent2d::~RenderComponent2d()
{
	auto event = std::make_shared<RemoveRenderComponentEvent>(this->_owner->getId());
	ServiceProvider::getEventManager()->triggerEvent(event);
}

bool RenderComponent2d::initialize(const std::shared_ptr<Message>& data)
{
	this->_render_data = std::dynamic_pointer_cast<platypus::RenderComponent_2d>(data);
	return this->_render_data != nullptr;
}

void RenderComponent2d::postInitialize()
{
	auto event =
		std::make_shared<NewRenderComponentEvent>(this->_owner->getId(), this->getSceneNode());
	ServiceProvider::getEventManager()->triggerEvent(event);
}

void RenderComponent2d::update(const Milliseconds /*delta*/)
{
	auto transform = _owner->getComponent<TransformComponent2d>("transform_component_2d").lock();
	if (transform != nullptr)
	{
		this->_scene_node->properties()->toWorld(transform->getTransform());
	}
}

std::shared_ptr<SceneNode> RenderComponent2d::getSceneNode()
{
	if (this->_scene_node != nullptr)
	{
		return this->_scene_node;
	}

	auto transform = _owner->getComponent<TransformComponent2d>("transform_component_2d").lock();
	if (transform != nullptr)
	{
		PtSceneNodeData node_data {};
		node_data.render_pass = RenderPass::Entity;
		node_data.entity_id = _owner->getId();
		node_data.name = "rect";
		node_data.to = &transform->getTransform();

		if (this->_render_data->has_sprite_sheet())
		{
			platypus::SpriteNodeData sprite_node_data {};
			sprite_node_data.base_node_data = &node_data;
			sprite_node_data.vetex_shader_path = this->_render_data->vertex_shader().c_str();
			sprite_node_data.pixel_shader_path = this->_render_data->pixel_shader().c_str();
			sprite_node_data.texture_path = this->_render_data->texture().c_str();
			sprite_node_data.sprite_dimensions.height =
				static_cast<uint64_t>(this->_render_data->sprite_sheet().viewport_size().height());
			sprite_node_data.sprite_dimensions.width =
				static_cast<uint64_t>(this->_render_data->sprite_sheet().viewport_size().width());

			this->_scene_node = std::make_shared<platypus::SpriteNode>(&sprite_node_data);
		}
		else
		{
			PtRectangleNodeData rect_node_data {};
			rect_node_data.texture_path = this->_render_data->texture().c_str();
			rect_node_data.pixel_shader_path = this->_render_data->pixel_shader().c_str();
			rect_node_data.vetex_shader_path = this->_render_data->vertex_shader().c_str();

			this->_scene_node = std::make_shared<RectangleNode>(&node_data, &rect_node_data);
		}
	}

	return this->_scene_node;
}

std::optional<platypus::SpriteAnimation> RenderComponent2d::getAnimation(std::string action) const
{
	std::optional<platypus::SpriteAnimation> found_animation;

	if (action.empty() || !this->_render_data->has_sprite_sheet() ||
		this->_render_data->sprite_sheet().animations_size() == 0)
		return found_animation;

	for (const auto& animation : this->_render_data->sprite_sheet().animations())
	{
		if (animation.action() == action)
		{
			found_animation = animation;
			break;
		}
	}

	return found_animation;
}

void RenderComponent2d::updateAnimationFrame()
{
	auto animation = this->getAnimation(this->_current_animation);
	if (animation.has_value())
	{
		uint32_t num_frames = animation.value().frames_size();
		this->_current_animation_frame = ((this->_current_animation_frame + 1) % num_frames);
		auto sprite = animation->frames(this->_current_animation_frame);

		std::dynamic_pointer_cast<platypus::SpriteNode>(this->getSceneNode())
			->setSprite({sprite.x(), sprite.y()});
	}
}

std::shared_ptr<EntityComponent> createRenderComponent2d()
{
	return std::make_shared<RenderComponent2d>();
}
