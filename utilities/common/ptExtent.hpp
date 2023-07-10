#pragma once

#include <cstdint>

struct PtExtent
{
	uint32_t height;
	uint32_t width;
};

namespace platypus
{

struct Extent
{
	// dx uses UINTs which are only 32bits long
	uint32_t height;
	uint32_t width;
};

};
