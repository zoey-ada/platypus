#include "resourceType.hpp"

std::string std::to_string(const ResourceType type)
{
	switch (type)
	{
	case ResourceType::General:
		return "general";
	case ResourceType::Audio:
		return "audio";
	case ResourceType::Mesh:
		return "mesh";
	case ResourceType::PixelShader:
		return "pixel_shader";
	case ResourceType::Protobuf:
		return "protobuf";
	case ResourceType::Texture:
		return "texture";
	case ResourceType::VertexShader:
		return "vertex_shader";
	default:
		return "unknown";
	}
}
