#include "baseGameLogic.hpp"

#include "views/IView.hpp"
#include <utilities/logging/logger.hpp>

BaseGameLogic::BaseGameLogic()
{ }

BaseGameLogic::~BaseGameLogic()
{ }

const bool BaseGameLogic::initialize()
{
	return true;
}

void BaseGameLogic::addView(const std::shared_ptr<IView>& view, const EntityId entityId)
{
	ViewId viewId = static_cast<ViewId>(this->_views.size());
	this->_views.push_back(view);
	view->onAttach(viewId, entityId);
	if (!view->onRestore())
		printf("error");  // WARNING: log error
}

void BaseGameLogic::removeView(const std::shared_ptr<IView>& view)
{
	this->_views.remove(view);
}

void BaseGameLogic::onUpdate(Milliseconds delta)
{
	// TODO: game states
	for (const auto& view : this->_views)
		view->onUpdate(delta);
}
