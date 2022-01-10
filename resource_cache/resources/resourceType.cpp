#include "resourceType.hpp"

std::string std::to_string(const ResourceType type)
{
	switch (type)
	{
	case ResourceType::PixelShader:
		return "pixel_shader";
	case ResourceType::VertexShader:
		return "vertex_shader";
	default:
		return "unknown";
	}
}
