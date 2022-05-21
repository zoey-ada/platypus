#include "directXTextureLoader.hpp"

#include <d3d11.h>
#include <dxgiformat.h>
#include <wincodec.h>

#include <exceptions/creationException.hpp>
#include <renderer/directx/directXRenderer.hpp>
#include <resource_cache/stores/iResourceStore.hpp>
#include <utilities/logging/logger.hpp>
#include <utilities/safeDelete.hpp>

#include "../resourceCache.hpp"
#include "../resources/textureResource.hpp"

DirectXTextureLoader::DirectXTextureLoader(std::shared_ptr<ResourceCache> cache,
	const std::shared_ptr<IRenderer>& renderer)
	: _cache(std::move(cache))
{
	this->_renderer = std::dynamic_pointer_cast<DirectXRenderer>(renderer);
	this->loadWicDxgiMappings();
	this->loadSupportedWicMappings();
}

std::shared_ptr<Resource> DirectXTextureLoader::load(const std::shared_ptr<IResourceStore>& store,
	const std::string& filename)
{
	if (_cache == nullptr || store == nullptr || filename.empty() || _renderer == nullptr)
		return nullptr;

	auto size = store->getResourceSize(filename);
	auto* buffer = new (std::nothrow) uint8_t[size];

	if (buffer == nullptr)
	{
		// log res cache full...
		return nullptr;
	}

	if (!store->getResource(filename, buffer))
	{
		// log error
		return nullptr;
	}

	PtResourceData resource_data {};
	resource_data.name = filename;
	resource_data.buffer = buffer;
	resource_data.size = size;
	resource_data.store = store;
	resource_data.cache = this->_cache;

	Scoped<IWICBitmapFrameDecode> wic_frame = this->createTextureFromWic(buffer, size);
	if (wic_frame.isNull())
	{
		// log error
		return nullptr;
	}

	auto texture = this->createWicTexture(wic_frame.get());
	if (texture == nullptr)
	{
		// log error
		return nullptr;
	}

	// Create the sample state
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	auto sampler_state = this->_renderer->create()->newSamplerState(sampler_desc);

	PtTextureData texture_data {};
	texture_data.texture = (PtTexture)texture;
	texture_data.sampler_state = (PtSamplerState)sampler_state;

	return std::make_shared<TextureResource>(&resource_data, &texture_data);
}

uint8_t* DirectXTextureLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}

IWICBitmapFrameDecode* DirectXTextureLoader::createTextureFromWic(const uint8_t* image_data,
	const uint64_t image_data_size)
{
	Scoped<IWICStream> wic_stream = this->_renderer->create()->newWicStream();
	wic_stream->InitializeFromMemory(const_cast<uint8_t*>(image_data),
		static_cast<DWORD>(image_data_size));

	Scoped<IWICBitmapDecoder> wic_decoder =
		this->_renderer->create()->newWicBitmapDecoder(wic_stream.get());

	IWICBitmapFrameDecode* frame = nullptr;
	auto hr = wic_decoder->GetFrame(0, &frame);
	if (FAILED(hr))
	{
		throw CreationException("frame", "Unable to get WIC frame from WIC bitmap decoder.");
	}

	return frame;
}

ID3D11ShaderResourceView* DirectXTextureLoader::createWicTexture(IWICBitmapFrameDecode* frame)
{
	IWICBitmapSource* source = frame;

	UINT width = 0;
	UINT height = 0;
	auto hr = source->GetSize(&width, &height);
	if (FAILED(hr) || width == 0 || height == 0)
	{
		throw CreationException("texture", "Unable to get size of texture from WIC frame");
	}

	Scoped<IWICFormatConverter> format_converter =
		this->_renderer->create()->newWicFormatConverter();
	hr = format_converter->Initialize(source, GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherType::WICBitmapDitherTypeNone, nullptr, 0.0,
		WICBitmapPaletteType::WICBitmapPaletteTypeCustom);

	if (FAILED(hr) || width == 0 || height == 0)
	{
		throw CreationException("texture", "Unable to configure format converter");
	}

	WICPixelFormatGUID pixel_format = GUID_WICPixelFormatUndefined;
	hr = source->GetPixelFormat(&pixel_format);
	if (FAILED(hr) || pixel_format == GUID_WICPixelFormatUndefined)
	{
		throw CreationException("texture", "Unable to get pixel format of texture from WIC frame");
	}
	else if (pixel_format != GUID_WICPixelFormat32bppRGBA)
	{
		IWICBitmapSource* converted = nullptr;
		hr = WICConvertBitmapSource(GUID_WICPixelFormat32bppRGBA, source, &converted);
		if (FAILED(hr))
		{
			throw CreationException("texture", "Unable to convert to 32bit RGBA pixel format.");
		}
		source = converted;
		pixel_format = GUID_WICPixelFormat32bppRGBA;
	}

	DXGI_FORMAT format = this->toDxgiFormat(pixel_format);
	if (format == DXGI_FORMAT_UNKNOWN)
	{
		throw CreationException("texture",
			"Unable to convert WIC pixel format to DXGI pixel format");
	}

	if (!this->_renderer->doesFormatSupport(format,
			D3D11_FORMAT_SUPPORT::D3D11_FORMAT_SUPPORT_TEXTURE2D))
	{
		throw CreationException("texture", "The pixel format of the texture is not supported");
		// // Fallback to RGBA 32-bit format which is supported by all devices
		// memcpy(&convertGUID, &GUID_WICPixelFormat32bppRGBA, sizeof(WICPixelFormatGUID));
		// format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// bpp = 32;
	}

	uint64_t bpp = this->getBitsPerPixel(pixel_format);
	if (bpp == 0)
	{
		throw CreationException("texture",
			"Unable to determine bits per pixel of texture from pixel format");
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

	auto texture = this->_renderer->create()->newTexture(desc, image_data);

	// generate mipmaps with texture_view

	return texture;
}

DXGI_FORMAT DirectXTextureLoader::toDxgiFormat(WICPixelFormatGUID& pixel_format) const
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

uint64_t DirectXTextureLoader::getBitsPerPixel(const WICPixelFormatGUID& pixel_format) const
{
	try
	{
		Scoped<IWICComponentInfo> component_info =
			this->_renderer->create()->newWicComponentInfo(pixel_format);

		WICComponentType type = WICComponentType::WICCOMPONENTTYPE_FORCE_DWORD;
		auto hr = component_info->GetComponentType(&type);
		if (FAILED(hr) || type != WICComponentType::WICPixelFormat)
		{
			throw CreationException("texture",
				"Unable to get component info type for pixel format");
		}

		Scoped<IWICPixelFormatInfo> pixel_format_info = nullptr;
		hr = component_info->QueryInterface(__uuidof(IWICPixelFormatInfo),
			reinterpret_cast<void**>(&pixel_format_info));
		if (FAILED(hr))
		{
			throw CreationException("texture",
				"Unable to get pixel format info from component info");
		}

		UINT bpp = 0;
		hr = pixel_format_info->GetBitsPerPixel(&bpp);
		if (FAILED(hr))
		{
			throw CreationException("texture",
				"Unable to get bit per pixel from pixel format info");
		}

		return bpp;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

void DirectXTextureLoader::loadWicDxgiMappings()
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

void DirectXTextureLoader::loadSupportedWicMappings()
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
