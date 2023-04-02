#pragma once

#include <string>

class ILoggingSystem
{
public:
	virtual ~ILoggingSystem() = default;

	virtual void debug(const std::string_view& message) = 0;
	virtual void debug(const std::string_view& channel, const std::string_view& message) = 0;

	virtual void verbose(const std::string_view& message) = 0;
	virtual void verbose(const std::string_view& channel, const std::string_view& message) = 0;

	virtual void info(const std::string_view& message) = 0;
	virtual void info(const std::string_view& channel, const std::string_view& message) = 0;

	virtual void warning(const std::string_view& message) = 0;
	virtual void warning(const std::string_view& channel, const std::string_view& message) = 0;

	virtual void error(const std::string_view& message) = 0;
	virtual void error(const std::string_view& channel, const std::string_view& message) = 0;
};
