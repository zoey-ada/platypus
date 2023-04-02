#pragma once

#include <functional>

#include <platypus_proto/settings.hpp>

#include "iLoggingSystem.hpp"

class IClock;
class VerbosityLogger;

using LoggerSettingsList = google::protobuf::RepeatedPtrField<platypus::LoggerSettings>;

class LoggingSystem: public ILoggingSystem
{
public:
	LoggingSystem(const LoggerSettingsList& settings, std::shared_ptr<IClock> clock);
	virtual ~LoggingSystem() = default;

	void debug(const std::string_view& message) override;
	void debug(const std::string_view& channel, const std::string_view& message) override;

	void verbose(const std::string_view& message) override;
	void verbose(const std::string_view& channel, const std::string_view& message) override;

	void info(const std::string_view& message) override;
	void info(const std::string_view& channel, const std::string_view& message) override;

	void warning(const std::string_view& message) override;
	void warning(const std::string_view& channel, const std::string_view& message) override;

	void error(const std::string_view& message) override;
	void error(const std::string_view& channel, const std::string_view& message) override;

private:
	std::shared_ptr<IClock> _clock;
	std::list<std::shared_ptr<VerbosityLogger>> _attachedLoggers;
	LoggerSettingsList _settings;

	void attachLogger(const std::shared_ptr<VerbosityLogger>& logger);
	void forEachLogger(std::function<void(const std::shared_ptr<VerbosityLogger>& l)> func) const;
};
