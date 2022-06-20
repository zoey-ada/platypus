#include "systemClock.hpp"

#include <chrono>

namespace chrono = std::chrono;

Milliseconds SystemClock::getCurrentTime()
{
	auto now = chrono::system_clock::now();
	auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	return static_cast<Milliseconds>(value.count());
}

std::string SystemClock::getCurrentTimestamp()
{
	return toTimestamp(getCurrentTime());
}
