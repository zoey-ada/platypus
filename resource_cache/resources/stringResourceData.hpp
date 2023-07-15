#pragma once

#include <string>

#include "resource.hpp"

namespace platypus
{

struct StringData: public ResourceData
{
	std::string string_data;
};

};
