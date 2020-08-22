#pragma once

enum class Platform {
	Windows,
	Unknown
};

Platform detectPlatform()
{
#ifdef _WIN32
	return Platform::Windows;
#endif

	return Platform::Unknown;
}
