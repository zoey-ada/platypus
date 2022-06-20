#pragma once

#include <cstdint>
#include <map>

#include <platypus_proto/entity.hpp>

#include <utilities/time/utils.hpp>

using EntityId = uint64_t;
extern const EntityId InvalidEntityId;

class EntityComponent;

using EntityComponentMap = std::map<std::string, std::shared_ptr<EntityComponent>>;
using ComponentMapping = std::pair<std::string, std::shared_ptr<EntityComponent>>;

class Entity
{
	friend class EntityFactory;

public:
	explicit Entity(EntityId id);
	virtual ~Entity();

	bool initialize(const platypus::Entity& data);
	void postInitialize();
	void deinitialize();

	void update(Milliseconds delta);

	inline EntityId getId() const { return this->_id; }

	template<class ComponentType>
	std::weak_ptr<ComponentType> getComponent(std::string id)
	{
		auto iter = this->_components.find(id);

		if (iter == this->_components.end())
			return std::weak_ptr<ComponentType>();

		return std::static_pointer_cast<ComponentType>(iter->second);
	}

private:
	EntityId _id {InvalidEntityId};
	EntityComponentMap _components {};

	void addComponent(std::shared_ptr<EntityComponent> component);
};
