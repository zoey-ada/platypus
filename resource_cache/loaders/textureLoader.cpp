#include "textureLoader.hpp"

#include <renderer/iRenderer.hpp>
#include <renderer/ptAddressOverscanMode.hpp>
#include <utilities/logging/logger.hpp>

#include "../resourceCache.hpp"
#include "../resources/textureResource.hpp"

TextureLoader::TextureLoader(std::shared_ptr<IResourceCache> cache,
	const std::shared_ptr<IRenderer>& renderer)
	: _cache(std::move(cache)), _renderer(std::move(renderer))
{}

std::shared_ptr<Resource> TextureLoader::load(const char* resource_id, const char* store_id,
	std::byte* resource_data, const uint64_t data_size)
{
	if (strlen(resource_id) == 0 || _cache == nullptr || _renderer == nullptr ||
		resource_data == nullptr)
	{
		return nullptr;
	}

	auto texture = this->_renderer->createTexture(resource_data, data_size);
	auto sampler_state = this->_renderer->createSamplerState(PtAddressOverscanMode::Wrap);

	PtTextureData texture_data {};
	texture_data.resource_id = resource_id;
	texture_data.store_id = store_id;
	texture_data.size = data_size;
	texture_data.texture = texture;
	texture_data.sampler_state = sampler_state;

	return std::make_shared<TextureResource>(&texture_data);
}

uint8_t* TextureLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}
