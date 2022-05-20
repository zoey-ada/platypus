#pragma once

#include <map>
#include <memory>

#include <wincodec.h>

#include "iResourceLoader.hpp"

class DirectXRenderer;
class IRenderer;

struct ID3D11ShaderResourceView;
struct IWICBitmapFrameDecode;

struct compareGuid
{
	bool operator()(REFGUID rguid1, REFGUID rguid2) const
	{
		if (rguid1.Data1 == rguid2.Data1)
		{
			if (rguid1.Data2 == rguid2.Data2)
			{
				return rguid1.Data3 < rguid2.Data3;
			}

			return rguid1.Data2 < rguid2.Data2;
		}

		return rguid1.Data1 < rguid2.Data1;
	}
};

class DirectXTextureLoader: public IResourceLoader
{
public:
	explicit DirectXTextureLoader(std::shared_ptr<ResourceCache> cache,
		const std::shared_ptr<IRenderer>& renderer);
	virtual ~DirectXTextureLoader() = default;

	[[nodiscard]] inline ResourceType getType() override { return ResourceType::Texture; }
	[[nodiscard]] inline std::string getPattern() override
	{
		return std::string(".*\\.(png)|(bmp)");
	}

	[[nodiscard]] std::shared_ptr<Resource> load(const std::shared_ptr<IResourceStore>& store,
		const std::string& filename) override;

protected:
	uint8_t* allocate(unsigned int size) override;

private:
	std::shared_ptr<ResourceCache> _cache;
	std::shared_ptr<DirectXRenderer> _renderer;

	std::map<const GUID, DXGI_FORMAT, compareGuid> _wic_to_dxgi_format_mappings;
	std::map<GUID, GUID, compareGuid> _wic_format_to_supported_format_mappings;

	[[nodiscard]] IWICBitmapFrameDecode* createTextureFromWic(const uint8_t* image_data,
		const uint64_t image_data_size);
	[[nodiscard]] std::shared_ptr<ID3D11ShaderResourceView> createWicTexture(
		IWICBitmapFrameDecode* frame);

	[[nodiscard]] DXGI_FORMAT toDxgiFormat(WICPixelFormatGUID& pixel_format) const;
	[[nodiscard]] uint64_t getBitsPerPixel(const WICPixelFormatGUID& pixel_format) const;

	void loadWicDxgiMappings();
	void loadSupportedWicMappings();
};
