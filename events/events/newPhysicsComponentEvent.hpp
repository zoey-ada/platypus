#pragma once

#include <memory>

#include "iEvent.hpp"

class RigidBodyObject;

class NewPhysicsComponentEvent: public IEvent
{
public:
	explicit NewPhysicsComponentEvent(RigidBodyObject* rigid_object): _rigid_object(rigid_object) {}
	virtual ~NewPhysicsComponentEvent() = default;

	const char* getType() override { return "new-physics-component-event"; }

	RigidBodyObject* _rigid_object;
};
