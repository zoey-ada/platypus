#pragma once

#include <list>

#include <utilities/logging/iLoggingSystem.hpp>

class MockLoggingSystem: public ILoggingSystem
{
public:
	virtual ~MockLoggingSystem() = default;

	bool initialize() override { return true; }
	void deinitialize() override {}

	void update(Milliseconds /*delta*/) override {}

	void pause() override { this->_is_paused = true; }
	void resume() override { this->_is_paused = false; }

	void debug(const std::string_view& message) override
	{
		this->_debug_logs.push_back(std::string(message));
	}
	void debug(const std::string_view& channel, const std::string_view& message) override
	{
		this->_debug_logs.push_back(std::string(message));
	}

	void verbose(const std::string_view& message) override
	{
		this->_verbose_logs.push_back(std::string(message));
	}
	void verbose(const std::string_view& channel, const std::string_view& message) override
	{
		this->_verbose_logs.push_back(std::string(message));
	}

	void info(const std::string_view& message) override
	{
		this->_info_logs.push_back(std::string(message));
	}
	void info(const std::string_view& channel, const std::string_view& message) override
	{
		this->_info_logs.push_back(std::string(message));
	}

	void warning(const std::string_view& message) override
	{
		this->_warning_logs.push_back(std::string(message));
	}
	void warning(const std::string_view& channel, const std::string_view& message) override
	{
		this->_warning_logs.push_back(std::string(message));
	}

	void error(const std::string_view& message) override
	{
		this->_error_logs.push_back(std::string(message));
	}
	void error(const std::string_view& channel, const std::string_view& message) override
	{
		this->_error_logs.push_back(std::string(message));
	}

	void flush_logs()
	{
		this->_debug_logs.clear();
		this->_verbose_logs.clear();
		this->_info_logs.clear();
		this->_warning_logs.clear();
		this->_error_logs.clear();
	}

	std::list<std::string> _debug_logs;
	std::list<std::string> _verbose_logs;
	std::list<std::string> _info_logs;
	std::list<std::string> _warning_logs;
	std::list<std::string> _error_logs;

	bool _is_paused {false};
};
