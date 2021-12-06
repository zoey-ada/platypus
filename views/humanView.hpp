#pragma once

#include "IView.hpp"

#include "IScreenElement.hpp"

#include "../settings.hpp"

#include <list>
#include <memory>

class IRenderer;
using ScreenElementList = std::list<std::shared_ptr<IScreenElement>>;

class HumanView : public IView
{
public:
	HumanView(std::shared_ptr<IRenderer> renderer, const EngineSettings& settings);
	virtual ~HumanView();

	bool onRestore() override;
	void onDeviceLost() override;

	void onUpdate(const Milliseconds delta) override;
	void onRender(const Milliseconds now, const Milliseconds delta) override;

	void onAttach(const ViewId viewId, const EntityId entityId) override;

private:
	Milliseconds _lastDraw;
	float _refreshRate;
	std::shared_ptr<IRenderer> _renderer;
	ScreenElementList _screenElements;
};
