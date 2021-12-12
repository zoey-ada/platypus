#pragma once

#include <platypus_proto/settings.hpp>
#include <utilities/time.hpp>

#include <cstdint>
#include <functional>

using UpdateFunction = std::function<void(Milliseconds delta)>;
using RenderFunction = std::function<void(Milliseconds time, Milliseconds delta)>;

class IWindow {
public:
	IWindow() = default;
	virtual ~IWindow() = default;

	[[nodiscard]] virtual bool initialize(const platypus::RectSize& dimensions) = 0;
	virtual int runLoop(UpdateFunction updateFunc, RenderFunction renderFunc) = 0;
};
