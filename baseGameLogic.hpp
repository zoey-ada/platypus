#pragma once

#include <list>
#include <memory>

#include <entities/entity.hpp>
#include <utilities/time/utils.hpp>

class IView;
class EntityFactory;

enum class GameState
{
	Initializing,
	MainMenu,
	Loading,
	Running,
};

class BaseGameLogic
{
public:
	BaseGameLogic() = default;
	virtual ~BaseGameLogic() = default;

	bool initialize();
	void deinitialize();

	virtual void addView(const std::shared_ptr<IView>& view,
		const EntityId entityId = InvalidEntityId);
	virtual void removeView(const std::shared_ptr<IView>& view);

	virtual void onUpdate(Milliseconds delta);

	std::shared_ptr<Entity> createEntity(const char* entity_resource);
	void removeEntity(EntityId entity_id);

	virtual void changeState(const GameState new_state);
	const GameState getState() const { return _state; }

	[[nodiscard]] std::list<std::shared_ptr<IView>> views() const { return this->_views; }

protected:
	GameState _state {GameState::Initializing};
	std::list<std::shared_ptr<IView>> _views;
	std::shared_ptr<EntityFactory> _entity_factory;
	std::map<EntityId, std::shared_ptr<Entity>> _entities;
};
