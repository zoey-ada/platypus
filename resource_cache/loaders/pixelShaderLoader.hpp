#pragma once

#include <memory>

#include "iResourceLoader.hpp"

class IRenderer;

class DirectXPixelShaderLoader: public IResourceLoader
{
public:
	explicit DirectXPixelShaderLoader(std::shared_ptr<ResourceCache> cache,
		std::shared_ptr<IRenderer> renderer);
	virtual ~DirectXPixelShaderLoader() = default;

	inline std::string getPattern() override { return std::string(".*\\.(cso)|(hlsl)"); }
	inline ResourceType getType() override { return ResourceType::PixelShader; }

	std::shared_ptr<Resource> load(const std::shared_ptr<IResourceStore>& store,
		const std::string& filename) override;

protected:
	uint8_t* allocate(unsigned int size) override;

private:
	std::shared_ptr<ResourceCache> _cache;
	std::shared_ptr<IRenderer> _renderer;
};
