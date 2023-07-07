#include "uuid.hpp"

namespace platypus
{

std::string generate_simple_uuid()
{
	static uint64_t previous_uuid = 0;
	return std::to_string(++previous_uuid);
}

}
