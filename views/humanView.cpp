#include "humanView.hpp"

#include "../renderer/IRenderer.hpp"
#include "../utilities/time.hpp"
#include <algorithm>

HumanView::HumanView(const std::shared_ptr<IRenderer>& renderer, const EngineSettings& settings)
	: _lastDraw(0), _refreshRate(settings.rendererSettings().refreshRateMs()),
	  _renderer(renderer), _screenElements()
{ }

HumanView::~HumanView()
{
	while (!this->_screenElements.empty())
		this->_screenElements.pop_front();
}

const bool HumanView::onRestore()
{
	return std::all_of(this->_screenElements.begin(), this->_screenElements.end(),
		[](const std::shared_ptr<IScreenElement>& element) { return element->onRestore(); });
}

void HumanView::onDeviceLost()
{
	for (const auto& element : this->_screenElements)
		element->onDeviceLost();
}

void HumanView::onUpdate(const Milliseconds delta)
{
	// process jobs
	// read input

	for (const auto& element : this->_screenElements)
		element->onUpdate(delta);
}

template <class T>
struct SortBy_SharedPtr_Content
{
	bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const
	{
		return *lhs < *rhs;
	}
};

void HumanView::onRender(const Milliseconds now, const Milliseconds delta)
{
	if ((now - this->_lastDraw) > this->_refreshRate)
	{
		if (this->_renderer->preRender())
		{
			this->_screenElements.sort(SortBy_SharedPtr_Content<IScreenElement>());

			for (const auto& element : this->_screenElements)
			{
				if (element->isVisible())
				{
					if (!element->onRender(now, delta))
						printf("error");  // WARNING: log rendering failure
				}
			}

			this->_lastDraw = now;
		}
		else
		{
			printf("error");  // WARNING: log pre-render failure
		}

		if (!this->_renderer->postRender())
			printf("error");  // WARNING: log post-render failure
	}
}

void HumanView::onAttach(const ViewId /*viewId*/, const EntityId /*entityId*/)
{
	// ! why?
	// this->_viewId = viewId;
	// this->_entityId = entityId;
}
