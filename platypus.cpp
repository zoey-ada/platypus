#include "platypus.h"

#include <application_layer/window/windowFactory.h>
#include <renderer/rendererFactory.h>

Platypus::Platypus(const std::string& appName)
	: _window(WindowFactory().createWindow(appName)), _renderer(nullptr)
{ }

bool Platypus::initialize()
{
	this->_settings.loadSettings();

	if (!this->_window->initialize(this->_settings.height(), this->_settings.width()))
		return false;

	this->_renderer = RendererFactory::createRenderer(this->_window.get(), this->_settings.rendererSettings());
	if (!this->_renderer->initialize(this->_settings.rendererSettings())) return false;

	return true;
}

int Platypus::run()
{
	return this->_window->runLoop(this->getUpdateFunction(), this->getRenderFunction());
}

void Platypus::shutdown()
{ }

UpdateFunction Platypus::getUpdateFunction() const
{
	return [this](Milliseconds /*delta*/) {
		// update game logic
	};
}

RenderFunction Platypus::getRenderFunction() const
{
	return [this](Milliseconds /*time*/, Milliseconds /*delta*/) {
		this->_renderer->preRender();
		// render all views
		this->_renderer->postRender();
	};
}
