#include "settings.hpp"

RendererSettings::RendererSettings()
	: _rendererType(RendererType::DirectX), _frameRate(60.0), _backgroundColor(Color::cyan), _fullScreenMode(false)
{ }

void RendererSettings::loadSettings()
{ }

LoggingSettings::LoggingSettings()
	: _loggers(std::list<LoggerSettings>() = {
				   { "console", "debug", "", false },
				   { "file", "debug", "./logs", false } })
{ }

void LoggingSettings::loadSettings()
{ }

EngineSettings::EngineSettings()
	: _rendererSettings(), _loggingSettings(), _height(720), _width(1280)
{ }

void EngineSettings::loadSettings()
{
	this->_rendererSettings.loadSettings();
	this->_loggingSettings.loadSettings();
}
