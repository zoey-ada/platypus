#include "windowFactory.hpp"

#ifdef _WIN32
	#include "windowsWindow.hpp"
#endif

std::unique_ptr<IWindow> WindowFactory::createWindow(const std::string& appName)
{
#ifdef _WIN32
	return std::make_unique<WindowsWindow>(appName);
#else
	return std::unique_ptr<IWindow>(nullptr);
#endif
}
