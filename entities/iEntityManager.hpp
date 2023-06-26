#pragma once

#include "entity.hpp"

class IEntityManager
{
public:
	virtual ~IEntityManager() = default;

	virtual std::shared_ptr<Entity> getEntity(const EntityId id) const = 0;
	virtual std::shared_ptr<Entity> getEntity(const std::string& tag) const = 0;
	virtual void removeEntity(const EntityId id) = 0;
};
