#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "entity.hpp"

namespace google
{
namespace protobuf
{
class Message;
}
}

using Message = google::protobuf::Message;

class EntityComponent;

using EntityComponentCreator = std::function<std::shared_ptr<EntityComponent>()>;
using EntityComponentCreatorMap = std::map<std::string, EntityComponentCreator>;

class EntityFactory
{
public:
	EntityFactory();
	virtual ~EntityFactory() = default;

	std::shared_ptr<Entity> createEntity(const std::string& entity_resource,
		std::string tag = std::string());

	void registerComponent(std::string name, EntityComponentCreator create_function);

private:
	EntityId _last_entity_id {InvalidEntityId};
	EntityComponentCreatorMap _entity_component_creators {};

	std::shared_ptr<EntityComponent> createComponent(std::shared_ptr<Message>& data);

	void registerStandardComponents();

	inline EntityId getNextEntityId()
	{
		++this->_last_entity_id;
		return this->_last_entity_id;
	}
};
