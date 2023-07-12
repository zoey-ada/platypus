#pragma once

#include <list>
#include <memory>

#include <platypus_proto/settings.hpp>

#include "iScreenElement.hpp"
#include "iView.hpp"

class IRenderer;
class Scene;
using ScreenElementList = std::list<std::shared_ptr<IScreenElement>>;

namespace platypus
{
class CameraNode;
class IResourceCache;
};

class HumanView: public IView
{
public:
	HumanView(std::shared_ptr<IRenderer> renderer, std::shared_ptr<platypus::IResourceCache> cache,
		const platypus::Settings& settings);
	virtual ~HumanView();

	bool initialize() override;
	bool reinitialize() override;
	void deinitialize() override;

	bool onRestore() override;
	void onDeviceLost() override;

	void onUpdate(const Milliseconds delta) override;
	void onRender(const Milliseconds now, const Milliseconds delta) override;

	void onAttach(const ViewId viewId, const EntityId entityId) override;

private:
	Milliseconds _last_draw;
	Milliseconds _frametime;
	std::shared_ptr<IRenderer> _renderer;
	std::shared_ptr<platypus::IResourceCache> _cache;
	std::shared_ptr<Scene> _scene;
	std::shared_ptr<platypus::CameraNode> _camera;
	ScreenElementList _screen_elements;

	virtual std::shared_ptr<platypus::CameraNode> createCamera() = 0;
};
