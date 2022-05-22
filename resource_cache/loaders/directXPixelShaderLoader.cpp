#include "directXPixelShaderLoader.hpp"

#include <d3d11.h>

#include <exceptions/creationException.hpp>
#include <renderer/directx/directXRenderer.hpp>
#include <renderer/directx/directXShaderLoader.hpp>
#include <renderer/iRenderer.hpp>
#include <utilities/safeDelete.hpp>

#include "../resourceCache.hpp"
#include "../resources/pixelShaderResource.hpp"
#include "../resources/resource.hpp"
#include "../stores/iResourceStore.hpp"

DirectXPixelShaderLoader::DirectXPixelShaderLoader(std::shared_ptr<ResourceCache> cache,
	std::shared_ptr<IRenderer> renderer)
	: _cache(std::move(cache)), _renderer(std::move(renderer))
{}

std::shared_ptr<Resource> DirectXPixelShaderLoader::load(
	const std::shared_ptr<IResourceStore>& store, const std::string& filename)
{
	if (_cache == nullptr || store == nullptr || filename.empty() || _renderer == nullptr)
		return nullptr;

	auto d3d_renderer = std::dynamic_pointer_cast<DirectXRenderer>(_renderer);
	if (d3d_renderer == nullptr)
		return nullptr;

	auto size = store->getResourceSize(filename);
	auto* buffer = new (std::nothrow) uint8_t[size];

	if (buffer == nullptr)
	{
		// log res cache full...
		return nullptr;
	}

	if (!store->getResource(filename, buffer))
	{
		// log error
		return nullptr;
	}

	PtResourceData resource_data {};
	resource_data.name = filename;
	resource_data.buffer = buffer;
	resource_data.size = size;
	resource_data.store = store;
	resource_data.cache = this->_cache;

	// load the shader
	ID3DBlob* bytecode = nullptr;
	if (!loadShaderBytecode(filename, buffer, size, &bytecode, ShaderType::Pixel))
	{
		// log error
		safeDeleteArray(&buffer);
		return nullptr;
	}

	ID3D11PixelShader* shader = nullptr;

	try
	{
		shader = d3d_renderer->create()->newPixelShader(bytecode);
		safeRelease(&bytecode);

		PtPixelShaderData shader_data {};
		shader_data.pixel_shader = (PtPixelShader)shader;

		return std::make_shared<PixelShaderResource>(&resource_data, &shader_data);
	}
	catch (CreationException&)
	{
		safeDeleteArray(&buffer);
		safeRelease(&bytecode);
		safeRelease(&shader);
		throw;
	}
}

uint8_t* DirectXPixelShaderLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}
