#include "rendererFactory.hpp"

#include <utilities/logging/logger.hpp>
#include "../application_layer/window/IWindow.hpp"

#ifdef _WIN32
	#include "directXRenderer.hpp"
	#include "../application_layer/window/windowsWindow.hpp"
#endif

std::shared_ptr<IRenderer> RendererFactory::createRenderer(const IWindow* window, const RendererSettings& settings)
{
#ifdef _WIN32
	if (settings.rendererType() == RendererType::DirectX)
	{
		auto windowsWindow = dynamic_cast<const WindowsWindow*>(window);
		if (windowsWindow)
		{
			return std::make_shared<DirectXRenderer>(windowsWindow->hwnd(), windowsWindow->hinstance());
		}
		else
		{
			// WARNING: engine configured to directX on non-Windows system
		}
	}
#endif

	return nullptr;
}
