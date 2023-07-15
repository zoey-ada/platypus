#include "windowsPlatform.hpp"

#include <input/devices/raw_input/rawInputKeyboardDevice.hpp>

#include "window/windowsWindow.hpp"

WindowsPlatform::WindowsPlatform(std::string application_name)
	: _application_name(std::move(application_name))
{}

bool WindowsPlatform::initialize()
{
	return true;
}

void WindowsPlatform::deinitialize()
{}

std::shared_ptr<IWindow> WindowsPlatform::createWindow(const platypus::RectSize& size)
{
	auto window = std::make_shared<WindowsWindow>(this->_application_name);
	window->initialize(size);
	this->_hwnd = window->hwnd();
	return window;
}

std::vector<std::shared_ptr<IInputDevice>> WindowsPlatform::queryInputDevices()
{
	assert(this->_hwnd != nullptr);
	std::vector<std::shared_ptr<IInputDevice>> devices;
	devices.push_back(std::make_shared<RawInputKeyboardDevice>(this->_hwnd));
	return devices;
}

PtExtent WindowsPlatform::getPixelDensity()
{
	SetProcessDPIAware();  // true
	HDC screen = GetDC(NULL);
	uint32_t hPixelsPerInch = static_cast<uint32_t>(GetDeviceCaps(screen, LOGPIXELSX));
	uint32_t vPixelsPerInch = static_cast<uint32_t>(GetDeviceCaps(screen, LOGPIXELSY));
	ReleaseDC(NULL, screen);
	return {hPixelsPerInch, vPixelsPerInch};
}
