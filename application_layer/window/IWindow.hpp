#pragma once

#include <utilities/time.hpp>
#include <cstdint>
#include <functional>

using UpdateFunction = std::function<void(Milliseconds delta)>;
using RenderFunction = std::function<void(Milliseconds time, Milliseconds delta)>;

class IWindow {
public:
	IWindow() = default;
	virtual ~IWindow() = default;

	[[nodiscard]] virtual bool initialize(uint16_t height, uint16_t width) = 0;
	virtual int runLoop(UpdateFunction updateFunc, RenderFunction renderFunc) = 0;
};
