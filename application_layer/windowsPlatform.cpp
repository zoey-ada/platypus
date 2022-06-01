#include "windowsPlatform.hpp"

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
