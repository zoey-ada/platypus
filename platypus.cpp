#include "platypus.hpp"

#include <platypus_proto/util.hpp>

#include <audio/core_audio/coreAudioSystem.hpp>
#include <events/eventManager.hpp>
#include <input/inputManager.hpp>
#include <physics/physicsSystem.hpp>
#include <platform/iPlatform.hpp>
#include <platform/platformFactory.hpp>
#include <platform/utils.hpp>
#include <platform/window/iWindow.hpp>
#include <renderer/rendererFactory.hpp>
#include <resource_cache/loaders/audioLoader.hpp>
#include <resource_cache/loaders/meshLoader.hpp>
#include <resource_cache/loaders/pixelShaderLoader.hpp>
#include <resource_cache/loaders/textureLoader.hpp>
#include <resource_cache/loaders/vertexShaderLoader.hpp>
#include <resource_cache/resourceCache.hpp>
#include <resource_cache/stores/resourceStoreFactory.hpp>
#include <utilities/logging/logger.hpp>
#include <utilities/logging/loggingSystem.hpp>
#include <utilities/time/systemClock.hpp>
#include <views/iView.hpp>

#include "baseGameLogic.hpp"
#include "serviceProvider.hpp"

Platypus::Platypus(const char* appName)
	: Platypus::Platypus(appName, std::make_shared<SystemClock>())
{}

Platypus::Platypus(const char* appName, std::shared_ptr<IClock> clock)
	: _clock(clock), _platform(PlatformFactory::getPlatform(appName))
{}

bool Platypus::initialize()
{
	this->LoadSettings();

	// TODO: remove
	configureLogger(this->_settings.loggers(), this->_clock);

	this->_logging = std::make_shared<LoggingSystem>(this->_settings.loggers(), this->_clock);
	ServiceProvider::registerLoggingSystem(this->_logging);

	if (this->_platform == nullptr || !this->_platform->initialize())
	{
		// log error
		return false;
	}
	ServiceProvider::registerPlatform(this->_platform);

	this->_window = this->_platform->createWindow(this->_settings.renderer_settings().resolution());
	if (this->_window == nullptr)
		return false;
	ServiceProvider::registerWindow(this->_window);

	this->_renderer =
		RendererFactory::createRenderer(this->_window.get(), this->_settings.renderer_settings());

	assert(this->createCache());

	if (!this->_renderer->initialize(this->_settings.renderer_settings(), this->_cache))
		return false;
	// this->_renderer->enableDebugMode();
	ServiceProvider::registerRenderer(this->_renderer);

	this->_event_manager = std::make_shared<EventManager>(
		this->_settings.general_settings().event_manager_queue_count());
	ServiceProvider::registerEventManager(this->_event_manager);

	// requires settings and renderer to be initialized
	this->_logic = this->createLogicAndView();

	this->_input_manager = std::make_shared<InputManager>();
	this->_input_manager->initialize();

	this->_physics = std::make_shared<PhysicsSystem>();
	this->_physics->initialize();
	ServiceProvider::registerPhysicsSystem(this->_physics);

	this->_audio = std::make_shared<CoreAudioSystem>();
	this->_audio->initialize(this->_cache);
	ServiceProvider::registerAudioSystem(this->_audio);

	return true;
}

int Platypus::run()
{
	return this->_window->runLoop(this->getUpdateFunction(), this->getRenderFunction(),
		this->_clock);
}

void Platypus::deinitialize()
{
	this->_audio->deinitialize();
	this->_physics->deinitialize();
	this->_renderer->deinitialize();
	this->_cache->flush();
	ServiceProvider::unregisterAllServices();
}

void Platypus::registerResourceLoaders()
{
	this->_cache->registerLoader(
		std::make_shared<VertexShaderLoader>(this->_cache, this->_renderer));

	this->_cache->registerLoader(
		std::make_shared<PixelShaderLoader>(this->_cache, this->_renderer));

	this->_cache->registerLoader(std::make_shared<TextureLoader>(this->_cache, this->_renderer));
	this->_cache->registerLoader(std::make_shared<MeshLoader>(this->_cache, this->_renderer));
	this->_cache->registerLoader(std::make_shared<AudioLoader>(this->_cache));
}

UpdateFunction Platypus::getUpdateFunction() const
{
	return [this](Milliseconds now, Milliseconds delta) {
		this->_physics->update(delta);
		this->_input_manager->readInput(now);
		this->_logic->onUpdate(delta);
		this->_audio->update(delta);
	};
}

RenderFunction Platypus::getRenderFunction() const
{
	return [this](Milliseconds now, Milliseconds delta) {
		for (const auto& view : this->_logic->views())
			view->onRender(now, delta);
	};
}

bool Platypus::createCache()
{
	try
	{
		auto cache_settings = this->_settings.resource_cache_settings();
		auto cache_size = cache_settings.cache_size_in_mb();

		this->_cache = std::make_shared<ResourceCache>(cache_size, this->_logging);

		std::list<std::shared_ptr<IResourceStore>> res_stores;
		for (auto& res_store : cache_settings.resource_stores())
		{
			auto store = ResourceStoreFactory::createResourceStore(res_store);
			res_stores.push_back(store);
		}
		if (!this->_cache->initialize(res_stores))
			return false;

		this->registerResourceLoaders();
		return true;
	}
	catch (const std::exception& e)
	{
		this->_logging->error(
			"exception occurred while trying to create resource cache: \n" + std::string(e.what()));
		return false;
	}
}

void Platypus::LoadSettings()
{
	try
	{
		this->_settings = platypus::readProtoFile<platypus::Settings>("./engine_settings.json");
	}
	catch (const platypus::ProtoFileParsingError& e)
	{
		std::cerr << "Unable to load settings file. Falling back to default settings." << std::endl
				  << "Error loading '" << e._filename << "' >> " << e.what() << std::endl;

		this->_settings = this->DefaultSettings();
	}
}

platypus::Settings Platypus::DefaultSettings()
{
	platypus::Settings default_settings;

	auto* renderer_settings = default_settings.mutable_renderer_settings();
	renderer_settings->set_background_color("#FFFFFF");
	renderer_settings->set_full_screen(false);
	renderer_settings->set_renderer_type("direct_x");

	auto* frame_rate = renderer_settings->mutable_frame_rate();
	frame_rate->set_numerator(60);
	frame_rate->set_denominator(1);
	frame_rate = nullptr;

	auto* resolution = renderer_settings->mutable_resolution();
	resolution->set_height(720);
	resolution->set_width(1280);
	resolution = nullptr;

	renderer_settings = nullptr;

	auto* logger = default_settings.add_loggers();
	logger->set_level("debug");
	logger->set_type("console");

	logger = default_settings.add_loggers();
	logger->set_level("debug");
	logger->set_type("file");

	auto* file_logger = logger->mutable_file_logger_settings();
	file_logger->set_root_dir("./logs");
	file_logger->set_use_single_file(false);
	file_logger = nullptr;

	logger = nullptr;

	return default_settings;
}
