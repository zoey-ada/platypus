#pragma once

#include <map>
#include <memory>
#include <stack>

#include <entities/entity.hpp>
#include <utilities/math/mathTypes.hpp>
#include <utilities/time/utils.hpp>

struct AlphaSceneNode;
class IEvent;
class IRenderer;
class ISceneNode;
class SceneNode;

namespace platypus
{
class CameraNode;
class IResourceCache;
};

using SceneEntityMap = std::map<EntityId, std::shared_ptr<ISceneNode>>;

class Scene: public std::enable_shared_from_this<Scene>
{
public:
	explicit Scene(std::shared_ptr<IRenderer> renderer,
		std::shared_ptr<platypus::IResourceCache> cache);
	virtual ~Scene() = default;

	[[nodiscard]] bool initialize();

	[[nodiscard]] bool onRender();
	[[nodiscard]] bool onUpdate(const Milliseconds delta);

	[[nodiscard]] bool addChild(EntityId id, std::shared_ptr<ISceneNode> child);
	[[nodiscard]] bool removeChild(EntityId id);

	void pushAndSetMatrix(const Mat4x4& to_world);
	void pushAndSetMatrix(const std::shared_ptr<Mat4x4>& to_world);
	void popMatrix();
	[[nodiscard]] const Mat4x4& getTopMatrix() const;

	void setCamera(std::shared_ptr<platypus::CameraNode> camera) { this->_camera = camera; }
	[[nodiscard]] std::shared_ptr<platypus::CameraNode> getCamera() const { return this->_camera; }

	void addAlphaSceneNode(std::shared_ptr<AlphaSceneNode> node)
	{
		this->_alpha_scene_nodes.push_back(node);
	}

	[[nodiscard]] std::shared_ptr<IRenderer> renderer() const { return this->_renderer; }
	[[nodiscard]] std::shared_ptr<platypus::IResourceCache> cache() const { return this->_cache; }

	void registerEventSinks();
	void onNewRenderComponent(std::shared_ptr<IEvent> event);
	void onRemoveRenderComponent(std::shared_ptr<IEvent> event);

private:
	std::shared_ptr<SceneNode> _root {nullptr};
	std::shared_ptr<platypus::CameraNode> _camera {nullptr};
	std::shared_ptr<IRenderer> _renderer {nullptr};
	std::shared_ptr<platypus::IResourceCache> _cache {nullptr};

	std::stack<Mat4x4> _matrix_stack {};
	std::list<std::shared_ptr<AlphaSceneNode>> _alpha_scene_nodes;
	SceneEntityMap _entity_map {};

	void renderAlphaPass();
};
