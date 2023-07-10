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

namespace platypus
{

class WicTextureLoader
{
public:
	WicTextureLoader() = default;
	virtual ~WicTextureLoader() { this->deinitialize(); }

	bool initialize();
	void deinitialize();

	std::optional<graphics::Texture> loadTexture(const Data& image_data,
		const std::shared_ptr<DirectXRenderer>& renderer);

private:
	std::shared_ptr<ILoggingSystem> _log;
	const char* _log_channel {"render"};

	bool _has_been_initialized {false};
	IWICImagingFactory* _wic_factory {nullptr};

	IWICBitmapFrameDecode* getWicFrame(IWICStream* stream);
	Extent getDimensions(IWICBitmapSource* source);
	bool hasAlphaChannel(const WICPixelFormatGUID& pixel_format);
	WICPixelFormatGUID getPixelFormat(IWICBitmapSource* source);

	IWICBitmapSource* convertTo32bitRgba(IWICBitmapSource* source);

	graphics::TextureResource createTextureFromWic(IWICBitmapSource* source,
		const std::shared_ptr<DirectXRenderer>& renderer, const Extent& dimensions);

	IWICStream* newWicStreamFromMemory(const Data& image_data) const;
	IWICBitmapDecoder* newWicBitmapDecoder(IWICStream* stream) const;
};

}
