#pragma once

#include <renderer/color.hpp>
#include <utilities/time.hpp>

#include <cstdint>
#include <list>
#include <string>

enum class RendererType
{
	DirectX,
	OpenGl
};

class RendererSettings
{
public:
	RendererSettings();

	void loadSettings();

	[[nodiscard]] RendererType rendererType() const { return this->_rendererType; }

	[[nodiscard]] float frameRate() const { return this->_frameRate; }
	[[nodiscard]] float refreshRateMs() const { return ((1.0f / this->_frameRate) * static_cast<float>(milliseconds_in_second)); }

	[[nodiscard]] Color backgroundColor() const { return this->_backgroundColor; }

	[[nodiscard]] bool fullScreenMode() const { return this->_fullScreenMode; }

private:
	RendererType _rendererType;
	float _frameRate;
	Color _backgroundColor;
	bool _fullScreenMode;
};

struct LoggerSettings
{
	std::string type;
	std::string level;
	std::string fileRoot;
	bool useSingleFile;
};

class LoggingSettings
{
public:
	LoggingSettings();

	void loadSettings();

	[[nodiscard]] std::list<LoggerSettings> loggers() const { return this->_loggers; }

private:
	std::list<LoggerSettings> _loggers;
};

class EngineSettings
{
public:
	EngineSettings();

	// TODO: load from file
	void loadSettings();

	[[nodiscard]] RendererSettings rendererSettings() const { return this->_rendererSettings; }
	[[nodiscard]] LoggingSettings loggingSettings() const { return this->_loggingSettings; }

	[[nodiscard]] uint16_t height() const { return this->_height; }
	[[nodiscard]] uint16_t width() const { return this->_width; }

private:
	RendererSettings _rendererSettings;
	LoggingSettings _loggingSettings;
	uint16_t _height;
	uint16_t _width;
};
