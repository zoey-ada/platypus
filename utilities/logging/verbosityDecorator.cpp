#include "verbosityDecorator.h"
#include "consoleFormat.h"

std::map<VerbosityLevel, FormattingOptions> formattingOptionsInit()
{
	std::map<VerbosityLevel, FormattingOptions> options;
	options[VerbosityLevel::Debug] = FormattingOptions("debug", ConsoleColor::BrightBlack);
	options[VerbosityLevel::Verbose] = FormattingOptions("verbose", ConsoleColor::White);
	options[VerbosityLevel::Info] = FormattingOptions("info", ConsoleColor::BrightWhite);
	options[VerbosityLevel::Warning] = FormattingOptions("warn", ConsoleColor::Yellow);
	options[VerbosityLevel::Error] = FormattingOptions("error", ConsoleColor::Red);
	return options;
}

VerbosityLogger::VerbosityLogger(const std::shared_ptr<ILogger>& logger)
	: _logger(logger), _level(VerbosityLevel::Warning), _channelLevels(), _mutex(),
	  _formattingOptions(formattingOptionsInit())
{ }

void VerbosityLogger::setVerbosity(VerbosityLevel level)
{
	auto lg = std::lock_guard(this->_mutex);
	this->_level = level;
}

void VerbosityLogger::setChannelVerbosity(std::string_view channel, VerbosityLevel level)
{
	auto lg = std::lock_guard(this->_mutex);
	this->_channelLevels[channel] = level;
}

void VerbosityLogger::log(std::string_view message)
{
	this->log(message, VerbosityLevel::Info);
}

void VerbosityLogger::log(std::string_view message, VerbosityLevel level)
{
	auto lg = std::lock_guard(this->_mutex);

	if (level >= this->_level)
	{
		if (this->_logger->supportsColor())
		{
			const ConsoleFormat format(this->_formattingOptions[level].color);
			this->_logger->log(std::string("(") + this->_formattingOptions[level].tag + std::string(") ") + message.data());
		}
		else
		{
			this->_logger->log(std::string("(") + this->_formattingOptions[level].tag + std::string(") ") + message.data());
		}
	}
}

void VerbosityLogger::log(std::string_view message, std::string_view channel)
{
	this->log(message, channel, VerbosityLevel::Info);
}

void VerbosityLogger::log(std::string_view message, std::string_view channel, VerbosityLevel level)
{
	auto lg = std::lock_guard(this->_mutex);

	auto channelLevel = this->_channelLevels.find(channel);
	auto resolvedLevel = (channelLevel != this->_channelLevels.end()) ? channelLevel->second : this->_level;

	if (level >= resolvedLevel)
	{
		if (this->_logger->supportsColor())
		{
			const ConsoleFormat format(this->_formattingOptions[level].color);
			this->_logger->log(std::string("(") + this->_formattingOptions[level].tag + std::string(") ") + message.data(), channel);
		}
		else
		{
			this->_logger->log(std::string("(") + this->_formattingOptions[level].tag + std::string(") ") + message.data(), channel);
		}
	}
}

void VerbosityLogger::debug(std::string_view message)
{
	this->log(message, VerbosityLevel::Debug);
}

void VerbosityLogger::debug(std::string_view message, std::string_view channel)
{
	this->log(message, channel, VerbosityLevel::Debug);
}

void VerbosityLogger::verbose(std::string_view message)
{
	this->log(message, VerbosityLevel::Verbose);
}

void VerbosityLogger::verbose(std::string_view message, std::string_view channel)
{
	this->log(message, channel, VerbosityLevel::Verbose);
}

void VerbosityLogger::info(std::string_view message)
{
	this->log(message, VerbosityLevel::Info);
}

void VerbosityLogger::info(std::string_view message, std::string_view channel)
{
	this->log(message, channel, VerbosityLevel::Info);
}

void VerbosityLogger::warning(std::string_view message)
{
	this->log(message, VerbosityLevel::Warning);
}

void VerbosityLogger::warning(std::string_view message, std::string_view channel)
{
	this->log(message, channel, VerbosityLevel::Warning);
}

void VerbosityLogger::error(std::string_view message)
{
	this->log(message, VerbosityLevel::Error);
}

void VerbosityLogger::error(std::string_view message, std::string_view channel)
{
	this->log(message, channel, VerbosityLevel::Error);
}
