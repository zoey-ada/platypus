#include <cstdint>

#include <Windows.h>
#undef min
#undef max

#include <utilities/encoding.hpp>

#include "utils.hpp"

std::string currentDirectory()
{
	auto size = GetCurrentDirectory(0, nullptr);
	wchar_t* buffer = new (std::nothrow) wchar_t[size];
	auto size2 = GetCurrentDirectoryW(size, buffer);

	if (size2 != (size - 1))
	{
		delete[] buffer;
		throw std::exception("failed to get current directory");
	}

	auto current_dir = Encoding::fromWindowsString(std::wstring(buffer));
	delete[] buffer;
	return current_dir;
}
