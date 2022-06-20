#pragma once

#include <utilities/math/mathTypes.hpp>

#include "entityComponent.hpp"

namespace platypus
{
class TransformComponent_2d;
}

class IEvent;
class SceneNode;

class TransformComponent2d: public EntityComponent
{
public:
	TransformComponent2d() = default;
	virtual ~TransformComponent2d() = default;

	[[nodiscard]] bool initialize(const std::shared_ptr<Message>& data) override;
	void postInitialize() override;

	void update(const Milliseconds delta) override;
	[[nodiscard]] const char* name() const override { return "transform_component_2d"; }

	[[nodiscard]] Vec2 getPosition() const;
	[[nodiscard]] float getDepth() const;
	void setPosition(const Vec2& new_position);
	void setPosition(const float x, const float y);
	void setDepth(const float z);

	[[nodiscard]] Vec2 getScale() const;
	[[nodiscard]] float getHeight() const;
	[[nodiscard]] float getWidth() const;
	void setScale(const Vec2& new_scale);
	void setScale(const float height, const float width);
	void setWidth(const float width);
	void setHeight(const float height);

	[[nodiscard]] float getRotationClockwise() const;
	void setRotationClockwise(float radians);

	[[nodiscard]] const Mat4x4& getTransform() const { return this->_transform; }

	void registerEventSinks();
	void onInput(std::shared_ptr<IEvent> event);

private:
	std::shared_ptr<platypus::TransformComponent_2d> _transform_data;
	Mat4x4 _transform;

	void updateTransform();
};

[[nodiscard]] std::shared_ptr<EntityComponent> createTransformComponent2d();
