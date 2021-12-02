#pragma once

#include "IWindow.hpp"

#include <utilities/time.hpp>

#include <Windows.h>
#include <string>

class WindowsWindow : public IWindow
{
public:
	explicit WindowsWindow(const std::string& appName);
	virtual ~WindowsWindow() = default;

	bool initialize(const uint16_t height, const uint16_t width) override;
	int runLoop(UpdateFunction updateFunc, RenderFunction renderFunc) override;

	static LRESULT CALLBACK initialWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND hwnd() const { return this->_hwnd; }
	HINSTANCE hinstance() const { return this->_hinstance; }

private:
	WindowsWindow(WindowsWindow&) = delete;
	void openConsole();

	HWND _hwnd;
	HINSTANCE _hinstance;
	std::string _appName;
	Milliseconds _prevTime;
};
