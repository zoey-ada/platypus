#include "platypus.hpp"

#include <application_layer/utils.hpp>
#include <application_layer/window/windowFactory.hpp>
#include <platypus_proto/util.hpp>
#include <renderer/rendererFactory.hpp>
#include <resource_cache/resourceCache.hpp>
#include <utilities/logging/logger.hpp>
#include <views/iView.hpp>

#include "baseGameLogic.hpp"

Platypus::Platypus(const std::string& appName)
	: _renderer(nullptr), _window(WindowFactory::createWindow(appName)), _logic(nullptr)
{}

bool Platypus::initialize()
{
	this->LoadSettings();

	configureLogger(this->_settings.loggers());

	if (!this->_window->initialize(this->_settings.renderer_settings().resolution()))
		return false;

	this->_renderer =
		RendererFactory::createRenderer(this->_window.get(), this->_settings.renderer_settings());

	this->_cache = std::make_shared<ResourceCache>(48);
	if (!this->_cache->initialize(this->_renderer))
		return false;

	if (!this->_renderer->initialize(this->_settings.renderer_settings(), this->_cache))
		return false;

	// requires settings and renderer to be initialized
	this->_logic = this->createLogicAndView();

	return true;
}

int Platypus::run()
{
	return this->_window->runLoop(this->getUpdateFunction(), this->getRenderFunction());
}

void Platypus::shutdown()
{}

UpdateFunction Platypus::getUpdateFunction() const
{
	return [this](Milliseconds delta) { this->_logic->onUpdate(delta); };
}

RenderFunction Platypus::getRenderFunction() const
{
	return [this](Milliseconds now, Milliseconds delta) {
		for (const auto& view : this->_logic->views())
			view->onRender(now, delta);
	};
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
