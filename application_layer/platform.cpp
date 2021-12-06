#include "platform.hpp"

Platform detectPlatform()
{
#ifdef _WIN32
	return Platform::Windows;
#else
	return Platform::Unknown;
#endif
}
