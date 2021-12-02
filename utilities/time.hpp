#pragma once

#include <cstdint>
#include <string>

typedef int64_t Milliseconds;

const Milliseconds getCurrentTime();

const std::string getCurrentTimestamp();
const std::string toTimestamp(const Milliseconds time);
