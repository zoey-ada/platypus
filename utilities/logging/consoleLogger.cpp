#include "consoleLogger.hpp"

#include "../time.hpp"

#include <iostream>

void ConsoleLogger::log(std::string_view message)
{
	const std::lock_guard lock(this->_mutex);
	std::cout << "[" << getCurrentTimestamp() << "] " << message << std::endl;
}

void ConsoleLogger::log(std::string_view message, std::string_view channel)
{
	const std::lock_guard lock(this->_mutex);
	std::cout << "[" << getCurrentTimestamp() << "] <" << channel << "> " << message << std::endl;
}
