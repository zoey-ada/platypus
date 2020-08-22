#include "Time.h"

#include <chrono>
namespace chrono = std::chrono;

const Milliseconds getCurrentTime()
{
	auto now = chrono::system_clock::now();
	auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	return static_cast<Milliseconds>(value.count());
}
