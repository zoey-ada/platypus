#include "baseGameLogic.hpp"

#include <entities/components/transformComponent3d.hpp>
#include <entities/entityFactory.hpp>
#include <resource_cache/iResourceCache.hpp>
#include <utilities/logging/logger.hpp>

#include "views/iView.hpp"

bool BaseGameLogic::initialize(std::shared_ptr<platypus::IResourceCache> cache)
{
	this->_entity_factory = std::make_shared<EntityFactory>(cache);
	return true;
}

void BaseGameLogic::addView(const std::shared_ptr<IView>& view, const EntityId entityId)
{
	auto viewId = static_cast<ViewId>(this->_views.size());
	this->_views.push_back(view);
	view->onAttach(viewId, entityId);
	if (!view->onRestore())
		logWarning("error");
}

void BaseGameLogic::removeView(const std::shared_ptr<IView>& view)
{
	this->_views.remove(view);
}

void BaseGameLogic::onUpdate(Milliseconds delta)
{
	switch (this->_state)
	{
	case GameState::Initializing:
	{
		this->changeState(GameState::MainMenu);
		break;
	}
	case GameState::MainMenu:
	{
		// auto cyndaquil = this->createEntity("cyndaquil.json");
		// auto cube = this->createEntity("cube.json");
		// auto component = cube->getComponent<TransformComponent3d>("transform_component_3d");
		// component.lock()->registerEventSinks();

		for (const auto& view : this->_views)
			view->onRestore();

		this->changeState(GameState::Running);
		break;
	}
	case GameState::Loading:
	{
		break;
	}
	case GameState::Running:
	{
		for (const auto& view : this->_views)
			view->onUpdate(delta);

		for (const auto& entity : this->_entities)
			entity.second->update(delta);

		break;
	}
	default:
		throw std::exception("The game entered a state that could not be handled.");
	}
	// TODO: game states
}

std::shared_ptr<Entity> BaseGameLogic::createEntity(const char* entity_resource)
{
	auto entity = this->_entity_factory->createEntity(entity_resource);

	if (entity != nullptr)
	{
		this->_entities.insert(std::make_pair(entity->getId(), entity));
	}

	return entity;
}

std::shared_ptr<Entity> BaseGameLogic::getEntity(const EntityId id) const
{
	auto iter = this->_entities.find(id);

	if (iter != this->_entities.end())
		return iter->second;

	return nullptr;
}

std::shared_ptr<Entity> BaseGameLogic::getEntity(const std::string& tag) const
{
	for (const auto& entity : this->_entities)
	{
		if (entity.second->getTag() == tag)
			return entity.second;
	}

	return nullptr;
}

void BaseGameLogic::removeEntity(const EntityId id)
{
	auto iter = this->_entities.find(id);

	if (iter != this->_entities.end())
	{
		// because entity components have shared_ptrs to the entity object,
		// the entity must be explicitly deinitialized before it can be destroyed
		iter->second->deinitialize();
		this->_entities.erase(id);
	}
}

void BaseGameLogic::removeAllEntities()
{
	for (auto iter = this->_entities.begin(); iter != this->_entities.end();
		 iter = this->_entities.begin())
	{
		// because entity components have shared_ptrs to the entity object,
		// the entity must be explicitly deinitialized before it can be destroyed
		iter->second->deinitialize();
		this->_entities.erase(iter->first);
	}
}

void BaseGameLogic::changeState(const GameState new_state)
{
	// todo: stuffs
	this->_state = new_state;
}
