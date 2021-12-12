#include "logger.hpp"

std::mutex Logger::_mutex = std::mutex();
std::unique_ptr<Logger> Logger::_logger = nullptr;
LoggerSettingsList Logger::_settings = LoggerSettingsList();
