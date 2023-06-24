#pragma once

#include <string>

#include "../common/iSystem.hpp"

class ILoggingSystem: public ISystem
{
public:
	virtual ~ILoggingSystem() = default;

	virtual bool initialize() = 0;
	virtual void deinitialize() = 0;

	virtual void update(Milliseconds delta) = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

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
