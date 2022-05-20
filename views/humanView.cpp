#include "humanView.hpp"

#include <algorithm>

#include <renderer/iRenderer.hpp>
#include <renderer/scene_nodes/cameraNode.hpp>
#include <renderer/scene_nodes/meshNode.hpp>
#include <renderer/screenElementScene.hpp>
#include <resource_cache/resourceCache.hpp>
#include <utilities/logging/logger.hpp>
#include <utilities/time.hpp>

HumanView::HumanView(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ResourceCache> cache,
	const platypus::Settings& settings)
	: _last_draw(0), _frametime(-1), _renderer(renderer), _cache(cache)
{
	this->_frametime = frametimeFromFrameRate(settings.renderer_settings().frame_rate());

	auto scene = std::make_shared<ScreenElementScene>(renderer, cache);
	this->_scene = scene;
	this->_screen_elements.push_back(scene);

	// setup camera
	Frustum frustum;
	frustum.initialize(3.1415f / 4.0f, 1.0f, 1.0f, 100.0f);
	this->_camera = std::make_shared<CameraNode>(Mat4x4::identity(), frustum);

	bool success = this->_scene->addChild(InvalidEntityId, this->_camera);
	this->_scene->setCamera(this->_camera);

	auto mat = Mat4x4::identity();
	//mat->scale(5, 5, 5);
	mat->rotateY(3.1415f / 4);
	mat->setPosition(Vec3(0, 0, 5));
	auto test = std::make_shared<MeshNode>("test_mesh", "assets.zip/cube.obj", renderer, cache,
		InvalidEntityId, RenderPass::Entity, mat, Color::black);

	bool contains = this->_camera->getFrustum().contains(Vec3(0, 0, 0));
	if (!contains) {}

	success = this->_scene->addChild(InvalidEntityId, test);
}

HumanView::~HumanView()
{
	while (!this->_screen_elements.empty())
		this->_screen_elements.pop_front();
}

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
