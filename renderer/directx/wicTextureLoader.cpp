#include "wicTextureLoader.hpp"

#include <d3d11.h>
#include <dxgiformat.h>

#include <serviceProvider.hpp>
#include <utilities/common/safeRelease.hpp>
#include <utilities/logging/iLoggingSystem.hpp>

#include "../textRenderer.hpp"
#include "directXRenderer.hpp"

namespace platypus
{

// Public
bool WicTextureLoader::initialize()

{
	this->_log = ServiceProvider::getLoggingSystem();

	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		__uuidof(IWICImagingFactory), reinterpret_cast<LPVOID*>(&this->_wic_factory));

	if (FAILED(hr))
	{
		this->_log->warning(this->_log_channel, "Unable to create WIC factory.", hr);
		return this->_has_been_initialized;
	}

	this->_has_been_initialized = true;
	return this->_has_been_initialized;
}

// Public
void WicTextureLoader::deinitialize()
{
	if (!this->_has_been_initialized)
		return;

	safeRelease(&this->_wic_factory);
}

// Public
std::optional<platypus::graphics::Texture> WicTextureLoader::loadTexture(
	const platypus::Data& image_data, const std::shared_ptr<DirectXRenderer>& renderer)
{
	assert(this->_has_been_initialized);
	std::optional<platypus::graphics::Texture> loaded_texture;

	if (image_data.isEmpty())
	{
		this->_log->warning(this->_log_channel, "WIC texture loader cannot load empty image data.");
		return loaded_texture;
	}

	auto wic_stream = this->newWicStreamFromMemory(image_data);
	if (wic_stream == nullptr)
	{
		return loaded_texture;
	}

	IWICBitmapFrameDecode* wic_frame = this->getWicFrame(wic_stream);
	safeRelease(&wic_stream);

	if (wic_frame == nullptr)
	{
		return loaded_texture;
	}

	auto dimensions = this->getDimensions(wic_frame);
	if (dimensions.width == 0 || dimensions.height == 0)
	{
		safeRelease(&wic_frame);
		return loaded_texture;
	}

	auto pixel_format = this->getPixelFormat(wic_frame);
	if (pixel_format == GUID_WICPixelFormatUndefined)
	{
		safeRelease(&wic_frame);
		return loaded_texture;
	}
	auto converted = this->convertTo32bitRgba(wic_frame);
	if (converted == nullptr)
	{
		safeRelease(&wic_frame);
		return loaded_texture;
	}

	bool has_alpha = this->hasAlphaChannel(pixel_format);
	auto texture = this->createTextureFromWic(converted, renderer, dimensions);
	safeRelease(&converted);
	safeRelease(&wic_frame);

	loaded_texture = {texture, dimensions, has_alpha};
	return loaded_texture;
}

IWICBitmapFrameDecode* WicTextureLoader::getWicFrame(IWICStream* wic_stream)
{
	auto wic_decoder = this->newWicBitmapDecoder(wic_stream);
	safeRelease(&wic_stream);

	if (wic_decoder == nullptr)
	{
		return nullptr;
	}

	IWICBitmapFrameDecode* frame = nullptr;
	auto hr = wic_decoder->GetFrame(0, &frame);
	if (FAILED(hr))
	{
		this->_log->warning(this->_log_channel, "Unable to get WIC frame from WIC bitmap decoder.",
			hr);
	}

	safeRelease(&wic_decoder);
	return frame;
}

platypus::Extent WicTextureLoader::getDimensions(IWICBitmapSource* source)
{
	UINT width = 0;
	UINT height = 0;
	auto hr = source->GetSize(&width, &height);
	if (FAILED(hr) || width == 0 || height == 0)
	{
		this->_log->warning(this->_log_channel, "Unable to get size of texture from WIC frame", hr);
	}

	return {height, width};
}

bool WicTextureLoader::hasAlphaChannel(const WICPixelFormatGUID& pixel_format)
{
	// err on the side of caution and assume true if anything fails
	IWICComponentInfo* component_info = nullptr;
	auto hr = this->_wic_factory->CreateComponentInfo(pixel_format, &component_info);
	if (FAILED(hr))
		return true;

	IWICPixelFormatInfo2* pixel_format_info = nullptr;
	hr = component_info->QueryInterface(&pixel_format_info);
	if (FAILED(hr) || pixel_format_info == nullptr)
		return true;

	BOOL has_alpha = FALSE;
	hr = pixel_format_info->SupportsTransparency(&has_alpha);
	if (FAILED(hr))
		return true;

	return has_alpha == TRUE;
}

WICPixelFormatGUID WicTextureLoader::getPixelFormat(IWICBitmapSource* source)
{
	WICPixelFormatGUID pixel_format = GUID_WICPixelFormatUndefined;
	auto hr = source->GetPixelFormat(&pixel_format);
	if (FAILED(hr) || pixel_format == GUID_WICPixelFormatUndefined)
	{
		this->_log->warning(this->_log_channel,
			"Unable to get pixel format of texture from WIC frame", hr);
		return GUID_WICPixelFormatUndefined;
	}

	return pixel_format;
}

IWICBitmapSource* WicTextureLoader::convertTo32bitRgba(IWICBitmapSource* source)
{
	IWICBitmapSource* converted = nullptr;
	auto hr = WICConvertBitmapSource(GUID_WICPixelFormat32bppRGBA, source, &converted);
	if (FAILED(hr))
	{
		this->_log->warning(this->_log_channel, "Unable to convert to 32bit RGBA pixel format", hr);
		return nullptr;
	}
	return converted;
}

platypus::graphics::TextureResource WicTextureLoader::createTextureFromWic(IWICBitmapSource* source,
	const std::shared_ptr<DirectXRenderer>& renderer, const platypus::Extent& dimensions)
{
	// bitmaps will have been converted to 32bit RGBA images by now, so don't worry about
	// conversions
	const DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	const UINT bytes_per_pixel = 4;

	UINT stride = dimensions.width * bytes_per_pixel;
	UINT image_data_size = stride * dimensions.height;

	std::unique_ptr<uint8_t[]> temp(new uint8_t[image_data_size]);
	source->CopyPixels(0, stride, image_data_size, temp.get());

	// handle resizing

	// auto-gen mipmaps
	auto mip_levels = 1;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = dimensions.width;
	desc.Height = dimensions.height;
	desc.MipLevels = mip_levels;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA image_data;
	image_data.pSysMem = temp.get();
	image_data.SysMemPitch = stride;
	image_data.SysMemSlicePitch = image_data_size;

	auto texture = renderer->create()->newTexture(desc, image_data);

	// generate mipmaps with texture_view

	return reinterpret_cast<platypus::graphics::TextureResource>(texture);
}

IWICStream* WicTextureLoader::newWicStreamFromMemory(const platypus::Data& data) const
{
	IWICStream* stream = nullptr;
	auto hr = this->_wic_factory->CreateStream(&stream);

	if (FAILED(hr))
	{
		this->_log->warning(this->_log_channel, "Unable to create WIC stream", hr);
	}

	hr = stream->InitializeFromMemory(data.dataAs<uint8_t>(), static_cast<DWORD>(data.size()));

	if (FAILED(hr))
	{
		this->_log->warning(this->_log_channel, "Unable to load image data into WIC stream", hr);
	}

	return stream;
}

IWICBitmapDecoder* WicTextureLoader::newWicBitmapDecoder(IWICStream* stream) const
{
	if (this->_wic_factory == nullptr)
	{
		this->_log->warning(this->_log_channel,
			"Unable to aquire WIC factory to create WIC bitmap decoder.");
		return nullptr;
	}

	IWICBitmapDecoder* decoder = nullptr;
	auto hr = this->_wic_factory->CreateDecoderFromStream(stream, 0,
		WICDecodeOptions::WICDecodeMetadataCacheOnDemand, &decoder);

	if (FAILED(hr))
	{
		this->_log->warning(this->_log_channel, "Unable to create  WIC bitmap decoder", hr);
	}

	return decoder;
}

}
