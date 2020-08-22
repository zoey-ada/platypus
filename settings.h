#pragma once

#include <cstdint>

class EngineSettings
{
public:
	EngineSettings();

	// TODO: load from file
	void loadSettings();

	const float frameRate() const { return this->_frameRate; }
	const float refreshRateMs() const { return ((1.0f / this->_frameRate) * 1000.0f); }

	const uint16_t height() const { return this->_height; }
	const uint16_t width() const { return this->_width; }

private:
	float _frameRate;
	uint16_t _height;
	uint16_t _width;
};
