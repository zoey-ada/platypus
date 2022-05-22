#pragma once

#include "iLogger.hpp"

#include <mutex>

class ConsoleLogger : public ILogger
{
public:
	ConsoleLogger() = default;
	virtual ~ConsoleLogger() = default;

	void log(std::string_view message) override;
	void log(std::string_view message, std::string_view channel) override;

	[[nodiscard]] bool supportsColor() const override { return true; }

private:
	std::mutex _mutex;
};
