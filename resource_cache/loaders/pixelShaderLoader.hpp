#pragma once

#include <memory>

#include "iResourceLoader.hpp"

class ILoggingSystem;
class IRenderer;
class IResourceCache;

namespace platypus
{

class PixelShaderLoader: public IResourceLoader
{
public:
	explicit PixelShaderLoader(std::shared_ptr<IResourceCache> cache,
		std::shared_ptr<IRenderer> renderer, std::shared_ptr<ILoggingSystem> logging);
	virtual ~PixelShaderLoader() = default;

	[[nodiscard]] inline ResourceType getType() override { return ResourceType::PixelShader; }
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
