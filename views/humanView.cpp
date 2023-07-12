#include "humanView.hpp"

#include <algorithm>

#include <renderer/iRenderer.hpp>
#include <renderer/scene_nodes/cameraNode.hpp>
#include <renderer/scene_nodes/meshNode.hpp>
#include <renderer/screenElementScene.hpp>
#include <resource_cache/iResourceCache.hpp>
#include <utilities/logging/logger.hpp>
#include <utilities/math/utils.hpp>

HumanView::HumanView(std::shared_ptr<IRenderer> renderer,
	std::shared_ptr<platypus::IResourceCache> cache, const platypus::Settings& settings)
	: _last_draw(0), _frametime(-1), _renderer(renderer), _cache(cache)
{
	this->_frametime = frametimeFromFrameRate(settings.renderer_settings().frame_rate());
}

HumanView::~HumanView()
{
	while (!this->_screen_elements.empty())
		this->_screen_elements.pop_front();
}

bool HumanView::initialize()
{
	auto scene = std::make_shared<ScreenElementScene>(this->_renderer, this->_cache);
	this->_scene = scene;
	if (!this->_scene->initialize())
	{
		// error log
	}

	this->_screen_elements.emplace_back(scene);

	// setup camera
	this->_camera = this->createCamera();

	if (!this->_scene->addChild(InvalidEntityId, this->_camera))
	{
		// error
		return false;
	}
	this->_scene->setCamera(this->_camera);

	return true;
}

bool HumanView::reinitialize()
{
	return true;
}

void HumanView::deinitialize()
{}

bool HumanView::onRestore()
{
	return std::all_of(this->_screen_elements.begin(), this->_screen_elements.end(),
		[](const std::shared_ptr<IScreenElement>& element) { return element->onRestore(); });
}

void HumanView::onDeviceLost()
{
	for (const auto& element : this->_screen_elements)
		element->onDeviceLost();
}

void HumanView::onUpdate(const Milliseconds delta)
{
	// process jobs
	// read input

	for (const auto& element : this->_screen_elements)
		element->onUpdate(delta);
}

template<class T>
struct SortBy_SharedPtr_Content
{
	bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const
	{
		return *lhs < *rhs;
	}
};

void HumanView::onRender(const Milliseconds now, const Milliseconds delta)
{
	if (static_cast<float>(now - this->_last_draw) >= this->_frametime)
	{
		if (this->_renderer->preRender())
		{
			this->_screen_elements.sort(SortBy_SharedPtr_Content<IScreenElement>());

			for (const auto& element : this->_screen_elements)
			{
				if (element->isVisible())
				{
					if (!element->onRender(now, delta))
						logWarning("error");  // WARNING: log rendering failure
				}
			}

			this->_last_draw = now;
		}
		else
		{
			logWarning("error");  // WARNING: log pre-render failure
		}

		if (!this->_renderer->postRender())
			logWarning("error");  // WARNING: log post-render failure
	}
}

void HumanView::onAttach(const ViewId /*viewId*/, const EntityId /*entityId*/)
{
	// ! why?
	// this->_viewId = viewId;
	// this->_entityId = entityId;
}
