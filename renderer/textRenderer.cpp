#include "textRenderer.hpp"

#include <platform/iPlatform.hpp>
#include <serviceProvider.hpp>
#include <utilities/encoding.hpp>
#include <utilities/logging/logger.hpp>

bool TextRenderer::initialize()
{
	auto error = FT_Init_FreeType(&this->_freetype_handle);
	if (FT_Err_Ok != error)
	{
		logError("Failed to load FreeType library.", "render");
		return false;
	}

	auto pixel_density = ServiceProvider::getPlatform()->getPixelDensity();
	if (pixel_density.height > UINT16_MAX || pixel_density.width > UINT16_MAX)
	{
		logError("Unable to handle such a high pixel density.", "render");
		return false;
	}
	this->_pixel_density = static_cast<uint32_t>((pixel_density.height + pixel_density.width) / 2);

	return true;
}

void TextRenderer::deinitialize()
{
	if (this->_freetype_handle != nullptr)
	{
		auto error = FT_Done_FreeType(this->_freetype_handle);
		if (FT_Err_Ok != error)
		{
			logError("Failed to unload FreeType library.", "render");
		}
		this->_freetype_handle = nullptr;
	}
}

bool TextRenderer::loadFont(const char* font_family)
{
	if (this->isFontLoaded(font_family))
	{
		logVerbose("Font already loaded.", "render");
		return true;
	}

	auto error = FT_New_Face(this->_freetype_handle, font_family, 0, &this->_face);
	if (FT_Err_Ok != error)
	{
		logError("Failed to load font " + std::string(font_family) + ".", "render");
		return false;
	}

	this->_font_family = font_family;
	this->_style = "regular";
	return true;
}

bool TextRenderer::loadFont(const char* font_family, const char* style)
{
	if (this->isFontLoaded(font_family, style))
	{
		logVerbose("Font already loaded.", "render");
		return true;
	}

	auto error = FT_New_Face(this->_freetype_handle, font_family, 0, &this->_face);
	if (FT_Err_Ok != error)
	{
		logError(
			"Failed to load font " + std::string(font_family) + " - " + std::string(style) + ".",
			"render");
		return false;
	}

	this->_font_family = font_family;
	this->_style = style;
	return true;
}

void TextRenderer::unloadFont()
{
	if (this->_face != nullptr)
	{
		auto error = FT_Done_Face(this->_face);
		if (FT_Err_Ok != error)
		{
			logError("Failed to unload font.", "render");
		}
		this->_face = nullptr;
	}
}

bool TextRenderer::setFontSize(uint16_t point_size)
{
	auto error = FT_Set_Char_Size(this->_face, 0, point_size * 64, this->_pixel_density,
		this->_pixel_density);
	if (FT_Err_Ok != error)
	{
		logError("Failed to update font size to " + std::to_string(point_size) + " point.",
			"render");
		return false;
	}

	return true;
}

PtTextMetrics TextRenderer::measureText(const char* message, uint16_t point_size)
{
	if (!this->setFontSize(point_size))
	{
		return PtTextMetrics {};
	}

	return this->measureText(message);
}

PtTextMetrics TextRenderer::measureText(const char* message)
{
	uint32_t width = 0;
	uint32_t highest_ascender = 0;
	uint32_t lowest_descender = 0;

	std::u32string u32_message = Encoding::toUtf32(message);

	for (int i = 0; u32_message[i] != '\0'; ++i)
	{
		auto error = FT_Load_Char(this->_face, u32_message[i], FT_LOAD_RENDER);
		if (FT_Err_Ok != error)
		{
			return {0, 0};
		}

		width += static_cast<uint32_t>(this->_face->glyph->metrics.horiAdvance / 64);
		uint32_t baseline_height = (this->_face->glyph->metrics.horiBearingY / 64);

		uint32_t pixel_height = this->_face->glyph->metrics.height / 64;
		uint32_t descender_height = 0;

		if (baseline_height < pixel_height)
			descender_height = pixel_height - baseline_height;

		if (baseline_height > highest_ascender)
			highest_ascender = baseline_height;

		if (descender_height > lowest_descender)
			lowest_descender = descender_height;
	}

	uint32_t height = highest_ascender + lowest_descender;

	PtTextMetrics metrics {};
	metrics.size = {height, width};
	metrics.message = message;
	metrics.max_ascender = highest_ascender;
	metrics.max_descender = lowest_descender;
	return metrics;
}

std::byte* TextRenderer::rasterizeText(const char* message)
{
	auto text_metrics = this->measureText(message);
	return this->rasterizeText(message, &text_metrics);
}

std::byte* TextRenderer::rasterizeText(const char* message, const uint16_t point_size)
{
	if (!this->setFontSize(point_size))
	{
		return nullptr;
	}
	auto text_metrics = this->measureText(message);
	return this->rasterizeText(message, &text_metrics);
}

std::byte* TextRenderer::rasterizeText(const char* message, PtTextMetrics* text_metrics,
	const uint16_t point_size)
{
	if (!this->setFontSize(point_size))
	{
		return nullptr;
	}
	return this->rasterizeText(message, text_metrics);
}

std::byte* TextRenderer::rasterizeText(const char* message, PtTextMetrics* text_metrics)
{
	uint64_t stride = text_metrics->size.width * 4;
	uint64_t image_data_size = stride * text_metrics->size.height;

	std::byte* buffer = new (std::nothrow) std::byte[image_data_size];

	std::u32string u32_message = Encoding::toUtf32(message);
	uint64_t glyph_offset = 0;

	for (int c = 0; u32_message[c] != '\0'; ++c)
	{
		auto error = FT_Load_Char(this->_face, u32_message[c], FT_LOAD_RENDER);
		if (FT_Err_Ok != error)
		{
			logError("unable to load glyph " + std::to_string(u32_message[c]) + " from font " +
				this->_face->family_name + " - " + this->_face->style_name);
			delete[] buffer;
			buffer = nullptr;
			return buffer;
		}

		PtGlyphMetrics glyph_metrics(text_metrics, this->_face->glyph);

		for (uint64_t row = 0; row < glyph_metrics.total_size.height; ++row)
		{
			uint64_t row_offset = glyph_offset + (row * text_metrics->size.width);

			for (uint64_t pixel = 0; pixel < glyph_metrics.total_size.width; ++pixel)
			{
				uint64_t pixel_offset = row_offset + pixel;
				uint64_t memory_offset = pixel_offset * 4;

				if (row < glyph_metrics.padding_top ||
					row >= glyph_metrics.padding_top + glyph_metrics.glyph_size.height ||
					pixel >= glyph_metrics.glyph_size.width)
				{
					buffer[memory_offset] = std::byte {255};
					buffer[memory_offset + 1] = std::byte {255};
					buffer[memory_offset + 2] = std::byte {255};
					buffer[memory_offset + 3] = std::byte {0};
				}
				else
				{
					uint64_t source_offset =
						(row - glyph_metrics.padding_top) * glyph_metrics.glyph_size.width;
					source_offset += pixel;

					uint8_t value = this->_face->glyph->bitmap.buffer[source_offset];
					uint8_t inverted_value = static_cast<uint8_t>(255 - value);

					buffer[memory_offset] = std::byte {inverted_value};
					buffer[memory_offset + 1] = std::byte {inverted_value};
					buffer[memory_offset + 2] = std::byte {inverted_value};
					buffer[memory_offset + 3] = std::byte {value};
				}
			}
		}

		glyph_offset += glyph_metrics.advance;
	}

	return buffer;
}

bool TextRenderer::isFontLoaded(const char* font_family, const char* style)
{
	return (strcmp(this->_font_family, font_family) == 0) && (strcmp(this->_style, style) == 0);
}
