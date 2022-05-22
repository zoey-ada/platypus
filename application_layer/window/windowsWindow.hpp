#pragma once

#include "iWindow.hpp"

#include <utilities/time.hpp>

#include <Windows.h>
#include <string>

class WindowsWindow : public IWindow
{
public:
	explicit WindowsWindow(const std::string& appName);
	WindowsWindow(WindowsWindow&) = delete;
	virtual ~WindowsWindow() = default;

	bool initialize(const platypus::RectSize& dimensions) override;
	int runLoop(UpdateFunction updateFunc, RenderFunction renderFunc) override;

	static LRESULT CALLBACK initialWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	[[nodiscard]] HWND hwnd() const { return this->_hwnd; }
	[[nodiscard]] HINSTANCE hinstance() const { return this->_hinstance; }

private:
	void openConsole();

	HWND _hwnd;
	HINSTANCE _hinstance;
	std::string _appName;
	Milliseconds _prevTime;
	int16_t _max_console_history;
};
