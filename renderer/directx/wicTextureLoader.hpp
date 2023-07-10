#pragma once

#include <map>
#include <memory>
#include <optional>

#include <wincodec.h>

#include <utilities/common/data.hpp>
#include <utilities/common/ptExtent.hpp>

#include "../graphics.hpp"

class DirectXRenderer;
class ILoggingSystem;

struct ID3D11ShaderResourceView;

class WicTextureLoader
{
public:
	WicTextureLoader() = default;
	virtual ~WicTextureLoader() { this->deinitialize(); }

	bool initialize();
	void deinitialize();

	std::optional<platypus::graphics::Texture> loadTexture(const platypus::Data& image_data,
		const std::shared_ptr<DirectXRenderer>& renderer);

	ID3D11ShaderResourceView* createTextTexture(const char* message, const char* font_family,
		const uint16_t point_size, const std::shared_ptr<DirectXRenderer>& renderer);

private:
	std::shared_ptr<ILoggingSystem> _log;
	const char* _log_channel {"render"};

	bool _has_been_initialized {false};
	IWICImagingFactory* _wic_factory {nullptr};

	IWICBitmapFrameDecode* getWicFrame(IWICStream* stream);
	platypus::Extent getDimensions(IWICBitmapSource* source);
	bool hasAlphaChannel(IWICBitmapSource* source);
	WICPixelFormatGUID getPixelFormat(IWICBitmapSource* source);

	IWICBitmapSource* convertTo32bitRgba(IWICBitmapSource* source);

	platypus::graphics::TextureResource createTextureFromWic(IWICBitmapSource* source,
		const std::shared_ptr<DirectXRenderer>& renderer, const platypus::Extent& dimensions);

	IWICStream* newWicStreamFromMemory(const platypus::Data& image_data) const;
	IWICBitmapDecoder* newWicBitmapDecoder(IWICStream* stream) const;
};
