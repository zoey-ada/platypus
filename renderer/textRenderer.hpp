#pragma once

#include <cstddef>
#include <cstdint>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <utilities/common/ptExtent.hpp>

struct PtTextMetrics
{
	const char* message;
	PtExtent size;
	uint32_t max_ascender;
	uint32_t max_descender;
};

struct PtGlyphMetrics
{
	uint32_t height_from_baseline;
	uint32_t descender_height;

	uint32_t padding_top;
	uint32_t padding_bottom;
	uint32_t padding_right;

	PtExtent glyph_size;
	PtExtent total_size;
	uint32_t advance;

	PtGlyphMetrics(PtTextMetrics* text_metrics, FT_GlyphSlot glyph)
		: height_from_baseline(glyph->metrics.horiBearingY / 64),
		  glyph_size(PtExtent {glyph->bitmap.rows, static_cast<uint32_t>(glyph->bitmap.pitch)}),
		  advance(glyph->metrics.horiAdvance / 64)
	{
		this->descender_height = (glyph->metrics.height / 64) - this->height_from_baseline;
		this->padding_top = text_metrics->max_ascender - this->height_from_baseline;
		this->padding_bottom = text_metrics->max_descender - this->descender_height;
		this->padding_right = this->advance - glyph->bitmap.pitch;
		this->total_size = PtExtent {text_metrics->size.height, this->advance};
	}
};

class TextRenderer
{
public:
	TextRenderer() = default;
	virtual ~TextRenderer() { this->deinitialize(); }

	bool initialize();
	void deinitialize();

	bool loadFont(const char* font_family);
	bool loadFont(const char* font_family, const char* style);
	void unloadFont();

	bool setFontSize(uint16_t point_size);

	PtTextMetrics measureText(const char* message);
	PtTextMetrics measureText(const char* message, const uint16_t point_size);

	std::byte* rasterizeText(const char* message);
	std::byte* rasterizeText(const char* message, const uint16_t point_size);
	std::byte* rasterizeText(const char* message, PtTextMetrics* text_metrics);
	std::byte* rasterizeText(const char* message, PtTextMetrics* text_metrics,
		const uint16_t point_size);

private:
	FT_Library _freetype_handle {nullptr};
	FT_Face _face {nullptr};

	const char* _font_family {""};
	const char* _style {""};
	uint16_t _point_size {0};

	uint32_t _pixel_density;

	bool isFontLoaded(const char* font_family, const char* style = "regular");
};
