#include "pixelShaderLoader.hpp"

#include <renderer/iRenderer.hpp>
#include <utilities/wildcardMatch.hpp>

#include "../resourceCache.hpp"
#include "../resources/pixelShaderResource.hpp"

PixelShaderLoader::PixelShaderLoader(std::shared_ptr<ResourceCache> cache,
	std::shared_ptr<IRenderer> renderer)
	: _cache(std::move(cache)), _renderer(std::move(renderer))
{}

std::shared_ptr<Resource> PixelShaderLoader::load(const char* resource_id, const char* store_id,
	std::byte* resource_data, const uint64_t data_size)
{
	if (strlen(resource_id) == 0 || _cache == nullptr || _renderer == nullptr ||
		resource_data == nullptr)
	{
		return nullptr;
	}

	bool is_precompiled = wildcardMatch("*.cso", resource_id);

	PtPixelShader shader = is_precompiled ?
		this->_renderer->shaderManager()->createPixelShader(resource_data, data_size) :
		this->_renderer->shaderManager()->compilePixelShader(resource_data, data_size, resource_id);

	PtPixelShaderData shader_data {};
	shader_data.resource_id = resource_id;
	shader_data.store_id = store_id;
	shader_data.pixel_shader = shader;
	return std::make_shared<PixelShaderResource>(&shader_data);
}

uint8_t* PixelShaderLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}
