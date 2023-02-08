#pragma once

#include <memory>

#include <entities/entity.hpp>
#include <utilities/common/ptExtent.hpp>
#include <utilities/math/mathTypes.hpp>

class IEvent;
class SceneNode;
class TextNode;
class TransformComponent3d;

class StaticText
{
public:
	StaticText(const Vec2& position, const char* text, EntityId entity_id = InvalidEntityId);
	virtual ~StaticText() = default;

	void initialize();
	virtual void registerEventSinks() {}

	std::shared_ptr<SceneNode> getSceneNode();
	std::shared_ptr<TransformComponent3d> getTransformComponent();

protected:
	std::string _text {""};
	std::shared_ptr<TextNode> _scene_node {nullptr};
	std::shared_ptr<TransformComponent3d> _transform_component {nullptr};
	Vec3 _position;

private:
	EntityId _entity_id {InvalidEntityId};
};
