#pragma once

#include <cstdint>
#include <string>

using Milliseconds = int64_t;

const uint32_t hours_in_day = 24;

const uint32_t minutes_in_hour = 60;
const uint32_t minutes_in_day = minutes_in_hour * hours_in_day;

const uint32_t seconds_in_minute = 60;
const uint32_t seconds_in_hour = seconds_in_minute * minutes_in_hour;
const uint32_t seconds_in_day = seconds_in_hour * hours_in_day;

const uint32_t milliseconds_in_second = 1000;
const uint32_t milliseconds_in_minute = milliseconds_in_second * seconds_in_minute;
const uint32_t milliseconds_in_hour = milliseconds_in_minute * minutes_in_hour;
const uint32_t milliseconds_in_day = milliseconds_in_hour * hours_in_day;

const Milliseconds Infinity = INT64_MAX;
const Milliseconds NegativeInfinity = INT64_MIN;

std::string toTimestamp(Milliseconds time);
