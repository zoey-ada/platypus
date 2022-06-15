#pragma once

#include <memory>
#include <vector>

#include <platypus_proto/common.hpp>

#include <utilities/common/ptExtent.hpp>

class IInputDevice;
class IWindow;

class IPlatform
{
public:
	virtual ~IPlatform() = default;

	virtual bool initialize() = 0;
	virtual void deinitialize() = 0;

	virtual std::shared_ptr<IWindow> createWindow(const platypus::RectSize& size) = 0;

	virtual std::vector<std::shared_ptr<IInputDevice>> queryInputDevices() = 0;

	virtual PtExtent getPixelDensity() = 0;
};
