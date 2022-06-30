#include "wicTextureLoader.hpp"

#include <d3d11.h>
#include <dxgiformat.h>

#include <utilities/common/safeRelease.hpp>
#include <utilities/logging/logger.hpp>

#include "../textRenderer.hpp"
#include "directXRenderer.hpp"

bool compareGuid::operator()(REFGUID rguid1, REFGUID rguid2) const
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

bool WicTextureLoader::initialize()
{
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		__uuidof(IWICImagingFactory), reinterpret_cast<LPVOID*>(&this->_wic_factory));

	if (FAILED(hr))
	{
		logWarning("Unable to create WIC factory.", hr, "render");
		return false;
	}

	this->loadSupportedWicMappings();
	this->loadWicDxgiMappings();

	return true;
}

void WicTextureLoader::deinitialize()
{
	safeRelease(&this->_wic_factory);
}

ID3D11ShaderResourceView* WicTextureLoader::loadTexture(std::byte* image_data,
	const uint64_t data_size, const std::shared_ptr<DirectXRenderer>& renderer)
{
	IWICBitmapFrameDecode* wic_frame = this->createTextureFromWic(image_data, data_size);
	if (wic_frame != nullptr)
	{
		auto texture = this->createWicTexture(wic_frame, renderer);
		return texture;
	}

	return nullptr;
}

IWICBitmapFrameDecode* WicTextureLoader::createTextureFromWic(std::byte* image_data,
	const uint64_t image_data_size)
{
	auto wic_stream = this->newWicStream();
	if (wic_stream == nullptr)
	{
		return nullptr;
	}

	wic_stream->InitializeFromMemory((uint8_t*)(image_data), static_cast<DWORD>(image_data_size));
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
		logWarning("Unable to get WIC frame from WIC bitmap decoder.", hr, "render");
	}

	safeRelease(&wic_decoder);
	return frame;
}

ID3D11ShaderResourceView* WicTextureLoader::createWicTexture(IWICBitmapFrameDecode* frame,
	const std::shared_ptr<DirectXRenderer>& renderer)
{
	IWICBitmapSource* source = frame;

	UINT width = 0;
	UINT height = 0;
	auto hr = source->GetSize(&width, &height);
	if (FAILED(hr) || width == 0 || height == 0)
	{
		logWarning("Unable to get size of texture from WIC frame", hr, "render");
		return nullptr;
	}

	IWICFormatConverter* format_converter = this->newWicFormatConverter();
	if (format_converter == nullptr)
	{
		return nullptr;
	}

	hr = format_converter->Initialize(source, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone,
		nullptr, 0.0, WICBitmapPaletteTypeCustom);

	safeRelease(&format_converter);

	if (FAILED(hr))
	{
		logWarning("Unable to convert image format", hr, "render");
		return nullptr;
	}

	WICPixelFormatGUID pixel_format = GUID_WICPixelFormatUndefined;
	hr = source->GetPixelFormat(&pixel_format);
	if (FAILED(hr) || pixel_format == GUID_WICPixelFormatUndefined)
	{
		logWarning("Unable to get pixel format of texture from WIC frame", hr, "render");
		return nullptr;
	}
	else if (pixel_format != GUID_WICPixelFormat32bppRGBA)
	{
		IWICBitmapSource* converted = nullptr;
		hr = WICConvertBitmapSource(GUID_WICPixelFormat32bppRGBA, source, &converted);
		if (FAILED(hr))
		{
			logWarning("Unable to convert to 32bit RGBA pixel format", hr, "render");
			return nullptr;
		}
		source = converted;
		pixel_format = GUID_WICPixelFormat32bppRGBA;
	}

	DXGI_FORMAT format = this->toDxgiFormat(pixel_format);
	if (format == DXGI_FORMAT_UNKNOWN)
	{
		logWarning("Unable to convert WIC pixel format to DXGI pixel format", "render");
		return nullptr;
	}

	if (!renderer->doesFormatSupport(format, D3D11_FORMAT_SUPPORT::D3D11_FORMAT_SUPPORT_TEXTURE2D))
	{
		logWarning("The pixel format of the texture is not supported", "render");
		return nullptr;
		// Fallback to RGBA 32-bit format which is supported by all devices
		// memcpy(&convertGUID, &GUID_WICPixelFormat32bppRGBA, sizeof(WICPixelFormatGUID));
		// format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// bpp = 32;
	}

	uint64_t bpp = this->getBitsPerPixel(pixel_format);
	if (bpp == 0)
	{
		logWarning("Unable to determine bits per pixel of texture from pixel format", "render");
		return nullptr;
	}

	// stride => the number of bytes in a single row of the image
	// pad the bits in a single row with 7 extra bits to ensure that we always
	// have the correct number of bytes after rounding
	const uint8_t byte_padding = 7;
	const uint8_t bits_in_byte = 8;
	UINT stride = (width * static_cast<UINT>(bpp) + byte_padding) / bits_in_byte;
	UINT image_data_size = stride * height;

	std::unique_ptr<uint8_t[]> temp(new uint8_t[image_data_size]);
	source->CopyPixels(0, stride, image_data_size, temp.get());

	// handle resizing
	// handle pixel format conversion without resizing

	// auto-gen mipmaps

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	// desc.Format = format;
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

	return texture;
}

ID3D11ShaderResourceView* WicTextureLoader::createTextTexture(const char* message,
	const char* font_family, const uint16_t point_size,
	const std::shared_ptr<DirectXRenderer>& renderer)
{
	TextRenderer text_renderer;
	if (!text_renderer.initialize() || !text_renderer.loadFont(font_family) ||
		!text_renderer.setFontSize(point_size))
	{
		return nullptr;
	}

	auto text_metrics = text_renderer.measureText(message);
	auto pixel_buffer = text_renderer.rasterizeText(message, &text_metrics);

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = static_cast<UINT>(text_metrics.size.width);
	desc.Height = static_cast<UINT>(text_metrics.size.height);
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	uint64_t stride = text_metrics.size.width * 4;
	uint64_t image_data_size = stride * text_metrics.size.height;

	D3D11_SUBRESOURCE_DATA image_data;
	image_data.pSysMem = pixel_buffer;
	image_data.SysMemPitch = static_cast<UINT>(stride);
	image_data.SysMemSlicePitch = static_cast<UINT>(image_data_size);

	auto texture = renderer->create()->newTexture(desc, image_data);
	return texture;
}

DXGI_FORMAT WicTextureLoader::toDxgiFormat(WICPixelFormatGUID& pixel_format) const
{
	try
	{
		return this->_wic_to_dxgi_format_mappings.at(pixel_format);
	}
	catch (const std::out_of_range&)
	{
		return DXGI_FORMAT_UNKNOWN;
	}
}

uint64_t WicTextureLoader::getBitsPerPixel(const WICPixelFormatGUID& pixel_format) const
{
	IWICComponentInfo* component_info = this->newWicComponentInfo(pixel_format);
	if (component_info == nullptr)
	{
		return 0;
	}

	WICComponentType type = WICComponentType::WICCOMPONENTTYPE_FORCE_DWORD;
	auto hr = component_info->GetComponentType(&type);
	if (FAILED(hr) || type != WICComponentType::WICPixelFormat)
	{
		logWarning("Unable to get component info type for pixel format", hr, "render");
		safeRelease(&component_info);
		return 0;
	}

	IWICPixelFormatInfo* pixel_format_info = nullptr;
	hr = component_info->QueryInterface(__uuidof(IWICPixelFormatInfo),
		reinterpret_cast<void**>(&pixel_format_info));

	safeRelease(&component_info);

	if (FAILED(hr))
	{
		logWarning("Unable to get pixel format info from component info", hr, "render");
		return 0;
	}

	UINT bpp = 0;
	hr = pixel_format_info->GetBitsPerPixel(&bpp);
	if (FAILED(hr))
	{
		logWarning("Unable to get bit per pixel from pixel format info", hr, "render");
	}

	safeRelease(&pixel_format_info);

	return bpp;
}

void WicTextureLoader::loadWicDxgiMappings()
{
	auto& mappings = this->_wic_to_dxgi_format_mappings;
	mappings[GUID_WICPixelFormat128bppRGBAFloat] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	mappings[GUID_WICPixelFormat64bppRGBAHalf] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	mappings[GUID_WICPixelFormat64bppRGBA] = DXGI_FORMAT_R16G16B16A16_UNORM;
	mappings[GUID_WICPixelFormat32bppRGBA] = DXGI_FORMAT_R8G8B8A8_UNORM;

	// DXGI 1.1
	mappings[GUID_WICPixelFormat32bppBGRA] = DXGI_FORMAT_B8G8R8A8_UNORM;
	mappings[GUID_WICPixelFormat32bppBGR] = DXGI_FORMAT_B8G8R8X8_UNORM;
	mappings[GUID_WICPixelFormat32bppRGBA1010102XR] = DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;

	mappings[GUID_WICPixelFormat32bppRGBA1010102] = DXGI_FORMAT_R10G10B10A2_UNORM;
	mappings[GUID_WICPixelFormat32bppRGBE] = DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
	mappings[GUID_WICPixelFormat32bppGrayFloat] = DXGI_FORMAT_R32_FLOAT;
	mappings[GUID_WICPixelFormat16bppGrayHalf] = DXGI_FORMAT_R16_FLOAT;
	mappings[GUID_WICPixelFormat16bppGray] = DXGI_FORMAT_R16_UNORM;
	mappings[GUID_WICPixelFormat8bppGray] = DXGI_FORMAT_R8_UNORM;
	mappings[GUID_WICPixelFormat8bppAlpha] = DXGI_FORMAT_A8_UNORM;

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
	mappings[GUID_WICPixelFormat96bppRGBFloat] = DXGI_FORMAT_R32G32B32_FLOAT;
#endif
}

void WicTextureLoader::loadSupportedWicMappings()
{
	auto& mappings = this->_wic_format_to_supported_format_mappings;

	// DXGI_FORMAT_R8_UNORM
	mappings[GUID_WICPixelFormatBlackWhite] = GUID_WICPixelFormat8bppGray;

	// DXGI_FORMAT_R8G8B8A8_UNORM
	mappings[GUID_WICPixelFormat1bppIndexed] = GUID_WICPixelFormat32bppRGBA;
	mappings[GUID_WICPixelFormat2bppIndexed] = GUID_WICPixelFormat32bppRGBA;
	mappings[GUID_WICPixelFormat4bppIndexed] = GUID_WICPixelFormat32bppRGBA;
	mappings[GUID_WICPixelFormat8bppIndexed] = GUID_WICPixelFormat32bppRGBA;

	// DXGI_FORMAT_R8_UNORM
	mappings[GUID_WICPixelFormat2bppGray] = GUID_WICPixelFormat8bppGray;
	mappings[GUID_WICPixelFormat4bppGray] = GUID_WICPixelFormat8bppGray;

	// DXGI_FORMAT_R16_FLOAT
	mappings[GUID_WICPixelFormat16bppGrayFixedPoint] = GUID_WICPixelFormat16bppGrayHalf;

	// DXGI_FORMAT_R32_FLOAT
	mappings[GUID_WICPixelFormat32bppGrayFixedPoint] = GUID_WICPixelFormat32bppGrayFloat;

#ifdef DXGI_1_2_FORMATS
	// DXGI_FORMAT_B5G5R5A1_UNORM
	mappings[GUID_WICPixelFormat16bppBGR555] = GUID_WICPixelFormat16bppBGRA5551;
#else
	// DXGI_FORMAT_R8G8B8A8_UNORM
	mappings[GUID_WICPixelFormat16bppBGR555] = GUID_WICPixelFormat32bppRGBA;
	mappings[GUID_WICPixelFormat16bppBGRA5551] = GUID_WICPixelFormat32bppRGBA;
	mappings[GUID_WICPixelFormat16bppBGR565] = GUID_WICPixelFormat32bppRGBA;
#endif

	// DXGI_FORMAT_R10G10B10A2_UNORM
	mappings[GUID_WICPixelFormat32bppBGR101010] = GUID_WICPixelFormat32bppRGBA1010102;

	// DXGI_FORMAT_R8G8B8A8_UNORM
	mappings[GUID_WICPixelFormat24bppBGR] = GUID_WICPixelFormat32bppRGBA;
	mappings[GUID_WICPixelFormat24bppRGB] = GUID_WICPixelFormat32bppRGBA;
	mappings[GUID_WICPixelFormat32bppPBGRA] = GUID_WICPixelFormat32bppRGBA;
	mappings[GUID_WICPixelFormat32bppPRGBA] = GUID_WICPixelFormat32bppRGBA;

	// DXGI_FORMAT_R16G16B16A16_UNORM
	mappings[GUID_WICPixelFormat48bppRGB] = GUID_WICPixelFormat64bppRGBA;
	mappings[GUID_WICPixelFormat48bppBGR] = GUID_WICPixelFormat64bppRGBA;
	mappings[GUID_WICPixelFormat64bppBGRA] = GUID_WICPixelFormat64bppRGBA;
	mappings[GUID_WICPixelFormat64bppPRGBA] = GUID_WICPixelFormat64bppRGBA;
	mappings[GUID_WICPixelFormat64bppPBGRA] = GUID_WICPixelFormat64bppRGBA;

	// DXGI_FORMAT_R16G16B16A16_FLOAT
	mappings[GUID_WICPixelFormat48bppRGBFixedPoint] = GUID_WICPixelFormat64bppRGBAHalf;
	mappings[GUID_WICPixelFormat48bppBGRFixedPoint] = GUID_WICPixelFormat64bppRGBAHalf;
	mappings[GUID_WICPixelFormat64bppRGBAFixedPoint] = GUID_WICPixelFormat64bppRGBAHalf;
	mappings[GUID_WICPixelFormat64bppBGRAFixedPoint] = GUID_WICPixelFormat64bppRGBAHalf;
	mappings[GUID_WICPixelFormat64bppRGBFixedPoint] = GUID_WICPixelFormat64bppRGBAHalf;
	mappings[GUID_WICPixelFormat64bppRGBHalf] = GUID_WICPixelFormat64bppRGBAHalf;
	mappings[GUID_WICPixelFormat48bppRGBHalf] = GUID_WICPixelFormat64bppRGBAHalf;

	// DXGI_FORMAT_R32G32B32A32_FLOAT
	mappings[GUID_WICPixelFormat96bppRGBFixedPoint] = GUID_WICPixelFormat128bppRGBAFloat;
	mappings[GUID_WICPixelFormat128bppPRGBAFloat] = GUID_WICPixelFormat128bppRGBAFloat;
	mappings[GUID_WICPixelFormat128bppRGBFloat] = GUID_WICPixelFormat128bppRGBAFloat;
	mappings[GUID_WICPixelFormat128bppRGBAFixedPoint] = GUID_WICPixelFormat128bppRGBAFloat;
	mappings[GUID_WICPixelFormat128bppRGBFixedPoint] = GUID_WICPixelFormat128bppRGBAFloat;

	// DXGI_FORMAT_R8G8B8A8_UNORM
	mappings[GUID_WICPixelFormat32bppCMYK] = GUID_WICPixelFormat32bppRGBA;

	// DXGI_FORMAT_R16G16B16A16_UNORM
	mappings[GUID_WICPixelFormat64bppCMYK] = GUID_WICPixelFormat64bppRGBA;

	// DXGI_FORMAT_R16G16B16A16_UNORM
	mappings[GUID_WICPixelFormat40bppCMYKAlpha] = GUID_WICPixelFormat64bppRGBA;
	mappings[GUID_WICPixelFormat80bppCMYKAlpha] = GUID_WICPixelFormat64bppRGBA;

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
	// DXGI_FORMAT_R8G8B8A8_UNORM
	mappings[GUID_WICPixelFormat32bppRGB] = GUID_WICPixelFormat32bppRGBA;

	// DXGI_FORMAT_R16G16B16A16_UNORM
	mappings[GUID_WICPixelFormat64bppRGB] = GUID_WICPixelFormat64bppRGBA;

	// DXGI_FORMAT_R16G16B16A16_FLOAT
	mappings[GUID_WICPixelFormat64bppPRGBAHalf] = GUID_WICPixelFormat64bppRGBAHalf;
#endif
}

IWICStream* WicTextureLoader::newWicStream() const
{
	if (this->_wic_factory == nullptr)
	{
		logWarning("Unable to aquire WIC factory to create WIC stream.", "render");
		return nullptr;
	}

	IWICStream* stream = nullptr;
	auto hr = this->_wic_factory->CreateStream(&stream);

	if (FAILED(hr))
	{
		logWarning("Unable to create  WIC stream", hr, "render");
	}

	return stream;
}

IWICBitmapDecoder* WicTextureLoader::newWicBitmapDecoder(IWICStream* stream) const
{
	if (this->_wic_factory == nullptr)
	{
		logWarning("Unable to aquire WIC factory to create WIC bitmap decoder.", "render");
		return nullptr;
	}

	IWICBitmapDecoder* decoder = nullptr;
	auto hr = this->_wic_factory->CreateDecoderFromStream(stream, 0,
		WICDecodeOptions::WICDecodeMetadataCacheOnDemand, &decoder);

	if (FAILED(hr))
	{
		logWarning("Unable to create  WIC bitmap decoder", hr, "render");
	}

	return decoder;
}

IWICComponentInfo* WicTextureLoader::newWicComponentInfo(
	const WICPixelFormatGUID& pixel_format) const
{
	if (this->_wic_factory == nullptr)
	{
		logWarning("Unable to aquire WIC factory to create WIC component info.", "render");
		return nullptr;
	}

	IWICComponentInfo* component_info = nullptr;
	auto hr = this->_wic_factory->CreateComponentInfo(pixel_format, &component_info);

	if (FAILED(hr))
	{
		logWarning("Unable to create  WIC component info", hr, "render");
	}

	return component_info;
}

IWICFormatConverter* WicTextureLoader::newWicFormatConverter() const
{
	if (this->_wic_factory == nullptr)
	{
		logWarning("Unable to aquire WIC factory to create WIC format converter.", "render");
		return nullptr;
	}

	IWICFormatConverter* converter = nullptr;
	auto hr = this->_wic_factory->CreateFormatConverter(&converter);

	if (FAILED(hr))
	{
		logWarning("Unable to create  WIC format converter", hr, "render");
	}

	return converter;
}
