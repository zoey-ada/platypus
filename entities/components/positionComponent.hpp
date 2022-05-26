#pragma once

#include <utilities/math/mathTypes.hpp>

#include "entityComponent.hpp"

namespace platypus
{
class PositionComponent;
}

class SceneNode;

class PositionComponent: public EntityComponent
{
public:
	PositionComponent() = default;
	virtual ~PositionComponent() = default;

	[[nodiscard]] bool initialize(const std::shared_ptr<Message>& data) override;
	void postInitialize() override;
	[[nodiscard]] const char* name() const override { return "position_component"; }

	[[nodiscard]] Vec3 getPosition() const;
	void setPosition(const Vec3& new_position);
	void setPosition(const float x, const float y, const float z);

private:
	std::shared_ptr<platypus::PositionComponent> _position_data;
};

[[nodiscard]] std::shared_ptr<EntityComponent> createPositionComponent();
