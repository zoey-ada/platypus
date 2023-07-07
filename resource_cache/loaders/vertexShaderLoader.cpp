#include "vertexShaderLoader.hpp"

#include <renderer/iRenderer.hpp>
#include <renderer/iShaderManager.hpp>
#include <utilities/logging/iLoggingSystem.hpp>
#include <utilities/wildcardMatch.hpp>

#include "../resourceCache.hpp"
#include "../resources/vertexShaderResource.hpp"

namespace platypus
{

VertexShaderLoader::VertexShaderLoader(std::shared_ptr<IResourceCache> cache,
	std::shared_ptr<IRenderer> renderer, std::shared_ptr<ILoggingSystem> logging)
	: _cache(std::move(cache)), _renderer(std::move(renderer)), _logging(std::move(logging))
{}

std::shared_ptr<Resource> VertexShaderLoader::load(const char* resource_id, const char* store_id,
	std::byte* resource_data, const uint64_t data_size)
{
	if (_cache == nullptr || _renderer == nullptr || resource_data == nullptr ||
		strlen(resource_id) == 0)
	{
		this->_logging->warning("cache", "received invalid data for " + std::string(resource_id));
		return nullptr;
	}

	const bool is_precompiled = wildcardMatch("*.cso", resource_id);

	PtVertexShader shader = is_precompiled ?
		this->_renderer->shaderManager()->createVertexShader(resource_data, data_size) :
		this->_renderer->shaderManager()->compileVertexShader(resource_data, data_size,
			resource_id);

	if (shader == nullptr)
	{
		this->_logging->warning("cache",
			"unable to load vertex shader from " + std::string(resource_id));

		return nullptr;
	}

	// TODO: need to load this from somewhere on a per shader basis
	std::array<PtInputLayoutDesc, 3> input_desc = {
		PtInputLayoutDesc {"POSITION", 0, PtInputFormat::Vec3_32bit_float},
		PtInputLayoutDesc {"NORMAL", 0, PtInputFormat::Vec3_32bit_float},
		PtInputLayoutDesc {"TEXCOORD", 0, PtInputFormat::Vec2_32bit_float}};

	PtInputLayout input_layout = this->_renderer->createInputLayout(resource_data, data_size,
		input_desc.data(), input_desc.size());

	if (input_layout == nullptr)
	{
		this->_logging->warning("cache",
			"unable to load input layout from " + std::string(resource_id));

		this->_renderer->shaderManager()->destroyVertexShader(shader);
		return nullptr;
	}

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

};
