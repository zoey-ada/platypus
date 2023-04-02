#pragma once

#include <memory>

#include "iResourceLoader.hpp"

class ILoggingSystem;
class IRenderer;
class IResourceCache;

class TextureLoader: public IResourceLoader
{
public:
	explicit TextureLoader(std::shared_ptr<IResourceCache> cache,
		std::shared_ptr<IRenderer> renderer, std::shared_ptr<ILoggingSystem> logging);
	virtual ~TextureLoader() = default;

	[[nodiscard]] inline ResourceType getType() override { return ResourceType::Texture; }
	[[nodiscard]] inline std::string getPattern() override
	{
		return std::string(".*\\.(png)|(bmp)");
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
