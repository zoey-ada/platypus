#pragma once

#include <list>
#include <memory>

#include <entities/entity.hpp>
#include <entities/iEntityManager.hpp>
#include <utilities/time/utils.hpp>

class IView;
class EntityFactory;

enum class GameState
{
	Initializing,
	LoadingMainMenu,
	MainMenu,
	LoadingPauseMenu,
	PauseMenu,
	Loading,
	Running,
};

class BaseGameLogic: public IEntityManager
{
public:
	BaseGameLogic() = default;
	virtual ~BaseGameLogic() = default;

	virtual bool initialize();
	virtual void deinitialize() {};

	virtual void addView(const std::shared_ptr<IView>& view,
		const EntityId entityId = InvalidEntityId);
	virtual void removeView(const std::shared_ptr<IView>& view);

	virtual void onUpdate(Milliseconds delta);

	std::shared_ptr<Entity> createEntity(const char* entity_resource);
	void removeEntity(const EntityId id) override;
	void removeAllEntities();

	std::shared_ptr<Entity> getEntity(const EntityId id) const override;
	std::shared_ptr<Entity> getEntity(const std::string& tag) const override;

	virtual void changeState(const GameState new_state);
	GameState getState() const { return _state; }

	[[nodiscard]] std::list<std::shared_ptr<IView>> views() const { return this->_views; }

protected:
	GameState _state {GameState::Initializing};
	std::list<std::shared_ptr<IView>> _views;
	std::shared_ptr<EntityFactory> _entity_factory;
	std::map<EntityId, std::shared_ptr<Entity>> _entities;
};
