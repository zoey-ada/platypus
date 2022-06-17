#pragma once

#include <memory>

#include "iResourceLoader.hpp"

class DirectXRenderer;
class IRenderer;
class MeshResource;

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

	static std::shared_ptr<MeshResource> createRectangle(
		const std::shared_ptr<DirectXRenderer>& renderer);

	static std::shared_ptr<MeshResource> createRectangleForText(
		const std::shared_ptr<DirectXRenderer>& renderer);

protected:
	uint8_t* allocate(unsigned int size) override;

private:
	std::shared_ptr<ResourceCache> _cache;
	std::shared_ptr<DirectXRenderer> _renderer;
};
