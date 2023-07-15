#include "entityFactory.hpp"

#include <filesystem>

#include <platypus_proto/util.hpp>

#include <google/protobuf/message.h>
#include <resource_cache/iResourceCache.hpp>
#include <resource_cache/resources/protobufResource.hpp>
#include <utilities/logging/logger.hpp>

#include "components/entityComponent.hpp"
#include "components/physicsComponent.hpp"
#include "components/renderComponent2d.hpp"
#include "components/renderComponent3d.hpp"
#include "components/transformComponent2d.hpp"
#include "components/transformComponent3d.hpp"

EntityFactory::EntityFactory(std::shared_ptr<platypus::IResourceCache> cache)
	: _cache(std::move(cache))
{
	this->registerStandardComponents();
}

std::shared_ptr<Entity> EntityFactory::createEntity(const std::string& entity_resource,
	std::string tag)
{
	auto resource = this->_cache->getProtobuf(entity_resource);
	auto data = platypus::toProto<platypus::Entity>(resource->getProtobufJson());

	if (tag.empty())
		tag = std::filesystem::path(entity_resource).stem().string();

	auto entity = std::make_shared<Entity>(this->getNextEntityId(), tag);

	const auto* reflection = data.GetReflection();
	const auto* desc = data.GetDescriptor();

	for (int i = 0; i < desc->field_count(); ++i)
	{
		const auto field = desc->field(i);
		if (reflection->HasField(data, field))
		{
			const auto& m = reflection->GetMessage(data, field);
			if (!m.IsInitialized())
			{
				// log error
				continue;
			}

			auto mcopy = m.New();
			mcopy->CopyFrom(m);
			auto message = std::shared_ptr<Message>(mcopy);

			auto component = this->createComponent(message);
			if (component == nullptr)
			{
				return nullptr;
			}

			entity->addComponent(component);
			component->setOwner(entity);
		}
	}

	entity->postInitialize();
	return entity;
}

void EntityFactory::registerComponent(std::string name, EntityComponentCreator create_function)
{
	this->_entity_component_creators[name] = create_function;
}

std::shared_ptr<EntityComponent> EntityFactory::createComponent(std::shared_ptr<Message>& data)
{
	std::string name(data->GetDescriptor()->name());

	auto iter = this->_entity_component_creators.find(name);
	if (iter == _entity_component_creators.end())
	{
		return nullptr;
	}

	auto component = iter->second();
	if (!component->initialize(data))
	{
		logWarning("failed to create entity component " + name, "entity");
		return nullptr;
	}

	return component;
}

void EntityFactory::registerStandardComponents()
{
	this->registerComponent("PhysicsComponent", createPhysicsComponent);
	this->registerComponent("RenderComponent_2d", createRenderComponent2d);
	this->registerComponent("RenderComponent_3d", createRenderComponent3d);
	this->registerComponent("TransformComponent_2d", createTransformComponent2d);
	this->registerComponent("TransformComponent_3d", createTransformComponent3d);
}
