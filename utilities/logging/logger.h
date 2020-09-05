#pragma once

#include "../../settings.h"
#include "consoleLogger.h"
#include "fileLogger.h"
#include "verbosityDecorator.h"

#include <functional>
#include <iostream>
#include <list>
#include <mutex>

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

	static void configureLogger(const LoggingSettings& settings)
	{
		Logger::_mutex.lock();

		std::cout << "configuring logger" << std::endl;
		Logger::_settings = settings;

		Logger::_logger = std::unique_ptr<Logger>(new (std::nothrow) Logger);

		for (const auto& l : settings.loggers())
		{
			std::shared_ptr<VerbosityLogger> logger = nullptr;

			if (l.type == "console")
				logger = std::make_shared<VerbosityLogger>(std::make_shared<ConsoleLogger>());
			else if (l.type == "file")
				logger = std::make_shared<VerbosityLogger>(std::make_shared<FileLogger>(l.fileRoot, l.useSingleFile));

			if (l.level == "debug")
				logger->setVerbosity(VerbosityLevel::Debug);
			else if (l.level == "verbose")
				logger->setVerbosity(VerbosityLevel::Verbose);
			else if (l.level == "info")
				logger->setVerbosity(VerbosityLevel::Info);
			else if (l.level == "warning")
				logger->setVerbosity(VerbosityLevel::Warning);
			else if (l.level == "error")
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
	Logger() = default;
	static Logger* get()
	{
		Logger::_mutex.lock();

		if (!Logger::_logger)
		{
			std::cout << "creating logger" << std::endl;
			Logger::_logger = std::unique_ptr<Logger>(new (std::nothrow) Logger);
			Logger::_logger->attachLogger(std::make_shared<VerbosityLogger>(std::make_shared<ConsoleLogger>()));
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

	static std::mutex _mutex;
	static std::unique_ptr<Logger> _logger;
	std::list<std::shared_ptr<VerbosityLogger>> _attachedLoggers;
	static LoggingSettings _settings;
};

inline void configureLogger(const LoggingSettings& settings)
{
	Logger::configureLogger(settings);
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
