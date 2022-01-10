#include "vertexShaderResource.hpp"

#include "../stores/iResourceStore.hpp"

VertexShaderResource::VertexShaderResource(std::string name, uint8_t* buffer,
	unsigned int size, std::shared_ptr<IResourceStore> store,
	std::shared_ptr<ResourceCache> cache,
	std::shared_ptr<graphics::VertexShader> shader,
	std::shared_ptr<graphics::InputLayout> input_layout)
	: Resource(std::move(name), buffer, size, std::move(store), std::move(cache)),
	_shader(std::move(shader)), _input_layout(std::move(input_layout))
{}
