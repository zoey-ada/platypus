#pragma once

#include "scene.hpp"
#include <views/iScreenElement.hpp>

class ScreenElementScene: public IScreenElement, public Scene
{
public:
	explicit ScreenElementScene(std::shared_ptr<IRenderer> renderer) : Scene(renderer) {}
	virtual ~ScreenElementScene() = default;

	// IScreenElement
	void onUpdate(Milliseconds delta) override { Scene::onUpdate(delta); }
	bool onRestore() override { return true; }
	bool onRender(Milliseconds /*now*/, Milliseconds /*delta*/) override { return Scene::onRender(); }
	void onDeviceLost() override {}
	[[nodiscard]] int zOrder() const override { return 0; }
	void setZOrder(const int /*zOrder*/) override { }
	[[nodiscard]] bool isVisible() const override { return true; }
	void setVisibility(bool const /*visible*/) override { }
};
