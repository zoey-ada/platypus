#include "entity.hpp"

#include "components/entityComponent.hpp"

const EntityId InvalidEntityId = 0;

Entity::Entity(EntityId id): _id(id)
{}

Entity::~Entity()
{
	if (!this->_components.empty())
		this->shutdown();
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

void Entity::shutdown()
{
	this->_components.clear();
}

void Entity::update(Milliseconds delta)
{
	for (auto mapping : this->_components)
		mapping.second->update(delta);
}

void Entity::addComponent(std::shared_ptr<EntityComponent> component)
{
	this->_components.insert(ComponentMapping(component->name(), component));
}
