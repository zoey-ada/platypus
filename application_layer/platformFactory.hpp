#pragma once

#include <memory>

class IPlatform;

class PlatformFactory
{
public:
	static std::shared_ptr<IPlatform> getPlatform(const char* application_name);
};
