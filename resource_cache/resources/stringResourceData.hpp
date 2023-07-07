#pragma once

#include <string>

#include "resource.hpp"

namespace platypus
{

struct StringData: public PtResourceData
{
	std::string string_data;
};

};
