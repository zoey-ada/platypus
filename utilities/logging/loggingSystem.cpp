#include "loggingSystem.hpp"

#include "consoleLogger.hpp"
#include "fileLogger.hpp"
#include "verbosityDecorator.hpp"

LoggingSystem::LoggingSystem(const LoggerSettingsList& settings, std::shared_ptr<IClock> clock)
{
	this->_settings = settings;
	this->_clock = clock;

	for (const auto& l : settings)
	{
		std::shared_ptr<VerbosityLogger> logger = nullptr;

		if (l.type() == "console")
			logger =
				std::make_shared<VerbosityLogger>(std::make_shared<ConsoleLogger>(this->_clock));
		else if (l.type() == "file")
			logger = std::make_shared<VerbosityLogger>(
				std::make_shared<FileLogger>(l.file_logger_settings(), this->_clock));

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

		this->attachLogger(logger);
	}
}

void LoggingSystem::debug(const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->debug(message); });
}

void LoggingSystem::debug(const std::string_view& channel, const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->debug(message, channel); });
}

void LoggingSystem::verbose(const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->verbose(message); });
}

void LoggingSystem::verbose(const std::string_view& channel, const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->verbose(message, channel); });
}

void LoggingSystem::info(const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->info(message); });
}

void LoggingSystem::info(const std::string_view& channel, const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->info(message, channel); });
}

void LoggingSystem::warning(const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->warning(message); });
}

void LoggingSystem::warning(const std::string_view& channel, const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->warning(message, channel); });
}

void LoggingSystem::error(const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->error(message); });
}

void LoggingSystem::error(const std::string_view& channel, const std::string_view& message)
{
	this->forEachLogger([&](auto l) { l->error(message, channel); });
}

void LoggingSystem::attachLogger(const std::shared_ptr<VerbosityLogger>& logger)
{
	this->_attachedLoggers.push_back(logger);
}

void LoggingSystem::forEachLogger(
	std::function<void(const std::shared_ptr<VerbosityLogger>& l)> func) const
{
	for (const auto& l : this->_attachedLoggers)
		func(l);
}
