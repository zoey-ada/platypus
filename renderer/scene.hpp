#pragma once

#include <map>
#include <memory>

#include <entities/entity.hpp>
#include <utilities/time.hpp>

class IRenderer;
class ISceneNode;
class SceneNode;

using SceneEntityMap = std::map<EntityId, std::shared_ptr<ISceneNode>>;

class Scene: public std::enable_shared_from_this<Scene>
{
public:
	explicit Scene(std::shared_ptr<IRenderer> renderer);
	virtual ~Scene() = default;

	bool onRender();
	bool onUpdate(const Milliseconds delta);

	bool addSceneNode(EntityId id, std::shared_ptr<ISceneNode> child);
	bool removeChild(EntityId id);

protected:
	std::shared_ptr<SceneNode> _root;
	std::shared_ptr<IRenderer> _renderer;

	SceneEntityMap _entity_map;
};
