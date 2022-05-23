#include "entityFactory.hpp"

#include <google/protobuf/message.h>
#include <platypus_proto/util.hpp>

#include "components/entityComponent.hpp"

EntityFactory::EntityFactory()
{}

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
			bool valid = m.IsInitialized();

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
