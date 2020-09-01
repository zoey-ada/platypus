#pragma once

#include "renderer/color.h"

#include <cstdint>

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

	const RendererType rendererType() const { return this->_rendererType; }

	const float frameRate() const { return this->_frameRate; }
	const float refreshRateMs() const { return ((1.0f / this->_frameRate) * 1000.0f); }

	const Color backgroundColor() const { return this->_backgroundColor; }

	const bool fullScreenMode() const { return this->_fullScreenMode; }

private:
	RendererType _rendererType;
	float _frameRate;
	Color _backgroundColor;
	bool _fullScreenMode;
};

class EngineSettings
{
public:
	EngineSettings();

	// TODO: load from file
	void loadSettings();

	const RendererSettings rendererSettings() const { return this->_rendererSettings; }

	const uint16_t height() const { return this->_height; }
	const uint16_t width() const { return this->_width; }

private:
	RendererSettings _rendererSettings;
	uint16_t _height;
	uint16_t _width;
};
