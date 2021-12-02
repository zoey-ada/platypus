#include "consoleLogger.hpp"

#include "../time.hpp"

#include <iostream>

ConsoleLogger::ConsoleLogger()
	: _mutex()
{ }

void ConsoleLogger::log(std::string_view message)
{
	auto lock_guard = std::lock_guard(this->_mutex);
	std::cout << "[" << getCurrentTimestamp() << "] " << message << std::endl;
}

void ConsoleLogger::log(std::string_view message, std::string_view channel)
{
	auto lock_guard = std::lock_guard(this->_mutex);
	std::cout << "[" << getCurrentTimestamp() << "] <" << channel << "> " << message << std::endl;
}
