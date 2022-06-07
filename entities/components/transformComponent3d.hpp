#pragma once

#include <utilities/math/mathTypes.hpp>

#include "entityComponent.hpp"

namespace platypus
{
class TransformComponent_3d;
}

class IEvent;
class SceneNode;

class TransformComponent3d: public EntityComponent
{
public:
	TransformComponent3d() = default;
	virtual ~TransformComponent3d() = default;

	[[nodiscard]] bool initialize(const std::shared_ptr<Message>& data) override;
	void postInitialize() override;
	[[nodiscard]] const char* name() const override { return "transform_component_3d"; }

	[[nodiscard]] Vec3 getPosition() const;
	void setPosition(const Vec3& new_position);
	void setPosition(const float x, const float y, const float z);

	[[nodiscard]] Vec3 getRotation() const;
	void setRotation(const Vec3& new_rotation);
	void setRotation(const float x, const float y, const float z);

	[[nodiscard]] Vec3 getScale() const;
	void setScale(const Vec3& new_scale);
	void setScale(const float x, const float y, const float z);

	[[nodiscard]] const Mat4x4& getTransform() const { return this->_transform; }

	void registerEventSinks();
	void onRotateY(std::shared_ptr<IEvent> event);

private:
	std::shared_ptr<platypus::TransformComponent_3d> _transform_data;
	Mat4x4 _transform;

	void updateTransform();
};

[[nodiscard]] std::shared_ptr<EntityComponent> createTransformComponent3d();
