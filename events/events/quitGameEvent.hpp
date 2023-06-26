#pragma once

#include <memory>

#include "iEvent.hpp"

class QuitGameEvent: public IEvent
{
public:
	QuitGameEvent() = default;
	virtual ~QuitGameEvent() = default;

	const char* getType() override { return "quit-game-event"; }
};
