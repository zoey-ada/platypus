#pragma once

#include "entities/entity.hpp"
#include "utilities/time.hpp"

#include <list>
#include <memory>

class IView;

class BaseGameLogic
{
public:
	BaseGameLogic() = default;
	virtual ~BaseGameLogic() = default;

	bool initialize();

	virtual void addView(const std::shared_ptr<IView>& view, const EntityId entityId = InvalidEntityId);
	virtual void removeView(const std::shared_ptr<IView>& view);

	virtual void onUpdate(Milliseconds delta);

	[[nodiscard]] std::list<std::shared_ptr<IView>> views() const { return this->_views; }

private:
	std::list<std::shared_ptr<IView>> _views;
};
