#pragma once

#include <utilities/time.h>
#include <cstdint>
#include <functional>

typedef std::function<void(Milliseconds delta)> UpdateFunction;
typedef std::function<void(Milliseconds time, Milliseconds delta)> RenderFunction;

class IWindow {
public:
	IWindow() = default;
	virtual ~IWindow() = default;

	virtual bool initialize(const uint16_t height, const uint16_t width) = 0;
	virtual int runLoop(UpdateFunction updateFunc, RenderFunction renderFunc) = 0;
};
