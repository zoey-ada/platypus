#pragma once

#include "entities/entity.hpp"
#include "utilities/time.hpp"

#include <list>
#include <memory>

class IView;

class BaseGameLogic
{
public:
	BaseGameLogic();
	virtual ~BaseGameLogic();

	const bool initialize();

	virtual void addView(const std::shared_ptr<IView>& view, const EntityId entityId = InvalidEntityId);
	virtual void removeView(const std::shared_ptr<IView>& view);

	virtual void onUpdate(Milliseconds delta);

	const std::list<std::shared_ptr<IView>> views() const { return this->_views; }

protected:
	std::list<std::shared_ptr<IView>> _views;
};
