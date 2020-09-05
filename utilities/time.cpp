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

const std::string getCurrentTimestamp()
{
	return toTimestamp(getCurrentTime());
}

const std::string toTimestamp(const Milliseconds time)
{
	time_t secs = time / 1000;
	char timestamp[25] = "";
	tm* ptm = gmtime(&secs);
	size_t len = strftime(timestamp, 20, "%FT%T", ptm);

	ptm->tm_isdst = -1;
	time_t gmt = mktime(ptm);
	int offset = (int)difftime(secs, gmt) / 60;
	std::string hrOffset = std::to_string(offset / 60);

	std::string minOffset = std::to_string(abs(offset % 60));
	if (minOffset.length() == 1)
		minOffset = "0" + minOffset;

	unsigned int ms = time % 1000;
	sprintf(timestamp + len, ".%03u", ms);

	return std::string(timestamp).append(hrOffset).append(":").append(minOffset);
}
