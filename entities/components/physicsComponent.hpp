#pragma once

#include "entityComponent.hpp"

namespace platypus
{
class PhysicsComponent;
}

class IShape;
class RigidBodyObject;

class PhysicsComponent: public EntityComponent
{
public:
	PhysicsComponent() = default;
	virtual ~PhysicsComponent();

	[[nodiscard]] bool initialize(const std::shared_ptr<Message>& data) override;
	void postInitialize() override;

	void update(const Milliseconds delta) override;
	[[nodiscard]] const char* name() const override { return "physics_component"; }

	[[nodiscard]] RigidBodyObject* getPhysicsObject();

private:
	std::shared_ptr<platypus::PhysicsComponent> _physics_data {nullptr};
	RigidBodyObject* _physics_object {nullptr};
};

[[nodiscard]] std::shared_ptr<EntityComponent> createPhysicsComponent();
