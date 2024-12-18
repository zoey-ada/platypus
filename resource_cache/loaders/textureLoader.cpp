#include "textureLoader.hpp"

#include <renderer/iRenderer.hpp>
#include <renderer/texelOverscanMode.hpp>
#include <utilities/logging/iLoggingSystem.hpp>

#include "../resourceCache.hpp"
#include "../resources/textureResource.hpp"

namespace platypus
{

TextureLoader::TextureLoader(std::shared_ptr<IResourceCache> cache,
	std::shared_ptr<IRenderer> renderer, std::shared_ptr<ILoggingSystem> logging)
	: _cache(std::move(cache)), _renderer(std::move(renderer)), _logging(std::move(logging))
{}

std::shared_ptr<Resource> TextureLoader::load(const char* resource_id, const char* store_id,
	std::byte* resource_data, const uint64_t data_size)
{
	if (_cache == nullptr || _renderer == nullptr || resource_data == nullptr ||
		strlen(resource_id) == 0)
	{
		this->_logging->warning("cache", "received invalid data for " + std::string(resource_id));
		return nullptr;
	}

	Data data(resource_data, data_size);
	auto texture = this->_renderer->createTexture(data);
	if (!texture.has_value())
	{
		this->_logging->warning("cache", "unable to load texture from " + std::string(resource_id));
		return nullptr;
	}

	auto sampler_state = this->_renderer->createSamplerState(TexelOverscanMode::Wrap);
	if (sampler_state == nullptr)
	{
		this->_logging->warning("cache", "unable to load sampler from " + std::string(resource_id));
		return nullptr;
	}

	TextureData texture_data {};
	texture_data.resource_id = resource_id;
	texture_data.store_id = store_id;
	texture_data.size = data_size;
	texture_data.texture = texture.value().texture_resource;
	texture_data.sampler_state = sampler_state;
	texture_data.dimensions = texture.value().dimensions;
	texture_data.has_alpha = texture.value().has_alpha;

	return std::make_shared<TextureResource>(&texture_data);
}

uint8_t* TextureLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}

};
