#include "settings.h"

RendererSettings::RendererSettings()
	: _rendererType(RendererType::DirectX), _frameRate(60.0), _backgroundColor(Color::cyan), _fullScreenMode(false)
{ }

void RendererSettings::loadSettings()
{ }

EngineSettings::EngineSettings()
	: _rendererSettings(), _height(720), _width(1280)
{ }

void EngineSettings::loadSettings()
{
	this->_rendererSettings.loadSettings();
}
