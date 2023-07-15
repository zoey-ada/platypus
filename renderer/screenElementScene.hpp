#pragma once

#include <views/iScreenElement.hpp>

#include "scene.hpp"

namespace platypus
{
class IResourceCache;
};

class ScreenElementScene: public IScreenElement, public Scene
{
public:
	explicit ScreenElementScene(std::shared_ptr<IRenderer> renderer,
		std::shared_ptr<platypus::IResourceCache> cache)
		: Scene(renderer, cache)
	{}
	virtual ~ScreenElementScene() = default;

	// IScreenElement
	void onUpdate(Milliseconds delta) override;
	bool onRestore() override { return true; }
	bool onRender(Milliseconds /*now*/, Milliseconds /*delta*/) override
	{
		return Scene::onRender();
	}
	void onDeviceLost() override {}
	[[nodiscard]] int zOrder() const override { return 50; }
	void setZOrder(const int /*zOrder*/) override {}
	[[nodiscard]] bool isVisible() const override { return true; }
	void setVisibility(bool const /*visible*/) override {}
};
