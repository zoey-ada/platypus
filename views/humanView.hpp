#pragma once

#include "iView.hpp"

#include "iScreenElement.hpp"

#include <platypus_proto/settings.hpp>

#include <list>
#include <memory>

class IRenderer;
class Scene;
using ScreenElementList = std::list<std::shared_ptr<IScreenElement>>;

class HumanView : public IView
{
public:
	HumanView(std::shared_ptr<IRenderer> renderer, const platypus::Settings& settings);
	virtual ~HumanView();

	bool onRestore() override;
	void onDeviceLost() override;

	void onUpdate(const Milliseconds delta) override;
	void onRender(const Milliseconds now, const Milliseconds delta) override;

	void onAttach(const ViewId viewId, const EntityId entityId) override;

private:
	Milliseconds _last_draw;
	float _frametime;
	std::shared_ptr<IRenderer> _renderer;
	std::shared_ptr<Scene> _scene;
	ScreenElementList _screen_elements;
};
