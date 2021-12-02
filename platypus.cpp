#include "platypus.hpp"

#include <application_layer/window/windowFactory.hpp>
#include <renderer/rendererFactory.hpp>
#include <utilities/logging/logger.hpp>
#include <views/IView.hpp>
#include "baseGameLogic.hpp"

Platypus::Platypus(const std::string& appName)
	: _settings(), _renderer(nullptr), _window(WindowFactory().createWindow(appName)), _logic(nullptr)
{ }

const bool Platypus::initialize()
{
	this->_settings.loadSettings();

	configureLogger(this->_settings.loggingSettings());

	if (!this->_window->initialize(this->_settings.height(), this->_settings.width()))
		return false;

	this->_renderer = RendererFactory::createRenderer(this->_window.get(), this->_settings.rendererSettings());
	if (!this->_renderer->initialize(this->_settings.rendererSettings()))
		return false;

	// requires settings and renderer to be initialized
	this->_logic = this->createLogicAndView();

	return true;
}

const int Platypus::run()
{
	return this->_window->runLoop(this->getUpdateFunction(), this->getRenderFunction());
}

void Platypus::shutdown()
{ }

UpdateFunction Platypus::getUpdateFunction() const
{
	return [this](Milliseconds delta) {
		this->_logic->onUpdate(delta);
	};
}

RenderFunction Platypus::getRenderFunction() const
{
	return [this](Milliseconds now, Milliseconds delta) {
		for (const auto& view : this->_logic->views())
			view->onRender(now, delta);
	};
}
