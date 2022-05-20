#include "textureResource.hpp"

TextureResource::TextureResource(std::string name, uint8_t* buffer, uint64_t size,
	std::shared_ptr<IResourceStore> store, std::shared_ptr<ResourceCache> cache,
	std::shared_ptr<graphics::Texture> texture,
	std::shared_ptr<graphics::SamplerState> sampler_state)
	: Resource(std::move(name), buffer, size, std::move(store), std::move(cache)),
	  _sampler_state(std::move(sampler_state)),
	  _texture(std::move(texture))
{}

Vec2 TextureResource::getDimensions() const
{
	return Vec2();
}
