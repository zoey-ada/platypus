#include "windowFactory.h"

#include "windowsWindow.h"

std::unique_ptr<IWindow> WindowFactory::createWindow(const std::string& appName) const
{
#ifdef _WIN32
	return std::make_unique<WindowsWindow>(appName);
#endif

	return std::unique_ptr<IWindow>(nullptr);
}
