#include "consoleLogger.hpp"

#include <iostream>

#include "../time/iClock.hpp"

void ConsoleLogger::log(std::string_view message)
{
	const std::lock_guard lock(this->_mutex);
	std::cout << "[" << this->_clock->getCurrentTimestamp() << "] " << message << std::endl;
}

void ConsoleLogger::log(std::string_view message, std::string_view channel)
{
	const std::lock_guard lock(this->_mutex);
	std::cout << "[" << this->_clock->getCurrentTimestamp() << "] <" << channel << "> " << message
			  << std::endl;
}
