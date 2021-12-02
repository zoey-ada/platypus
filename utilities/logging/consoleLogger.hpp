#pragma once

#include "ILogger.hpp"

#include <mutex>

class ConsoleLogger : public ILogger
{
public:
	ConsoleLogger();
	virtual ~ConsoleLogger() = default;

	void log(std::string_view message) override;
	void log(std::string_view message, std::string_view channel) override;

	const bool supportsColor() const override { return true; }

private:
	std::mutex _mutex;
};
