#pragma once

#include <memory>
#include <string>

class IWindow;

class WindowFactory
{
public:
	WindowFactory() = default;
	virtual ~WindowFactory() = default;

	[[nodiscard]] static std::unique_ptr<IWindow> createWindow(const std::string& appName);
};
