#pragma once

#include <string>

#include <Windows.h>

#include "iPlatform.hpp"

class WindowsPlatform: public IPlatform
{
public:
	explicit WindowsPlatform(std::string application_name);
	virtual ~WindowsPlatform() = default;

	bool initialize() override;
	void deinitialize() override;

	std::shared_ptr<IWindow> createWindow(const platypus::RectSize& size) override;

	std::vector<std::shared_ptr<IInputDevice>> queryInputDevices() override;

	PtExtent getPixelDensity() override;

private:
	std::string _application_name;
	HWND _hwnd {nullptr};
};
