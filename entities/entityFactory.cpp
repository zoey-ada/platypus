#include "entityFactory.hpp"

#include <platypus_proto/util.hpp>

#include <google/protobuf/message.h>

#include "components/entityComponent.hpp"
#include "components/movementComponent2d.hpp"
#include "components/physicsComponent.hpp"
#include "components/renderComponent2d.hpp"
#include "components/renderComponent3d.hpp"
#include "components/transformComponent2d.hpp"
#include "components/transformComponent3d.hpp"

EntityFactory::EntityFactory()
{
	this->_entity_component_creators["MovementComponent_2d"] = createMovementComponent2d;
	this->_entity_component_creators["PhysicsComponent"] = createPhysicsComponent;
	this->_entity_component_creators["RenderComponent_2d"] = createRenderComponent2d;
	this->_entity_component_creators["RenderComponent_3d"] = createRenderComponent3d;
	this->_entity_component_creators["TransformComponent_2d"] = createTransformComponent2d;
	this->_entity_component_creators["TransformComponent_3d"] = createTransformComponent3d;
}

std::shared_ptr<Entity> EntityFactory::createEntity(const char* entity_resource)
{
	auto data = platypus::readProtoFile<platypus::Entity>(entity_resource);

	auto entity = std::make_shared<Entity>(this->getNextEntityId());

	const auto reflection = data.GetReflection();
	const auto desc = data.GetDescriptor();

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
		return nullptr;
	}

	return component;
}
