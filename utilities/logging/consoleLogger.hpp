#pragma once

#include <mutex>

#include "iLogger.hpp"

class IClock;

class ConsoleLogger: public ILogger
{
public:
	ConsoleLogger(std::shared_ptr<IClock> clock): _clock(clock) {};
	virtual ~ConsoleLogger() = default;

	void log(std::string_view message) override;
	void log(std::string_view message, std::string_view channel) override;

	[[nodiscard]] bool supportsColor() const override { return true; }

private:
	std::mutex _mutex;
	std::shared_ptr<IClock> _clock;
};
