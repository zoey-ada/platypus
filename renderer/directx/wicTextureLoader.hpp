#pragma once

#include <map>
#include <memory>

#include <wincodec.h>

class DirectXRenderer;
class TextureResource;

struct ID3D11ShaderResourceView;
// struct IWICBitmapFrameDecode;
// struct GUID;

// using REFGUID = const GUID&;

struct compareGuid
{
	bool operator()(REFGUID rguid1, REFGUID rguid2) const;
};

class WicTextureLoader
{
public:
	WicTextureLoader() = default;
	virtual ~WicTextureLoader() { this->deinitialize(); }

	bool initialize();
	void deinitialize();

	[[nodiscard]] ID3D11ShaderResourceView* loadTexture(std::byte* image_data,
		const uint64_t data_size, const std::shared_ptr<DirectXRenderer>& renderer);

	[[nodiscard]] ID3D11ShaderResourceView* createTextTexture(const char* message,
		const char* font_family, const uint16_t point_size,
		const std::shared_ptr<DirectXRenderer>& renderer);

private:
	IWICImagingFactory* _wic_factory {nullptr};

	std::map<const GUID, DXGI_FORMAT, compareGuid> _wic_to_dxgi_format_mappings;
	std::map<GUID, GUID, compareGuid> _wic_format_to_supported_format_mappings;

	[[nodiscard]] IWICStream* newWicStream() const;
	[[nodiscard]] IWICFormatConverter* newWicFormatConverter() const;
	[[nodiscard]] IWICBitmapDecoder* newWicBitmapDecoder(IWICStream* stream) const;
	[[nodiscard]] IWICComponentInfo* newWicComponentInfo(
		const WICPixelFormatGUID& pixel_format) const;

	[[nodiscard]] IWICBitmapFrameDecode* createTextureFromWic(std::byte* image_data,
		const uint64_t image_data_size);

	[[nodiscard]] ID3D11ShaderResourceView* createWicTexture(IWICBitmapFrameDecode* frame,
		const std::shared_ptr<DirectXRenderer>& renderer);

	[[nodiscard]] DXGI_FORMAT toDxgiFormat(WICPixelFormatGUID& pixel_format) const;
	[[nodiscard]] uint64_t getBitsPerPixel(const WICPixelFormatGUID& pixel_format) const;

	void loadWicDxgiMappings();
	void loadSupportedWicMappings();
};
