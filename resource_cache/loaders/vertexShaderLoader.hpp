#pragma once

#include <memory>

#include "iResourceLoader.hpp"

class ILoggingSystem;
class IRenderer;

namespace platypus
{

class IResourceCache;

class VertexShaderLoader: public IResourceLoader
{
public:
	explicit VertexShaderLoader(std::shared_ptr<IResourceCache> cache,
		std::shared_ptr<IRenderer> renderer, std::shared_ptr<ILoggingSystem> logging);
	virtual ~VertexShaderLoader() = default;

	[[nodiscard]] inline ResourceType getType() override { return ResourceType::VertexShader; }
	[[nodiscard]] inline std::string getPattern() override
	{
		return std::string(".*\\.(cso)|(hlsl)");
	}

	[[nodiscard]] std::shared_ptr<Resource> load(const char* resource_id, const char* store_id,
		std::byte* resource_data, const uint64_t data_size) override;

protected:
	uint8_t* allocate(unsigned int size) override;

private:
	std::shared_ptr<IResourceCache> _cache;
	std::shared_ptr<IRenderer> _renderer;
	std::shared_ptr<ILoggingSystem> _logging;
};

};
