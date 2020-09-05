#include "consoleLogger.h"

#include "../time.h"

#include <iostream>

ConsoleLogger::ConsoleLogger()
	: _mutex()
{ }

void ConsoleLogger::log(std::string_view message)
{
	std::lock_guard(this->_mutex);
	std::cout << "[" << getCurrentTimestamp() << "] " << message << std::endl;
}

void ConsoleLogger::log(std::string_view message, std::string_view channel)
{
	std::lock_guard(this->_mutex);
	std::cout << "[" << getCurrentTimestamp() << "] <" << channel << "> " << message << std::endl;
}
