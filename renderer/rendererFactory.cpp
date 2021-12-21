#include "rendererFactory.hpp"

#include <utilities/logging/logger.hpp>
#include "../application_layer/window/iWindow.hpp"

#ifdef _WIN32
	#include "directx/directXRenderer.hpp"
	#include "../application_layer/window/windowsWindow.hpp"
#endif

std::shared_ptr<IRenderer> RendererFactory::createRenderer(const IWindow* window, const platypus::RendererSettings& settings)
{
#ifdef _WIN32
	if (settings.renderer_type() == "direct_x")
	{
		const auto* windows_window = dynamic_cast<const WindowsWindow*>(window);
		if (windows_window == nullptr)
		{
			logWarning("renderer", "Attempted to use DirectX renderer on an unsupported platform.");
		}

		return std::make_shared<DirectXRenderer>(windows_window->hwnd(), windows_window->hinstance());
	}
#endif

	return nullptr;
}
