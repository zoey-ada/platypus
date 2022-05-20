#include "directXVertexShaderLoader.hpp"

#include <d3d11.h>

#include <exceptions/creationException.hpp>
#include <renderer/directx/directXRenderer.hpp>
#include <renderer/directx/directXShaderLoader.hpp>
#include <renderer/iRenderer.hpp>
#include <utilities/safeDelete.hpp>

#include "../resourceCache.hpp"
#include "../resources/resource.hpp"
#include "../resources/vertexShaderResource.hpp"
#include "../stores/iResourceStore.hpp"

DirectXVertexShaderLoader::DirectXVertexShaderLoader(std::shared_ptr<ResourceCache> cache,
	std::shared_ptr<IRenderer> renderer)
	: _cache(std::move(cache)), _renderer(std::move(renderer))
{}

std::shared_ptr<Resource> DirectXVertexShaderLoader::load(
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

	// load the shader
	ID3DBlob* bytecode = nullptr;
	if (!loadShaderBytecode(filename, buffer, size, &bytecode, ShaderType::Vertex))
	{
		// log error
		safeDeleteArray(&buffer);
		return nullptr;
	}

	ID3D11VertexShader* raw_shader = nullptr;
	ID3D11InputLayout* raw_input_layout = nullptr;

	try
	{
		raw_shader = d3d_renderer->create()->newVertexShader(bytecode);
		auto shader = toSharedPtr(&raw_shader);

		const D3D11_INPUT_ELEMENT_DESC input_desc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0}};
		raw_input_layout =
			d3d_renderer->create()->newInputLayout(bytecode, input_desc, ARRAYSIZE(input_desc));
		auto inputLayout = toSharedPtr(&raw_input_layout);

		safeRelease(&bytecode);
		return std::make_shared<VertexShaderResource>(filename, buffer, size, store, _cache, shader,
			inputLayout);
	}
	catch (CreationException&)
	{
		safeDeleteArray(&buffer);
		safeRelease(&bytecode);
		safeRelease(&raw_shader);
		safeRelease(&raw_input_layout);
		throw;
	}
}

uint8_t* DirectXVertexShaderLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}
