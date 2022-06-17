#include "entity.hpp"

#include "components/entityComponent.hpp"

const EntityId InvalidEntityId = 0;

Entity::Entity(EntityId id): _id(id)
{}

Entity::~Entity()
{
	if (!this->_components.empty())
		this->deinitialize();
}

bool Entity::initialize(const platypus::Entity& /*data*/)
{
	return true;
}

void Entity::postInitialize()
{
	for (auto mapping : this->_components)
		mapping.second->postInitialize();
}

void Entity::deinitialize()
{
	this->_components.clear();
}

void Entity::update(Milliseconds delta)
{
	for (const auto& mapping : this->_components)
		mapping.second->update(delta);
}

void Entity::addComponent(std::shared_ptr<EntityComponent> component)
{
	this->_components.insert(ComponentMapping(component->name(), component));
}
