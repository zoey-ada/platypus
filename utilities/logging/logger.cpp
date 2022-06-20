#include "logger.hpp"

std::shared_ptr<IClock> Logger::_clock = nullptr;
std::mutex Logger::_mutex = std::mutex();
std::unique_ptr<Logger> Logger::_logger = nullptr;
LoggerSettingsList Logger::_settings = LoggerSettingsList();
