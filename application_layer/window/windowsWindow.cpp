#include "windowsWindow.hpp"

#include <utilities/encoding.hpp>

#include <iostream>

const int16_t max_console_history = 500;

WindowsWindow::WindowsWindow(const std::string& appName)
	: _hwnd(nullptr), _hinstance(nullptr), _appName(appName), _prevTime(0),
	_max_console_history(max_console_history)
{ }

bool WindowsWindow::initialize(const uint16_t height, const uint16_t width)
{
// defined in cmake file
#if ENABLE_WINDOWS_CONSOLE
	#ifdef _DEBUG
	this->openConsole();
	#endif
#endif

	this->_hinstance = GetModuleHandleW(nullptr);
	if (this->_hinstance == nullptr)
		return false;

	const std::wstring wAppName(Encoding::toWindowsString(this->_appName));

	// register the window class
	WNDCLASSEXW wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hInstance = this->_hinstance;
	wndclass.lpfnWndProc = WindowsWindow::initialWndProc;
	wndclass.lpszClassName = wAppName.c_str();
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassExW(&wndclass))
	{
		MessageBoxW(nullptr, L"Failed to register window class.", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// determine window dimensions
	RECT dimensions = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&dimensions, WS_OVERLAPPEDWINDOW, FALSE);

	// create window
	this->_hwnd = CreateWindowW(wAppName.c_str(), wAppName.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, dimensions.right - dimensions.left,
		dimensions.bottom - dimensions.top, nullptr, nullptr, this->_hinstance, this);

	if (this->_hwnd == nullptr)
	{
		MessageBoxW(nullptr, L"Failed to create window.", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(this->_hwnd, SW_NORMAL);
	UpdateWindow(this->_hwnd);

	return true;
}

int WindowsWindow::runLoop(UpdateFunction updateFunc, RenderFunction renderFunc)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (TRUE == PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			auto now = getCurrentTime();
			auto delta = now - this->_prevTime;

			updateFunc(delta);
			renderFunc(now, delta);

			this->_prevTime = now;
		}
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowsWindow::initialWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_NCCREATE)
	{
		LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lparam);
		void* lpCreateParam = create_struct->lpCreateParams;
		WindowsWindow* this_window = reinterpret_cast<WindowsWindow*>(lpCreateParam);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_window));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::staticWndProc));
		return this_window->wndProc(hwnd, msg, wparam, lparam);
	}

	return DefWindowProcW(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK WindowsWindow::staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	auto user_data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	auto* this_window = reinterpret_cast<WindowsWindow*>(user_data);
	return this_window->wndProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK WindowsWindow::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
	}

}

void WindowsWindow::openConsole()
{
	AllocConsole();

	CONSOLE_SCREEN_BUFFER_INFOEX coninfo;
	GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = this->_max_console_history;
	SetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	std::cout.clear();
	std::cerr.clear();
	std::cin.clear();

	std::wcout.clear();
	std::wcerr.clear();
	std::wcin.clear();
}
