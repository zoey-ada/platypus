#pragma once

#include "iLogger.hpp"

#include "consoleFormat.hpp"

#include <map>
#include <memory>
#include <mutex>

enum class VerbosityLevel
{
	Debug,
	Verbose,
	Info,
	Warning,
	Error
};

struct FormattingOptions
{
	std::string tag;
	ConsoleColor color = ConsoleColor::White;

	FormattingOptions() = default;
	FormattingOptions(std::string tag, const ConsoleColor color)
		: tag(std::move(tag)), color(color) { }
};

class VerbosityLogger : public ILogger
{
public:
	explicit VerbosityLogger(std::shared_ptr<ILogger> logger);
	virtual ~VerbosityLogger() = default;

	void setVerbosity(const VerbosityLevel level);
	void setChannelVerbosity(std::string_view channel, const VerbosityLevel level);

	void log(std::string_view message) override;
	void log(std::string_view message, const VerbosityLevel level);
	void log(std::string_view message, std::string_view channel) override;
	void log(std::string_view message, std::string_view channel, const VerbosityLevel level);

	void debug(std::string_view message);
	void debug(std::string_view message, std::string_view channel);

	void verbose(std::string_view message);
	void verbose(std::string_view message, std::string_view channel);

	void info(std::string_view message);
	void info(std::string_view message, std::string_view channel);

	void warning(std::string_view message);
	void warning(std::string_view message, std::string_view channel);

	void error(std::string_view message);
	void error(std::string_view message, std::string_view channel);

	[[nodiscard]] bool supportsColor() const override { return this->_logger->supportsColor(); }

private:
	std::shared_ptr<ILogger> _logger;
	VerbosityLevel _level;
	std::map<std::string_view, VerbosityLevel> _channelLevels;
	std::mutex _mutex;
	std::map<VerbosityLevel, FormattingOptions> _formattingOptions;
};
