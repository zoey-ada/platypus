#include "vertexShaderLoader.hpp"

#include <renderer/iRenderer.hpp>
#include <utilities/wildcardMatch.hpp>

#include "../resourceCache.hpp"
#include "../resources/vertexShaderResource.hpp"

VertexShaderLoader::VertexShaderLoader(std::shared_ptr<ResourceCache> cache,
	std::shared_ptr<IRenderer> renderer)
	: _cache(std::move(cache)), _renderer(std::move(renderer))
{}

std::shared_ptr<Resource> VertexShaderLoader::load(const char* resource_id, const char* store_id,
	std::byte* resource_data, const uint64_t data_size)
{
	if (strlen(resource_id) == 0 || _cache == nullptr || _renderer == nullptr ||
		resource_data == nullptr)
	{
		return nullptr;
	}

	bool is_precompiled = wildcardMatch("*.cso", resource_id);

	PtVertexShader shader = is_precompiled ?
		this->_renderer->shaderManager()->createVertexShader(resource_data, data_size) :
		this->_renderer->shaderManager()->compileVertexShader(resource_data, data_size,
			resource_id);

	// TODO: need to load this from somewhere on a per shader basis
	std::array<PtInputLayoutDesc, 3> input_desc = {
		PtInputLayoutDesc {"POSITION", 0, PtInputFormat::Vec3_32bit_float},
		PtInputLayoutDesc {"NORMAL", 0, PtInputFormat::Vec3_32bit_float},
		PtInputLayoutDesc {"TEXCOORD", 0, PtInputFormat::Vec2_32bit_float}};

	PtInputLayout input_layout = this->_renderer->createInputLayout(resource_data, data_size,
		input_desc.data(), input_desc.size());

	PtVertexShaderData shader_data {};
	shader_data.resource_id = resource_id;
	shader_data.store_id = store_id;
	shader_data.vertex_shader = shader;
	shader_data.input_layout = input_layout;

	return std::make_shared<VertexShaderResource>(&shader_data);
}

uint8_t* VertexShaderLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}
