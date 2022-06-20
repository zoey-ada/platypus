#pragma once

#include <utilities/math/mathTypes.hpp>

#include "entityComponent.hpp"

namespace platypus
{
class MovementComponent_2d;
}

class IEvent;
class SceneNode;

class MovementComponent2d: public EntityComponent
{
public:
	MovementComponent2d() = default;
	virtual ~MovementComponent2d() = default;

	[[nodiscard]] bool initialize(const std::shared_ptr<Message>& data) override;
	void postInitialize() override;

	void update(const Milliseconds delta) override;
	[[nodiscard]] const char* name() const override { return "movement_component_2d"; }

	[[nodiscard]] Vec2 getAcceleration() const;
	void setAcceleration(const Vec2& new_acceleration);

	[[nodiscard]] float getAccelerationX() const;
	void setAccelerationX(const float new_acceleration);

	[[nodiscard]] float getAccelerationY() const;
	void setAccelerationY(const float new_acceleration);

	[[nodiscard]] inline Vec2 getVelocity() const
	{
		return Vec2 {this->_velocity_x, this->_velocity_y};
	}
	inline void setVelocity(const Vec2& new_velocity)
	{
		this->_velocity_x = new_velocity.x;
		this->_velocity_y = new_velocity.y;
	}

	[[nodiscard]] inline float getVelocityX() const { return this->_velocity_x; }
	inline void setVelocityX(const float new_velocity) { this->_velocity_x = new_velocity; }

	[[nodiscard]] inline float getVelocityY() const { return this->_velocity_y; }
	inline void setVelocityY(const float new_velocity) { this->_velocity_y = new_velocity; }

	[[nodiscard]] Vec2 getMaxVelocity() const;
	void setMaxVelocity(const Vec2& new_max_velocity);

	[[nodiscard]] float getMaxVelocityX() const;
	void setMaxVelocityX(const float new_max_velocity);

	[[nodiscard]] float getMaxVelocityY() const;
	void setMaxVelocityY(const float new_max_velocity);

	[[nodiscard]] inline bool getIsMovingX() const { return this->_is_moving_x; }
	inline void setIsMovingX(const bool new_is_moving) { this->_is_moving_x = new_is_moving; }

	[[nodiscard]] inline bool getIsMovingY() const { return this->_is_moving_y; }
	inline void setIsMovingY(const bool new_is_moving) { this->_is_moving_y = new_is_moving; }

	void registerEventSinks();
	void onInput(std::shared_ptr<IEvent> event);

private:
	std::shared_ptr<platypus::MovementComponent_2d> _movement_data;
	float _velocity_x {0.0f};
	float _velocity_y {0.0f};
	bool _is_moving_x {false};
	bool _is_moving_y {false};
};

[[nodiscard]] std::shared_ptr<EntityComponent> createMovementComponent2d();
