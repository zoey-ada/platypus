#pragma once

#include <map>
#include <memory>
#include <stack>

#include <entities/entity.hpp>
#include <utilities/math/mathTypes.hpp>
#include <utilities/time.hpp>

class CameraNode;
class IEvent;
class IRenderer;
class ISceneNode;
class ResourceCache;
class SceneNode;

using SceneEntityMap = std::map<EntityId, std::shared_ptr<ISceneNode>>;

class Scene: public std::enable_shared_from_this<Scene>
{
public:
	explicit Scene(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ResourceCache> cache);
	virtual ~Scene() = default;

	[[nodiscard]] bool initialize();

	[[nodiscard]] bool onRender();
	[[nodiscard]] bool onUpdate(const Milliseconds delta);

	[[nodiscard]] bool addChild(EntityId id, std::shared_ptr<ISceneNode> child);
	[[nodiscard]] bool removeChild(EntityId id);

	void pushAndSetMatrix(const Mat4x4& to_world);
	void pushAndSetMatrix(const std::shared_ptr<Mat4x4>& to_world);
	void popMatrix();
	[[nodiscard]] std::shared_ptr<Mat4x4> getTopMatrix();

	void setCamera(std::shared_ptr<CameraNode> camera) { this->_camera = camera; }
	[[nodiscard]] std::shared_ptr<CameraNode> getCamera() const { return this->_camera; }

	[[nodiscard]] std::shared_ptr<IRenderer> renderer() const { return this->_renderer; }
	[[nodiscard]] std::shared_ptr<ResourceCache> cache() const { return this->_cache; }

	void registerEventSinks();
	void onNewRenderComponent(std::shared_ptr<IEvent> event);

private:
	std::shared_ptr<SceneNode> _root {nullptr};
	std::shared_ptr<CameraNode> _camera {nullptr};
	std::shared_ptr<IRenderer> _renderer {nullptr};
	std::shared_ptr<ResourceCache> _cache {nullptr};

	std::stack<std::shared_ptr<Mat4x4>> _matrix_stack {};
	SceneEntityMap _entity_map {};
};
