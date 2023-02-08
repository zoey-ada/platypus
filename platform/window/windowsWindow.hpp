#pragma once

#include <string>
#include <vector>

#include <Windows.h>
#undef min
#undef max

#include <input/devices/raw_input/iRawInputDevice.hpp>
#include <utilities/time/utils.hpp>

#include "iWindow.hpp"

class WindowsWindow: public IWindow
{
public:
	explicit WindowsWindow(const std::string& appName);
	WindowsWindow(WindowsWindow&) = delete;
	virtual ~WindowsWindow() = default;

	bool initialize(const platypus::RectSize& dimensions) override;
	int runLoop(UpdateFunction updateFunc, RenderFunction renderFunc,
		std::shared_ptr<IClock> clock) override;

	static LRESULT CALLBACK initialWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	[[nodiscard]] HWND hwnd() const { return this->_hwnd; }
	[[nodiscard]] HINSTANCE hinstance() const { return this->_hinstance; }

	void addRawInputDevice(std::shared_ptr<IRawInputDevice> input);

private:
	HWND _hwnd;
	HINSTANCE _hinstance;
	std::string _appName;
	Milliseconds _prevTime;
	int16_t _max_console_history;

	std::vector<std::shared_ptr<IRawInputDevice>> _raw_input_devices;

	void openConsole();
};
