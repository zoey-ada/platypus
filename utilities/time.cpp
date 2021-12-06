#include "time.hpp"

#include <chrono>
namespace chrono = std::chrono;

Milliseconds getCurrentTime()
{
	auto now = chrono::system_clock::now();
	auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	return static_cast<Milliseconds>(value.count());
}

std::string getCurrentTimestamp()
{
	return toTimestamp(getCurrentTime());
}

std::string toTimestamp(const Milliseconds time)
{
	time_t secs = time / milliseconds_in_second;
	char timestamp[25] = "";
	tm* ptm = gmtime(&secs);
	size_t len = strftime(timestamp, 20, "%FT%T", ptm);

	ptm->tm_isdst = -1;
	time_t gmt = mktime(ptm);
	int offset = static_cast<int>(difftime(secs, gmt) / seconds_in_minute);
	std::string hrOffset = std::to_string(offset / minutes_in_hour);
	std::string minOffset = std::to_string(abs(offset % static_cast<int>(minutes_in_hour)));
	if (minOffset.length() == 1)
		minOffset = "0" + minOffset;

	unsigned int ms = time % milliseconds_in_second;
	sprintf(timestamp + len, ".%03u", ms);

	return std::string(timestamp).append(hrOffset).append(":").append(minOffset);
}
