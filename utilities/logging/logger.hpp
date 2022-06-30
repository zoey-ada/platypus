#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <mutex>

#include <platypus_proto/settings.hpp>

#include "../time/SystemClock.hpp"
#include "consoleLogger.hpp"
#include "fileLogger.hpp"
#include "verbosityDecorator.hpp"

using LoggerSettingsList = google::protobuf::RepeatedPtrField<platypus::LoggerSettings>;

class Logger
{
public:
	virtual ~Logger() = default;

	static void debug(std::string_view message)
	{
		Logger::forEachLogger([&](auto l) { l->debug(message); });
	}
	static void debug(std::string_view message, std::string_view channel)
	{
		Logger::forEachLogger([&](auto l) { l->debug(message, channel); });
	}
	static void verbose(std::string_view message)
	{
		Logger::forEachLogger([&](auto l) { l->verbose(message); });
	}
	static void verbose(std::string_view message, std::string_view channel)
	{
		Logger::forEachLogger([&](auto l) { l->verbose(message, channel); });
	}
	static void info(std::string_view message)
	{
		Logger::forEachLogger([&](auto l) { l->info(message); });
	}
	static void info(std::string_view message, std::string_view channel)
	{
		Logger::forEachLogger([&](auto l) { l->info(message, channel); });
	}
	static void warning(std::string_view message)
	{
		Logger::forEachLogger([&](auto l) { l->warning(message); });
	}
	static void warning(std::string_view message, std::string_view channel)
	{
		Logger::forEachLogger([&](auto l) { l->warning(message, channel); });
	}
	static void error(std::string_view message)
	{
		Logger::forEachLogger([&](auto l) { l->error(message); });
	}
	static void error(std::string_view message, std::string_view channel)
	{
		Logger::forEachLogger([&](auto l) { l->error(message, channel); });
	}

	static void configureLogger(const LoggerSettingsList& settings, std::shared_ptr<IClock> clock)
	{
		Logger::_mutex.lock();

		Logger::_settings = settings;
		Logger::_clock = clock;
		Logger::_logger = std::unique_ptr<Logger>(new (std::nothrow) Logger);

		for (const auto& l : settings)
		{
			std::shared_ptr<VerbosityLogger> logger = nullptr;

			if (l.type() == "console")
				logger = std::make_shared<VerbosityLogger>(
					std::make_shared<ConsoleLogger>(Logger::_clock));
			else if (l.type() == "file")
				logger = std::make_shared<VerbosityLogger>(
					std::make_shared<FileLogger>(l.file_logger_settings(), Logger::_clock));

			if (l.level() == "debug")
				logger->setVerbosity(VerbosityLevel::Debug);
			else if (l.level() == "verbose")
				logger->setVerbosity(VerbosityLevel::Verbose);
			else if (l.level() == "info")
				logger->setVerbosity(VerbosityLevel::Info);
			else if (l.level() == "warning")
				logger->setVerbosity(VerbosityLevel::Warning);
			else if (l.level() == "error")
				logger->setVerbosity(VerbosityLevel::Error);

			Logger::_logger->attachLogger(logger);
		}

		Logger::_mutex.unlock();
	}

	void attachLogger(const std::shared_ptr<VerbosityLogger>& logger)
	{
		this->_attachedLoggers.push_back(logger);
	}

private:
	static std::shared_ptr<IClock> _clock;
	static std::mutex _mutex;
	static std::unique_ptr<Logger> _logger;
	std::list<std::shared_ptr<VerbosityLogger>> _attachedLoggers;
	static LoggerSettingsList _settings;

	Logger() = default;
	static Logger* get()
	{
		Logger::_mutex.lock();

		if (!Logger::_logger)
		{
			std::cout << "creating logger" << std::endl;
			Logger::_clock = std::make_shared<SystemClock>();
			Logger::_logger = std::unique_ptr<Logger>(new (std::nothrow) Logger);
			Logger::_logger->attachLogger(
				std::make_shared<VerbosityLogger>(std::make_shared<ConsoleLogger>(Logger::_clock)));
			Logger::_logger->_attachedLoggers.front()->setVerbosity(VerbosityLevel::Debug);
		}

		Logger::_mutex.unlock();

		return Logger::_logger.get();
	}

	static void forEachLogger(std::function<void(const std::shared_ptr<VerbosityLogger>& l)> func)
	{
		for (const auto& l : Logger::get()->_attachedLoggers)
			func(l);
	}
};

inline void configureLogger(const LoggerSettingsList& settings, std::shared_ptr<IClock> clock)
{
	Logger::configureLogger(settings, clock);
}

inline void logDebug(std::string_view message)
{
	Logger::debug(message);
}

inline void logDebug(std::string_view message, std::string_view channel)
{
	Logger::debug(message, channel);
}

inline void logVerbose(std::string_view message)
{
	Logger::verbose(message);
}

inline void logVerbose(std::string_view message, std::string_view channel)
{
	Logger::verbose(message, channel);
}

inline void logInfo(std::string_view message)
{
	Logger::info(message);
}

inline void logInfo(std::string_view message, std::string_view channel)
{
	Logger::info(message, channel);
}

inline void logWarning(std::string_view message)
{
	Logger::warning(message);
}

inline void logWarning(std::string_view message, std::string_view channel)
{
	Logger::warning(message, channel);
}

inline void logError(std::string_view message)
{
	Logger::error(message);
}

inline void logError(std::string_view message, std::string_view channel)
{
	Logger::error(message, channel);
}

#ifdef _WIN32

	#include <Windows.h>

inline void logDebug(std::string_view message, const HRESULT& hr)
{
	Logger::debug(message);
	Logger::debug("Windows Error Code: " + std::to_string(hr));
}

inline void logDebug(std::string_view message, const HRESULT& hr, std::string_view channel)
{
	Logger::debug(message, channel);
	Logger::debug("Windows Error Code: " + std::to_string(hr), channel);
}

inline void logVerbose(std::string_view message, const HRESULT& hr)
{
	Logger::verbose(message);
	Logger::verbose("Windows Error Code: " + std::to_string(hr));
}

inline void logVerbose(std::string_view message, const HRESULT& hr, std::string_view channel)
{
	Logger::verbose(message, channel);
	Logger::verbose("Windows Error Code: " + std::to_string(hr), channel);
}

inline void logInfo(std::string_view message, const HRESULT& hr)
{
	Logger::info(message);
	Logger::info("Windows Error Code: " + std::to_string(hr));
}

inline void logInfo(std::string_view message, const HRESULT& hr, std::string_view channel)
{
	Logger::info(message, channel);
	Logger::info("Windows Error Code: " + std::to_string(hr), channel);
}

inline void logWarning(std::string_view message, const HRESULT& hr)
{
	Logger::warning(message);
	Logger::warning("Windows Error Code: " + std::to_string(hr));
}

inline void logWarning(std::string_view message, const HRESULT& hr, std::string_view channel)
{
	Logger::warning(message, channel);
	Logger::warning("Windows Error Code: " + std::to_string(hr), channel);
}

inline void logError(std::string_view message, const HRESULT& hr)
{
	Logger::error(message);
	Logger::error("Windows Error Code: " + std::to_string(hr));
}

inline void logError(std::string_view message, const HRESULT& hr, std::string_view channel)
{
	Logger::error(message, channel);
	Logger::error("Windows Error Code: " + std::to_string(hr), channel);
}

#endif
