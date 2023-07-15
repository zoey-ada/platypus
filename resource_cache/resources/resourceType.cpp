#include "resourceType.hpp"

std::string std::to_string(const platypus::ResourceType type)
{
	switch (type)
	{
	case platypus::ResourceType::General:
		return "general";
	case platypus::ResourceType::Audio:
		return "audio";
	case platypus::ResourceType::Mesh:
		return "mesh";
	case platypus::ResourceType::PixelShader:
		return "pixel_shader";
	case platypus::ResourceType::Protobuf:
		return "protobuf";
	case platypus::ResourceType::Texture:
		return "texture";
	case platypus::ResourceType::VertexShader:
		return "vertex_shader";
	default:
		return "unknown";
	}
}
