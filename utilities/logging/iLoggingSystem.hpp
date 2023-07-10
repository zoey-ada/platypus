#pragma once

#include <string>

#include "../common/iSystem.hpp"

#ifdef _WIN32
using HRESULT = long;
#endif

class ILoggingSystem: public ISystem
{
public:
	virtual ~ILoggingSystem() = default;

	virtual bool initialize() = 0;
	virtual void deinitialize() = 0;

	virtual void update(const Milliseconds delta) = 0;

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

#ifdef _WIN32

	virtual void debug(const std::string_view& message, const HRESULT hr) = 0;
	virtual void debug(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) = 0;

	virtual void verbose(const std::string_view& message, const HRESULT hr) = 0;
	virtual void verbose(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) = 0;

	virtual void info(const std::string_view& message, const HRESULT hr) = 0;
	virtual void info(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) = 0;

	virtual void warning(const std::string_view& message, const HRESULT hr) = 0;
	virtual void warning(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) = 0;

	virtual void error(const std::string_view& message, const HRESULT hr) = 0;
	virtual void error(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) = 0;

#endif
};
