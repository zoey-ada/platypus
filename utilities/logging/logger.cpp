#include "logger.hpp"

std::mutex Logger::_mutex = std::mutex();
std::unique_ptr<Logger> Logger::_logger = nullptr;
LoggingSettings Logger::_settings = LoggingSettings();
