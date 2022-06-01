#include "platformFactory.hpp"

#ifdef _WIN32
	#include "windowsPlatform.hpp"
#endif

std::shared_ptr<IPlatform> PlatformFactory::getPlatform(const char* application_name)
{
#ifdef _WIN32
	return std::make_shared<WindowsPlatform>(application_name);
#else
	return std::shared_ptr<IPlatform>(nullptr);
#endif
}
