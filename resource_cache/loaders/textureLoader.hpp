#pragma once

#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "iResourceLoader.hpp"

class IRenderer;
class IResourceCache;
struct PtFontDesc;

class TextureLoader: public IResourceLoader
{
public:
	explicit TextureLoader(std::shared_ptr<IResourceCache> cache,
		const std::shared_ptr<IRenderer>& renderer);
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
};
