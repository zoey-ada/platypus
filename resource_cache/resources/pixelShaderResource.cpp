#include "pixelShaderResource.hpp"

#include "../stores/iResourceStore.hpp"

PixelShaderResource::PixelShaderResource(std::string name, uint8_t* buffer, uint64_t size,
	std::shared_ptr<IResourceStore> store, std::shared_ptr<ResourceCache> cache,
	std::shared_ptr<graphics::PixelShader> shader)
	: Resource(std::move(name), buffer, size, std::move(store), std::move(cache)),
	  _shader(std::move(shader))
{}
