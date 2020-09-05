#pragma once

#include <string_view>

class ILogger
{
public:
	virtual ~ILogger() = default;

	virtual void log(std::string_view message) = 0;
	virtual void log(std::string_view message, std::string_view channel) = 0;

	virtual const bool supportsColor() const = 0;
};
