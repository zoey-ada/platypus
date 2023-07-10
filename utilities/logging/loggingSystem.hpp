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

	bool initialize() override
	{  // TODO: have each logger initialize here
		return true;
	}
	void deinitialize() override
	{
		// TODO: have each logger deinitialize here
	}

	void update(const Milliseconds /*delta*/) override {}

	void pause() override { this->_is_paused = true; }
	void resume() override { this->_is_paused = false; }

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

#ifdef _WIN32

	void debug(const std::string_view& message, const HRESULT hr) override;
	void debug(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) override;

	void verbose(const std::string_view& message, const HRESULT hr) override;
	void verbose(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) override;

	void info(const std::string_view& message, const HRESULT hr) override;
	void info(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) override;

	void warning(const std::string_view& message, const HRESULT hr) override;
	void warning(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) override;

	void error(const std::string_view& message, const HRESULT hr) override;
	void error(const std::string_view& channel, const std::string_view& message,
		const HRESULT hr) override;

#endif

private:
	std::shared_ptr<IClock> _clock;
	std::list<std::shared_ptr<VerbosityLogger>> _attachedLoggers;
	LoggerSettingsList _settings;
	bool _is_paused {false};

	void attachLogger(const std::shared_ptr<VerbosityLogger>& logger);
	void forEachLogger(std::function<void(const std::shared_ptr<VerbosityLogger>& l)> func) const;
};
