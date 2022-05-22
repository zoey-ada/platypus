#pragma once

#include <memory>

#include "iResourceLoader.hpp"

class DirectXRenderer;
class IRenderer;

class DirectXMeshLoader: public IResourceLoader
{
public:
	explicit DirectXMeshLoader(std::shared_ptr<ResourceCache> cache,
		const std::shared_ptr<IRenderer>& renderer);
	virtual ~DirectXMeshLoader() = default;

	inline std::string getPattern() override { return std::string(".*\\.(dae)"); }
	inline ResourceType getType() override { return ResourceType::Mesh; }

	std::shared_ptr<Resource> load(const std::shared_ptr<IResourceStore>& store,
		const std::string& filename) override;

	// std::shared_ptr<Resource> CreateRectangle(std::shared_ptr<ResourceCache> cache,
	// 	std::string name, const uint32_t height, const uint32_t width);

protected:
	uint8_t* allocate(unsigned int size) override;

private:
	std::shared_ptr<ResourceCache> _cache;
	std::shared_ptr<DirectXRenderer> _renderer;
};
