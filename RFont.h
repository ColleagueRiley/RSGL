/*
* Copyright (c) 2021-25 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following r estrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/

/*
preprocessor args

make sure

** #define RFONT_IMPLEMENTATION ** - include function defines

is in exactly one of your files or arguments

#define RFONT_INT_DEFINED - int types are already defined
#define RFONT_C89 - uses cints instead of stdint.h and __inline instead of inline
#define RFONT_INLINE x - set your own inline

#define RFONT_NO_STDIO - do not include stdio.h
#define RFONT_EXTERNAL_STB - load stb_truetype from stb_truetype.h instead of using the internal version
#define RFONT_EXTERNAL_STB_IMPLEMENTATION - the same as RFONT_EXTERNAL_STB but doesn't define STB_TRUETYPE_IMPLEMENTATION
-- NOTE: By default, opengl 3.3 vbos are used for rendering --
*/

/*
credits :

stb_truetype.h - a dependency for RFont, most of (a slightly motified version of) stb_truetype.h is included directly into RFont.h
http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ - UTF-8 decoding function
fontstash - fontstash was used as a refference for some parts
*/

/*

... = [add code here]

BASIC TEMPLATE :
#define RFONT_IMPLEMENTATION
#include "RFont.h"

...

int main () {
   ...

   RFont_renderer renderer = ...;
   RFont_renderer_init(&renderer);
   RFont_font* font = RFont_font_init(&renderer, "font.ttf", 20, 500, 500);

   i32 w = ...;
   i32 h = ...;

   while (1) {
      ...
	  RFont_renderer_set_framebuffer(&renderer, (u32)w, (u32)h);
	  RFont_renderer_set_color(&renderer, 0.0f, 1.0f, 0, 1.0f);
	  RFont_draw_text(&renderer, font, "text", 100, 100, 20);
      ...
   }

   RFont_font_free(&renderer, font);
   RFont_renderer_free(&renderer);
   ...
}
*/

#ifndef RFONT_INLINE
    #ifdef RFONT_C89
        #define RFONT_INLINE
    #else
        #define RFONT_INLINE inline
    #endif
#endif

#ifndef RFONT_API
    #ifdef RFONT_STATIC
        #define RFONT_API static
    #else
        #define RFONT_API extern RFONT_INLINE
    #endif
#endif


#ifndef RFONT_NO_STDIO
#include <stdio.h>
#endif

#ifndef RFONT_MALLOC
#include <stdlib.h>
#define RFONT_MALLOC malloc
#define RFONT_FREE free
#endif

#if !defined(RFONT_MEMCPY) || !defined(RFONT_MEMSET)
	#include <string.h>
#endif

#ifndef RFONT_MEMSET
    #define RFONT_MEMSET(ptr, value, num) memset(ptr, value, num)
#endif

#ifndef RFONT_MEMCPY
	#define RFONT_MEMCPY(dist, src, len) memcpy(dist, src, len)
#endif

#ifndef RFONT_ASSERT
    #include <assert.h>
    #define RFONT_ASSERT(x) assert(x)
#endif

#if !defined(RFONT_FLOOR) || !defined(RFONT_CEIL) || !defined(RFONT_FABS) || !defined(RFONT_SQRT)
    #include <math.h>
#endif

#ifndef RFONT_FLOOR
    #define RFONT_FLOOR(x) floor(x)
#endif

#ifndef RFONT_CEIL
    #define RFONT_CEIL(x) ceil(x)
#endif

#ifndef RFONT_FABS
    #define RFONT_FABS(x) fabs(x)
#endif

#ifndef RFONT_SQRT
    #define RFONT_SQRT(x) sqrt(x)
#endif

#if defined(__STDC__) && !defined(__STDC_VERSION__)
    #define RFONT_C89
#endif

#if !defined(RFONT_INT_DEFINED)
    #define RFONT_INT_DEFINED
    #if defined(_MSC_VER) || defined(__SYMBIAN32__) || defined(RFONT_C89)
        typedef unsigned char 	u8;
		typedef signed char		i8;
		typedef unsigned short  u16;
		typedef signed short 	i16;
		typedef unsigned int 	u32;
		typedef signed int		i32;
		typedef unsigned long	u64;
		typedef signed long		i64;
	#else
		#include <stdint.h>

		typedef uint8_t     u8;
		typedef int8_t      i8;
		typedef uint16_t   u16;
		typedef int16_t    i16;
		typedef uint32_t   u32;
		typedef int32_t    i32;
		typedef uint64_t   u64;
		typedef int64_t    i64;
	#endif
#endif

#if !defined(b8)
	typedef u8 b8;
#endif

/*
You can define these yourself if
you want to change anything
*/

#ifndef RFont_texture
typedef size_t RFont_texture;
#endif

#ifndef RFONT_MAX_GLYPHS
#define RFONT_MAX_GLYPHS 256
#endif

#ifndef RFONT_INIT_VERTS
#define RFONT_INIT_VERTS 20 * RFONT_MAX_GLYPHS
#endif

#ifndef RFONT_UNUSED
#define RFONT_UNUSED(x) (void) (x);
#endif

#ifndef RFONT_RENDERER_H
#define RFONT_RENDERER_H
typedef struct RFont_renderer_proc {
	size_t (*size)(void); /*!< get the size of the renderer context */
	void (*initPtr)(void* ctx); /* any initalizations the renderer needs to do */
	RFont_texture (*create_atlas)(void* ctx, u32 atlasWidth, u32 atlasHeight); /* create a bitmap texture based on the given size */
	void (*free_atlas)(void* ctx, RFont_texture atlas);
	void (*bitmap_to_atlas)(void* ctx, RFont_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y); /* add the given bitmap to the texture based on the given coords and size data */
	void (*render)(void* ctx, RFont_texture atlas, float* verts, float* tcoords, size_t nverts); /* render the text, using the vertices, atlas texture, and texture coords given. */
	void (*set_framebuffer)(void* ctx, u32 weight, u32 height); /*!< set the frame buffer size (for ortho, for example) */
	void (*set_color)(void* ctx, float r, float g, float b, float a); /*!< set the current rendering color */
	void (*freePtr)(void* ctx); /* free any memory the renderer might need to free */
} RFont_renderer_proc;

typedef struct RFont_renderer {
	void* ctx; /*!< source renderer data */
	RFont_renderer_proc proc;
} RFont_renderer;

typedef struct RFont_font RFont_font;

#endif /* RFONT_RENDERER_H */

#ifndef RFONT_H
#define RFONT_H

RFONT_API size_t RFont_renderer_size(RFont_renderer* renderer);

RFONT_API RFont_renderer* RFont_renderer_init(RFont_renderer_proc proc);
RFONT_API void RFont_renderer_initPtr(RFont_renderer_proc proc, void* ptr, RFont_renderer* renderer);

RFONT_API void RFont_renderer_set_framebuffer(RFont_renderer* renderer, u32 w, u32 h);
RFONT_API void RFont_renderer_set_color(RFont_renderer* renderer, float r, float g, float b, float a);

RFONT_API void RFont_renderer_free(RFont_renderer* renderer);
RFONT_API void RFont_renderer_freePtr(RFont_renderer* renderer);

#define RFONT_GET_FONT_WIDTH(fontHeight) RFONT_MAX_GLYPHS * fontHeight

typedef struct {
   u32 codepoint; /* the character (for checking) */
   size_t size; /* the size of the glyph */
   i32 x, x2, y, y2;  /* coords of the character on the texture */
   RFont_font* font; /* the font that the glyph belongs to */

   /* source glyph data */
   i32 src;
   float w, h, x1, y1, advance;
} RFont_glyph;

typedef struct RFont_src RFont_src;

struct RFont_font {
	RFont_src* src; /* source stb font info */
	float fheight; /* font height from stb */
	float descent; /* font descent */
	float numOfLongHorMetrics;
	float space_adv;
	u32 maxHeight;

	RFont_glyph glyphs[RFONT_MAX_GLYPHS]; /* glyphs */
	size_t glyph_len;

	RFont_texture atlas; /* atlas texture */
	size_t atlasWidth, atlasHeight;
	float atlasX, atlasY; /* the current position inside the atlas */

	float verts[RFONT_INIT_VERTS];
	float tcoords[RFONT_INIT_VERTS];
};


/**
 * @brief Converts a codepoint to a utf8 string.
 * @param codepoint The codepoint to convert to utf8.
 * @return The utf8 string.
*/
RFONT_API char* RFont_codepoint_to_utf8(u32 codepoint);

#ifndef RFONT_NO_STDIO
/**
 * @brief Init font stucture with a TTF file path.
 * @param font_name The TTF file path.
 * @param atlasWidth The width of the atlas texture.
 * @param atlasHeight The height of the atlas texture. (This should == the max text size)
 * @return The `RFont_font` created using the TTF file data.
*/
RFONT_API RFont_font* RFont_font_init(RFont_renderer* renderer, const char* font_name, u32 maxHeight, size_t atlasWidth, size_t atlasHeight);
/**
 * @brief Init a given font stucture with a TTF file path.
 * @param font_name The TTF file path.
 * @param atlasWidth The width of the atlas texture.
 * @param atlasHeight The height of the atlas texture. (This should == the max text size)
 * @pram ptr Pointer to the given font structure
 * @return returns the same pointer or NULL if the font failed to load
*/
RFONT_API RFont_font* RFont_font_init_ptr(RFont_renderer* renderer, const char* font_name, u32 maxHeight, size_t atlasWidth, size_t atlasHeight, RFont_font* font);

#endif

/**
 * @brief Init font stucture with raw TTF data.
 * @param font_data The raw TTF data.
 * @param atlasWidth The width of the atlas texture.
 * @param atlasHeight The height of the atlas texture. (This should == the max text size)
 * @return The `RFont_font` created from the data.
*/
RFONT_API RFont_font* RFont_font_init_data(RFont_renderer* renderer, u8* font_data, u32 maxHeight, size_t atlasWidth, size_t atlasHeight);

/**
 * @brief Init a given font stucture with raw TTF data.
 * @param font_data The raw TTF data.
 * @param atlasWidth The width of the atlas texture.
 * @param atlasHeight The height of the atlas texture. (This should == the max text size)
 * @return The `RFont_font` created from the data.
 * @return returns the same pointer or NULL if the font failed to load
*/
RFONT_API RFont_font* RFont_font_init_data_ptr(RFont_renderer* renderer, u8* font_data, u32 maxHeight, size_t atlasWidth, size_t atlasHeight, RFont_font* ptr);

/**
 * @brief Free data from the font stucture, including the stucture itself
 * @param font The font stucture to free
*/
RFONT_API void RFont_font_free(RFont_renderer* renderer, RFont_font* font);

/**
 * @brief Free data from the font stucture only (not including the stucture)
 * @param font The strucutre with the font data  to free
*/
RFONT_API void RFont_font_free_ptr(RFont_renderer* renderer, RFont_font* font);

typedef RFont_glyph (*RFont_glyph_fallback_callback)(RFont_renderer* renderer, RFont_font* font, u32 codepoint, size_t size);
RFont_glyph_fallback_callback RFont_set_glyph_fallback_callback(RFont_glyph_fallback_callback callback);

/**
 * @brief Add a character to the font's atlas.
 * @param font The font to use.
 * @param ch The character to add to the atlas.
 * @param size The size of the character.
 * @return The `RFont_glyph` created from the data and added to the atlas.
*/
RFONT_API RFont_glyph RFont_font_add_char(RFont_renderer* renderer,RFont_font* font, char ch, size_t size);

/**
 * @brief Add a codepoint to the font's atlas.
 * @param font The font to use.
 * @param codepoint The codepoint to add to the atlas.
 * @param size The size of the character.
 * @return The `RFont_glyph` created from the data and added to the atlas.
*/
RFONT_API RFont_glyph RFont_font_add_codepoint(RFont_renderer* renderer, RFont_font* font, u32 codepoint, size_t size);

/**
 * @brief Add a codepoint to the font's atlas.
 * @param font The font to use.
 * @param codepoint The codepoint to add to the atlas.
 * @param size The size of the character.
 * @param fallback If the fallback function should not be called.
 * @return The `RFont_glyph` created from the data and added to the atlas.
*/
RFONT_API RFont_glyph RFont_font_add_codepoint_ex(RFont_renderer* renderer, RFont_font* font, u32 codepoint, size_t size, b8 fallback);

/**
 * @brief Add a string to the font's atlas.
 * @param font The font to use.
 * @param ch The character to add to the atlas.
 * @param sizes The supported sizes of the character.
 * @param sizeLen length of the size array
*/
RFONT_API void RFont_font_add_string(RFont_renderer* renderer, RFont_font* font, const char* string, size_t* sizes, size_t sizeLen);

/**
 * @brief Add a string to the font's atlas based on a given string length.
 * @param font The font to use.
 * @param ch The character to add to the atlas.
 * @param strLen length of the string
 * @param sizes The supported sizes of the character.
 * @param sizeLen length of the size array
*/
RFONT_API void RFont_font_add_string_len(RFont_renderer* renderer, RFont_font* font, const char* string, size_t strLen, size_t* sizes, size_t sizeLen);

/**
 * @brief Get the area of the text based on the size using the font.
 * @param font The font stucture to use for drawing
 * @param text The string to draw
 * @param size The size of the text
 * @param [OUTPUT] the output width
 * @param [OUTPUT] the output height
*/
RFONT_API void RFont_text_area(RFont_renderer* renderer, RFont_font* font, const char* text, u32 size, u32* w, u32* h);

/**
 * @brief Get the area of the text based on the size using the font, using a given length.
 * @param font The font stucture to use for drawing
 * @param text The string to draw
 * @param size The size of the text
 * @param spacing The spacing of the text
 * @param [OUTPUT] the output width
 * @param [OUTPUT] the output height
*/
RFONT_API void RFont_text_area_spacing(RFont_renderer* renderer, RFont_font* font, const char* text, float spacing, u32 size, u32* w, u32* h);

/**
 * @brief Get the area of the text based on the size using the font, using a given length.
 * @param font The font stucture to use for drawing
 * @param text The string to draw
 * @param len The length of the string
 * @param size The size of the text
 * @param stopNL the number of \n s until it stops (0 = don't stop until the end)
 * @param spacing The spacing of the text
 * @param [OUTPUT] the output width
 * @param [OUTPUT] the output height
*/
RFONT_API void RFont_text_area_len(RFont_renderer* renderer, RFont_font* font, const char* text, size_t len, u32 size, size_t stopNL, float spacing, u32* w, u32* h);

/**
 * @brief Draw a text string using the font.
 * @param font The font stucture to use for drawing
 * @param text The string to draw
 * @param x The x position of the text
 * @param y The y position of the text
 * @param size The size of the text
*/
RFONT_API void RFont_draw_text(RFont_renderer* renderer, RFont_font* font, const char* text, float x, float y, u32 size);

/**
 * @brief Draw a text string using the font and a given spacing.
 * @param font The font stucture to use for drawing
 * @param text The string to draw
 * @param x The x position of the text
 * @param y The y position of the text
 * @param size The size of the text
 * @param spacing The spacing of the text
*/
RFONT_API void RFont_draw_text_spacing(RFont_renderer* renderer, RFont_font* font, const char* text, float x, float y, u32 size, float spacing);

/**
 * @brief Draw a text string using the font using a given length and a given spacing.
 * @param font The font stucture to use for drawing
 * @param text The string to draw
 * @param len The length of the string
 * @param x The x position of the text
 * @param y The y position of the text
 * @param size The size of the text
 * @param spacing The spacing of the text
*/
RFONT_API void RFont_draw_text_len(RFont_renderer* renderer, RFont_font* font, const char* text, size_t len, float x, float y, u32 size, float spacing);

#endif /* RFONT_H */

#ifdef RFONT_IMPLEMENTATION

#ifndef RFONT_GET_TEXPOSX
#define RFONT_GET_TEXPOSX(x, w) (float)((float)(x) / (float)(w))
#define RFONT_GET_TEXPOSY(y, h) (float)((float)(y) / (float)(h))
#endif

size_t RFont_renderer_size(RFont_renderer* renderer) {
	return renderer->proc.size();
}

RFont_renderer* RFont_renderer_init(RFont_renderer_proc proc) {
	RFont_renderer* renderer = (RFont_renderer*)RFONT_MALLOC(sizeof(RFont_renderer));
	void* ptr = NULL;
	size_t size = 0;
	if (proc.size) size = proc.size();

	if (size) ptr = RFONT_MALLOC(size);

	RFont_renderer_initPtr(proc, ptr, renderer);
	return renderer;
}

void RFont_renderer_initPtr(RFont_renderer_proc proc, void* ptr, RFont_renderer* renderer) {
	renderer->ctx = ptr;
	renderer->proc = proc;
	renderer->proc.initPtr(renderer->ctx);
}

void RFont_renderer_set_framebuffer(RFont_renderer* renderer, u32 w, u32 h) {
	renderer->proc.set_framebuffer(renderer->ctx, w, h);
}

void RFont_renderer_set_color(RFont_renderer* renderer, float r, float g, float b, float a) {
	renderer->proc.set_color(renderer->ctx, r, g, b, a);
}

void RFont_renderer_free(RFont_renderer* renderer) {
	RFont_renderer_freePtr(renderer);
	if (renderer->ctx) RFONT_FREE(renderer->ctx);
	RFONT_FREE(renderer);
}

void RFont_renderer_freePtr(RFont_renderer* renderer) {
	renderer->proc.freePtr(renderer->ctx);
}

#define RFONT_CHAR(p, index)     (((char*)p)[index])
#define RFONT_BYTE(p, index)     (((u8*)p)[index])
#define RFONT_SHORT(arr, index) (i16)((i16)((u8*)arr)[(size_t)(index)]*256 + (i16)(((u8*)arr)[(size_t)(index) + 1]))
#define RFONT_USHORT(arr, index) (u16)((u16)((u8*)arr)[(size_t)(index)]*256 + (u16)(((u8*)arr)[(size_t)(index) + 1]))
#define RFONT_ULONG(arr, index)  (u32)((u32)(((u8*)arr)[(size_t)(index)]<<24) + (u32)(((u8*)arr)[(size_t)(index) + 1]<<16) + (u32)(((u8*)arr)[(size_t)(index) + 2]<<8) + (u32)(((u8*)arr))[(size_t)(index) + 3])

/*
stb defines required by RFont

you probably don't care about this part if you're reading just the RFont code
*/

#ifndef RFONT_EXTERNAL_STB
typedef struct {
   unsigned char *data;
   int cursor;
   int size;
} rstbtt__buf;

typedef struct rstbtt_fontinfo rstbtt_fontinfo;

struct rstbtt_fontinfo {
   unsigned char  * data;              /* pointer to .ttf file  */
   int              fontstart;         /* offset of start of font  */

   int numGlyphs;                     /* number of glyphs, needed for range checking  */

   int loca,head,glyf,hhea,hmtx,kern,gpos,svg; /* table locations as offset from start of .ttf  */
   int index_map;                     /* a cmap mapping for our chosen character encoding  */
   int indexToLocFormat;              /* format needed to map from glyph index to glyph  */

   rstbtt__buf cff;                    /* cff font data  */
   rstbtt__buf charstrings;            /* the charstring index  */
   rstbtt__buf gsubrs;                 /* global charstring subroutines index  */
   rstbtt__buf subrs;                  /* private charstring subroutines index  */
   rstbtt__buf fontdicts;              /* array of font dicts  */
   rstbtt__buf fdselect;               /* map from glyph to fontdict */
};

RFONT_API int rstbtt_InitFont(rstbtt_fontinfo *info, const unsigned char *data, int offset);

RFONT_API unsigned char* rstbtt_GetGlyphBitmapSubpixel(const rstbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int *width, int *height, int *xoff, int *yoff);

RFONT_API int rstbtt_FindGlyphIndex(const rstbtt_fontinfo *info, int unicode_codepoint);

RFONT_API int  rstbtt_GetGlyphKernAdvance(const rstbtt_fontinfo *info, int glyph1, int glyph2);
RFONT_API int  rstbtt_GetGlyphBox(const rstbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1);
#else
#ifdef RFONT_EXTERNAL_STB_IMPLEMENTATION
    #define RFONT_EXTERNAL_STB
#endif

#ifndef RFONT_EXTERNAL_STB_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#endif
#include "stb_truetype.h"

typedef struct stbtt_fontinfo rstbtt_fontinfo;

#define rstbtt_InitFont stbtt_InitFont
#define rstbtt_GetGlyphBitmapSubpixel stbtt_GetGlyphBitmapSubpixel
#define rstbtt_FindGlyphIndex stbtt_FindGlyphIndex
#define rstbtt_GetGlyphKernAdvance stbtt_GetGlyphKernAdvance
#define rstbtt_GetGlyphBox stbtt_GetGlyphBox
#endif /* RFONT_EXTERNAL_STB */

struct RFont_src {
    rstbtt_fontinfo info;
};

/*
END of stb defines required by RFont

you probably care about this part
*/

#ifndef RFONT_NO_STDIO
char* RFont_read_file(const char* font_name) {
    size_t size, out;
    char* ttf_buffer;
    FILE* ttf_file = fopen(font_name, "rb");

    if (ttf_file == NULL) return NULL;

    fseek(ttf_file, 0U, SEEK_END);
    size = (size_t)ftell(ttf_file);
    if (size <= 0) return NULL;

    ttf_buffer = (char*)RFONT_MALLOC(sizeof(char) * (size_t)size);
    fseek(ttf_file, 0U, SEEK_SET);

    out = fread(ttf_buffer, 1, (size_t)size, ttf_file);
    RFONT_UNUSED(out);

    return ttf_buffer;
}

RFont_font* RFont_font_init(RFont_renderer* renderer, const char* font_name, u32 maxHeight, size_t atlasWidth, size_t atlasHeight) {
    char* ttf_buffer = RFont_read_file(font_name);
    RFont_font* font = RFont_font_init_data(renderer, (u8*)ttf_buffer, maxHeight, atlasWidth, atlasHeight);
    return font;
}

RFont_font* RFont_font_init_ptr(RFont_renderer* renderer, const char* font_name, u32 maxHeight, size_t atlasWidth, size_t atlasHeight, RFont_font* ptr) {
    char* ttf_buffer = RFont_read_file(font_name);
    RFont_font* font = RFont_font_init_data_ptr(renderer, (u8*)ttf_buffer, maxHeight, atlasWidth, atlasHeight, ptr);
    return font;
}
#endif

RFont_font* RFont_font_init_data(RFont_renderer* renderer, u8* font_data, u32 maxHeight, size_t atlasWidth, size_t atlasHeight) {
    RFont_font* font = (RFont_font*)RFONT_MALLOC(sizeof(RFont_font));
    return RFont_font_init_data_ptr(renderer, font_data, maxHeight, atlasWidth, atlasHeight, font);
}

RFont_font* RFont_font_init_data_ptr(RFont_renderer* renderer, u8* font_data, u32 maxHeight, size_t atlasWidth, size_t atlasHeight, RFont_font* font) {
	i32 space_codepoint = 0;
	font->src = (RFont_src*)RFONT_MALLOC(sizeof(RFont_src));
	font->atlasWidth = atlasWidth;
	font->atlasHeight = atlasHeight;
	font->maxHeight = maxHeight;

	rstbtt_InitFont(&font->src->info, font_data, 0);

	font->fheight = RFONT_SHORT(font->src->info.data, font->src->info.hhea + 4) - RFONT_SHORT(font->src->info.data, font->src->info.hhea + 6);
	font->descent = RFONT_SHORT(font->src->info.data, font->src->info.hhea + 6);

	font->numOfLongHorMetrics = RFONT_USHORT(font->src->info.data, font->src->info.hhea + 34);


	space_codepoint = rstbtt_FindGlyphIndex(&font->src->info, (int)' ');
	if (' ' < font->numOfLongHorMetrics)
		font->space_adv = RFONT_SHORT(font->src->info.data, font->src->info.hmtx + 4 * space_codepoint);
	else
		font->space_adv = RFONT_SHORT(font->src->info.data, font->src->info.hmtx + 4 * (i32)(font->numOfLongHorMetrics - 1));

	font->atlas = renderer->proc.create_atlas(renderer->ctx, (u32)atlasWidth, (u32)atlasHeight);
	font->atlasX = 0;
	font->atlasY = 0;
	font->glyph_len = 0;
	return font;
}

void RFont_font_free_ptr(RFont_renderer* renderer, RFont_font* font) {
   renderer->proc.free_atlas(renderer->ctx, font->atlas);
   RFONT_FREE(font->src);
}

void RFont_font_free(RFont_renderer* renderer, RFont_font* font) {
    RFONT_FREE(font->src->info.data);
    RFont_font_free_ptr(renderer, font);
    RFONT_FREE(font);
}

/*
decode utf8 character to codepoint
*/

/* Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de>
 See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.
*/

#define RFONT_UTF8_ACCEPT 0
#define RFont_UTF8_REJECT 12

RFONT_API u32 RFont_decode_utf8(u32* state, u32* codep, u32 byte);

u32 RFont_decode_utf8(u32* state, u32* codep, u32 byte) {
   static const u8 utf8d[] = {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 00..1f */
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 20..3f */
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 40..5f */
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 60..7f */
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, /* 80..9f */
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, /* a0..bf */
      8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, /* c0..df  */
      0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, /* e0..ef  */
      0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, /* f0..ff  */
      0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, /* s0..s0  */
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, /* s1..s2  */
      1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, /* s3..s4  */
      1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, /* s5..s6  */
      1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /* s7..s8  */
   };

   u32 type = utf8d[byte];

   *codep = (*state != RFONT_UTF8_ACCEPT) ?
      (byte & 0x3fu) | (*codep << 6) :
      (0xff >> type) & (byte);

   *state = utf8d[256 + *state * 16 + type];
   return *state;
}

void RFont_font_add_string(RFont_renderer* renderer, RFont_font* font, const char* string, size_t* sizes, size_t sizeLen) {
   RFont_font_add_string_len(renderer, font, string, 0, sizes, sizeLen);
}

void RFont_font_add_string_len(RFont_renderer* renderer, RFont_font* font, const char* string, size_t strLen, size_t* sizes, size_t sizeLen) {
   u32 i;
   char* str;
   for (str = (char*)string; (!strLen || (size_t)(str - string) < strLen) && *str; str++)
      for (i = 0; i < sizeLen; i++)
         RFont_font_add_char(renderer, font, *str, sizes[i]);
}

RFont_glyph_fallback_callback RFont_glyph_fallback = NULL;
RFont_glyph_fallback_callback RFont_set_glyph_fallback_callback(RFont_glyph_fallback_callback callback) {
   RFont_glyph_fallback_callback old = RFont_glyph_fallback;
   RFont_glyph_fallback = callback;
   return old;
}

RFont_glyph RFont_font_add_char(RFont_renderer* renderer, RFont_font* font, char ch, size_t size) {
   static u32 utf8state = 0, codepoint = 0;

   if (RFont_decode_utf8(&utf8state, &codepoint, (u8)ch) != RFONT_UTF8_ACCEPT) {
      RFont_glyph g;
      RFONT_MEMSET(&g, 0, sizeof(RFont_glyph));
      return g;
   }

   return RFont_font_add_codepoint(renderer, font, codepoint, size);
}

RFont_glyph RFont_font_add_codepoint(RFont_renderer* renderer, RFont_font* font, u32 codepoint, size_t size) {
   return RFont_font_add_codepoint_ex(renderer, font, codepoint, size, 1);
}

RFont_glyph RFont_font_add_codepoint_ex(RFont_renderer* renderer, RFont_font* font, u32 codepoint, size_t size, b8 fallback) {
	RFont_glyph* glyph;
	RFont_glyph glyphNull;

	u8* bitmap;
	float scale;

	i32 x0, y0, x1, y1, w = 0, h = 0, advanceX = 0;
	u32 i;
	for (i = 0; i < font->glyph_len; i++)
		if (font->glyphs[i].codepoint == codepoint && font->glyphs[i].size == size)
			return font->glyphs[i];

	RFONT_MEMSET(&glyphNull, 0, sizeof(glyphNull));
	if (i < sizeof(font->glyphs)) {
		glyph = &font->glyphs[i];
	} else {
		return glyphNull;
	}

	glyph->src = rstbtt_FindGlyphIndex(&font->src->info, (int)codepoint);

	if ((glyph->src == 0 && codepoint) && fallback && RFont_glyph_fallback) {
		RFont_glyph fallbackGlyph = RFont_glyph_fallback(renderer, font, codepoint, size);
		if (fallbackGlyph.codepoint != 0 && fallbackGlyph.size != 0) {
			return fallbackGlyph;
		}
	}

	if (glyph->src == 0 && codepoint) return RFont_font_add_codepoint_ex(renderer, font, 0, size, fallback);
	font->glyph_len++;

	if (codepoint && rstbtt_GetGlyphBox(&font->src->info, glyph->src, &x0, &y0, &x1, &y1) == 0) {
		return glyphNull;
	}

	scale = ((float)size) / font->fheight;
	bitmap =  rstbtt_GetGlyphBitmapSubpixel(&font->src->info, 0, scale, 0.0f, 0.0f, glyph->src, &w, &h, 0, 0);
	glyph->w = (float)w;
	glyph->h = (float)h;

	if (codepoint) {
		glyph->x1 = (float)floor((float)x0 * scale);
		glyph->y1 = (float)floor((float)-y1 * scale);
	} else glyph->y1 = (float)-((float)h * 0.75f);

	glyph->codepoint = codepoint;
	glyph->size = size;
	glyph->font = font;

	renderer->proc.bitmap_to_atlas(renderer->ctx, font->atlas, (u32)font->atlasWidth, (u32)font->atlasHeight, font->maxHeight, bitmap, glyph->w, glyph->h, &font->atlasX, &font->atlasY);
	RFONT_FREE(bitmap);
	glyph->x = (i32)(font->atlasX - glyph->w);
	glyph->x2 = (i32)(font->atlasX);

	glyph->y = (i32)(font->atlasY);
	glyph->y2 = (i32)((font->atlasY) + glyph->h);

	if (glyph->src < font->numOfLongHorMetrics)
		advanceX = RFONT_SHORT(font->src->info.data, font->src->info.hmtx + 4 * glyph->src);
	else
		advanceX = RFONT_SHORT(font->src->info.data, font->src->info.hmtx + 4 * (i32)(font->numOfLongHorMetrics - 1));

	glyph->advance = (u32)((float)advanceX * scale);

	return *glyph;
}

void RFont_text_area(RFont_renderer* renderer, RFont_font* font, const char* text, u32 size, u32* w, u32* h) {
   RFont_text_area_len(renderer, font, text, 0, size, 0, 0.0f, w, h);
}

void RFont_text_area_spacing(RFont_renderer* renderer, RFont_font* font, const char* text, float spacing, u32 size, u32* w, u32* h) {
   RFont_text_area_len(renderer, font, text, 0, size, 0, spacing, w, h);
}

void RFont_text_area_len(RFont_renderer* renderer, RFont_font* font, const char* text, size_t len, u32 size, size_t stopNL, float spacing, u32* w, u32* h) {
	float x = 0;
	size_t y = 1;

	char* str;

	float scale = (((float)size) / font->fheight);
	float space_adv = (scale * font->space_adv);
	RFont_glyph glyph;

	for (str = (char*)text; (len == 0 || (size_t)(str - text) < len) && *str; str++) {
		if (*str == '\n') {
			if (y == stopNL) {
				if (w) *w = (u32)x;
				if (h) *h = (u32)y * size;
				return;
			}

			y++;
			x = 0;
			continue;
		}

		if (*str == ' ' || *str == '\t') {
			x += space_adv + spacing;
			continue;
		}

		glyph = RFont_font_add_char(renderer, font,  *str, size);

		if (glyph.codepoint == 0 && glyph.size == 0)
			continue;

		x += (float)glyph.advance + spacing;
	}

	if (w) *w = (u32)x;
	if(h) *h = (u32)(y * size);
}

void RFont_draw_text(RFont_renderer* renderer, RFont_font* font, const char* text, float x, float y, u32 size) {
   RFont_draw_text_len(renderer, font, text, 0, x, y, size, 0.0f);
}

void RFont_draw_text_spacing(RFont_renderer* renderer, RFont_font* font, const char* text, float x, float y, u32 size, float spacing) {
   RFont_draw_text_len(renderer, font, text, 0, x, y, size, spacing);
}

char* RFont_codepoint_to_utf8(u32 codepoint) {
   static char utf8[5];
   if (codepoint <= 0x7F) {
      utf8[0] = (char)codepoint;
      utf8[1] = 0;
   } else if (codepoint <= 0x7FF) {
      utf8[0] = (char)(0xC0 | (codepoint >> 6));
      utf8[1] = (char)(0x80 | (codepoint & 0x3F));
      utf8[2] = 0;
   } else if (codepoint <= 0xFFFF) {
      utf8[0] = (char)(0xE0 | (codepoint >> 12));
      utf8[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
      utf8[2] = (char)(0x80 | (codepoint & 0x3F));
      utf8[3] = 0;
   } else if (codepoint <= 0x10FFFF) {
      utf8[0] = (char)(0xF0 | (codepoint >> 18));
      utf8[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
      utf8[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
      utf8[3] = (char)(0x80 | (codepoint & 0x3F));
      utf8[4] = 0;
   } else {
      utf8[0] = 0;
   }

   return utf8;
}

void RFont_draw_text_len(RFont_renderer* renderer, RFont_font* font, const char* text, size_t len, float x, float y, u32 size, float spacing) {
   float* verts = font->verts;
   float* tcoords = font->tcoords;

   float startX = x;
   float startY = y;

   u32 i = 0;
   u32 tIndex = 0;

   char* str;
   RFont_glyph glyph;
   float realX, realY;

   float scale = (((float)size) / font->fheight);
   float space_adv = (scale * font->space_adv);

   float descent_offset =  (-font->descent * scale);

   RFONT_UNUSED(startY);

   y = (y + (float)size - descent_offset);

   for (str = (char*)text; (len == 0 || (size_t)(str - text) < len) && *str; str++) {
      if (*str == '\n') {
         x = startX;
         y += (float)size;
         continue;
      }

      if (*str == ' ' || *str == '\t') {
         x += space_adv + spacing;
         continue;
      }

      glyph = RFont_font_add_char(renderer, font, *str, size);

      if (glyph.codepoint == 0 && glyph.size == 0)
         continue;

      if (glyph.font != font) {
         RFont_draw_text_len(renderer, glyph.font, RFont_codepoint_to_utf8(glyph.codepoint), 4, x, y - (float)size + descent_offset, size, spacing);
         x += glyph.advance + spacing;
         continue;
      }

      realX = x + glyph.x1;
      realY = y + glyph.y1;

      verts[i] = (i32)realX;
      verts[i + 1] = realY;
      verts[i + 2] = 0;
      /*  */
      verts[i + 3] = (i32)realX;
      verts[i + 4] = realY + glyph.h;
      verts[i + 5] = 0;
      /*  */
      verts[i + 6] = (i32)(realX + glyph.w);
      verts[i + 7] = realY + glyph.h;
      verts[i + 8] = 0;
      /*  */
      /*  */
      verts[i + 9] = (i32)(realX + glyph.w);
      verts[i + 10] = realY;
      verts[i + 11] = 0;
      /*  */
      verts[i + 12] = (i32)realX;
      verts[i + 13] = realY;
      verts[i + 14] = 0;
      /*  */

      verts[i + 15] = (i32)(realX + glyph.w);
      verts[i + 16] = realY + glyph.h;
      verts[i + 17] = 0;

      /* texture coords */

      tcoords[tIndex] = RFONT_GET_TEXPOSX(glyph.x, font->atlasWidth);
      tcoords[tIndex + 1] = RFONT_GET_TEXPOSY(glyph.y, font->atlasWidth);

      /*  */
      tcoords[tIndex + 2] = RFONT_GET_TEXPOSX(glyph.x, font->atlasWidth);
      tcoords[tIndex + 3] = RFONT_GET_TEXPOSY(glyph.y2, font->atlasHeight);
      /*  */
      tcoords[tIndex + 4] = RFONT_GET_TEXPOSX(glyph.x2, font->atlasWidth);
      tcoords[tIndex + 5] = RFONT_GET_TEXPOSY(glyph.y2, font->atlasHeight);
      /*  */
      /*  */
      tcoords[tIndex + 6] = RFONT_GET_TEXPOSX(glyph.x2, font->atlasWidth);
      tcoords[tIndex + 7] = RFONT_GET_TEXPOSY(glyph.y, font->atlasWidth);
      /*  */
      tcoords[tIndex + 8] = RFONT_GET_TEXPOSX(glyph.x, font->atlasWidth);
      tcoords[tIndex + 9] = RFONT_GET_TEXPOSY(glyph.y, font->atlasWidth);
      /*  */
      tcoords[tIndex + 10] = RFONT_GET_TEXPOSX(glyph.x2, font->atlasWidth);
      tcoords[tIndex + 11] = RFONT_GET_TEXPOSY(glyph.y2, font->atlasHeight);

      x += glyph.advance + spacing;
      i += 18;
      tIndex += 12;
   }

	if (i && renderer->proc.render)
      renderer->proc.render(renderer->ctx, font->atlas, verts, tcoords, i / 3);
}

/*
stb_truetype defines and source code required by RFont

you probably don't care about this part if you're reading just the RFont code
*/

#ifndef RFONT_EXTERNAL_STB
   typedef char rstbtt__check_size32[sizeof(i32)==4 ? 1 : -1];
   typedef char rstbtt__check_size16[sizeof(i16)==2 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif

enum {
    rstbtt_vmove=1,
    rstbtt_vline,
    rstbtt_vcurve,
    rstbtt_vcubic
};

typedef struct rstbtt_vertex {
    float x,y,cx,cy,cx1,cy1;
    unsigned char type,padding;
} rstbtt_vertex;

RFONT_API int rstbtt_GetGlyphShape(const rstbtt_fontinfo *info, int glyph_index, rstbtt_vertex **vertices);

/* @TODO: don't expose this structure */
typedef struct
{
   int w,h,stride;
   unsigned char *pixels;
} rstbtt__bitmap;

/* rasterize a shape with quadratic beziers into a bitmap */
RFONT_API void rstbtt_Rasterize(rstbtt__bitmap *result,        /* 1-channel bitmap to draw into */
                               float flatness_in_pixels,     /* allowable error of curve in pixels */
                               rstbtt_vertex *vertices,       /* array of vertices defining shape */
                               int num_verts,                /* number of vertices in above array */
                               float scale_x, float scale_y, /* scale applied to input vertices */
                               float shift_x, float shift_y, /* translation applied to input vertices */
                               int x_off, int y_off,         /* another translation applied to input */
                               int invert);                   /* if non-zero, vertically flip shape */

enum { /* platformID */
   rstbtt_PLATFORM_ID_UNICODE   =0,
   rstbtt_PLATFORM_ID_MAC       =1,
   rstbtt_PLATFORM_ID_ISO       =2,
   rstbtt_PLATFORM_ID_MICROSOFT =3
};

enum { /* encodingID for rstbtt_PLATFORM_ID_UNICODE */
   rstbtt_UNICODE_EID_UNICODE_1_0    =0,
   rstbtt_UNICODE_EID_UNICODE_1_1    =1,
   rstbtt_UNICODE_EID_ISO_10646      =2,
   rstbtt_UNICODE_EID_UNICODE_2_0_BMP=3,
   rstbtt_UNICODE_EID_UNICODE_2_0_FULL=4
};

enum { /* encodingID for rstbtt_PLATFORM_ID_MICROSOFT */
   rstbtt_MS_EID_SYMBOL        =0,
   rstbtt_MS_EID_UNICODE_BMP   =1,
   rstbtt_MS_EID_SHIFTJIS      =2,
   rstbtt_MS_EID_UNICODE_FULL  =10
};

enum { /* encodingID for rstbtt_PLATFORM_ID_MAC; same as Script Manager codes */
   rstbtt_MAC_EID_ROMAN        =0,   rstbtt_MAC_EID_ARABIC       =4,
   rstbtt_MAC_EID_JAPANESE     =1,   rstbtt_MAC_EID_HEBREW       =5,
   rstbtt_MAC_EID_CHINESE_TRAD =2,   rstbtt_MAC_EID_GREEK        =6,
   rstbtt_MAC_EID_KOREAN       =3,   rstbtt_MAC_EID_RUSSIAN      =7
};

enum { /* languageID for rstbtt_PLATFORM_ID_MICROSOFT; same as LCID...
            problematic because there are e.g. 16 english LCIDs and 16 arabic LCIDs */
   rstbtt_MS_LANG_ENGLISH     =0x0409,   rstbtt_MS_LANG_ITALIAN     =0x0410,
   rstbtt_MS_LANG_CHINESE     =0x0804,   rstbtt_MS_LANG_JAPANESE    =0x0411,
   rstbtt_MS_LANG_DUTCH       =0x0413,   rstbtt_MS_LANG_KOREAN      =0x0412,
   rstbtt_MS_LANG_FRENCH      =0x040c,   rstbtt_MS_LANG_RUSSIAN     =0x0419,
   rstbtt_MS_LANG_GERMAN      =0x0407,   rstbtt_MS_LANG_SPANISH     =0x0409,
   rstbtt_MS_LANG_HEBREW      =0x040d,   rstbtt_MS_LANG_SWEDISH     =0x041D
};

enum { /* languageID for rstbtt_PLATFORM_ID_MAC */
   rstbtt_MAC_LANG_ENGLISH      =0 ,   rstbtt_MAC_LANG_JAPANESE     =11,
   rstbtt_MAC_LANG_ARABIC       =12,   rstbtt_MAC_LANG_KOREAN       =23,
   rstbtt_MAC_LANG_DUTCH        =4 ,   rstbtt_MAC_LANG_RUSSIAN      =32,
   rstbtt_MAC_LANG_FRENCH       =1 ,   rstbtt_MAC_LANG_SPANISH      =6 ,
   rstbtt_MAC_LANG_GERMAN       =2 ,   rstbtt_MAC_LANG_SWEDISH      =5 ,
   rstbtt_MAC_LANG_HEBREW       =10,   rstbtt_MAC_LANG_CHINESE_SIMPLIFIED =33,
   rstbtt_MAC_LANG_ITALIAN      =3 ,   rstbtt_MAC_LANG_CHINESE_TRAD =19
};

#ifdef __cplusplus
}
#endif

#ifndef rstbtt_MAX_OVERSAMPLE
#define rstbtt_MAX_OVERSAMPLE   8
#endif

#if rstbtt_MAX_OVERSAMPLE > 255
#error "rstbtt_MAX_OVERSAMPLE cannot be > 255"
#endif

typedef int rstbtt__test_oversample_pow2[(rstbtt_MAX_OVERSAMPLE & (rstbtt_MAX_OVERSAMPLE-1)) == 0 ? 1 : -1];

#ifndef rstbtt_RASTERIZER_VERSION
#define rstbtt_RASTERIZER_VERSION 2
#endif

#ifdef _MSC_VER
#define rstbtt__NOTUSED(v)  (void)(v)
#else
#define rstbtt__NOTUSED(v)  (void)sizeof(v)
#endif

#undef RFONT_API
#define RFONT_API extern



RFONT_API u8 rstbtt__buf_get8(rstbtt__buf *b)
{
   if (b->cursor >= b->size)
      return 0;
   return b->data[b->cursor++];
}

RFONT_API u8 rstbtt__buf_peek8(rstbtt__buf *b)
{
   if (b->cursor >= b->size)
      return 0;
   return b->data[b->cursor];
}

RFONT_API void rstbtt__buf_seek(rstbtt__buf *b, int o)
{
   assert(!(o > b->size || o < 0));
   b->cursor = (o > b->size || o < 0) ? b->size : o;
}

RFONT_API void rstbtt__buf_skip(rstbtt__buf *b, int o)
{
   rstbtt__buf_seek(b, b->cursor + o);
}

RFONT_API u32 rstbtt__buf_get(rstbtt__buf *b, int n)
{
   u32 v = 0;
   int i;
   assert(n >= 1 && n <= 4);
   for (i = 0; i < n; i++)
      v = (v << 8) | rstbtt__buf_get8(b);
   return v;
}

RFONT_API rstbtt__buf rstbtt__new_buf(const void *p, size_t size)
{
   rstbtt__buf r;
   assert(size < 0x40000000);
   r.data = (u8*) p;
   r.size = (int) size;
   r.cursor = 0;
   return r;
}

#define rstbtt__buf_get16(b)  (u16)rstbtt__buf_get((b), 2)
#define rstbtt__buf_get32(b)  (u32)rstbtt__buf_get((b), 4)

RFONT_API rstbtt__buf rstbtt__buf_range(const rstbtt__buf *b, int o, int s)
{
   rstbtt__buf r = rstbtt__new_buf(NULL, 0);
   if (o < 0 || s < 0 || o > b->size || s > b->size - o) return r;
   r.data = b->data + o;
   r.size = s;
   return r;
}

RFONT_API rstbtt__buf rstbtt__cff_get_index(rstbtt__buf *b)
{
   int count, start, offsize;
   start = b->cursor;
   count = rstbtt__buf_get16(b);
   if (count) {
      offsize = rstbtt__buf_get8(b);
      assert(offsize >= 1 && offsize <= 4);
      rstbtt__buf_skip(b, offsize * count);
      rstbtt__buf_skip(b, (int)rstbtt__buf_get(b, offsize) - 1);
   }
   return rstbtt__buf_range(b, start, b->cursor - start);
}

RFONT_API u32 rstbtt__cff_int(rstbtt__buf *b)
{
   int b0 = rstbtt__buf_get8(b);
   if (b0 >= 32 && b0 <= 246)       return (u32)b0 - 139;
   else if (b0 >= 247 && b0 <= 250) return (u32)((b0 - 247)*256 + rstbtt__buf_get8(b) + 108);
   else if (b0 >= 251 && b0 <= 254) return (u32)(-(b0 - 251)*256 - rstbtt__buf_get8(b) - 108);
   else if (b0 == 28)               return rstbtt__buf_get16(b);
   else if (b0 == 29)               return rstbtt__buf_get32(b);
   assert(0);
   return 0;
}

RFONT_API void rstbtt__cff_skip_operand(rstbtt__buf *b) {
   int v, b0 = rstbtt__buf_peek8(b);
   assert(b0 >= 28);
   if (b0 == 30) {
      rstbtt__buf_skip(b, 1);
      while (b->cursor < b->size) {
         v = rstbtt__buf_get8(b);
         if ((v & 0xF) == 0xF || (v >> 4) == 0xF)
            break;
      }
   } else {
      rstbtt__cff_int(b);
   }
}

RFONT_API rstbtt__buf rstbtt__dict_get(rstbtt__buf *b, int key)
{
   rstbtt__buf_seek(b, 0);
   while (b->cursor < b->size) {
      int start = b->cursor, end, op;
      while (rstbtt__buf_peek8(b) >= 28)
         rstbtt__cff_skip_operand(b);
      end = b->cursor;
      op = rstbtt__buf_get8(b);
      if (op == 12)  op = rstbtt__buf_get8(b) | 0x100;
      if (op == key) return rstbtt__buf_range(b, start, end-start);
   }
   return rstbtt__buf_range(b, 0, 0);
}

RFONT_API void rstbtt__dict_get_ints(rstbtt__buf *b, int key, int outcount, u32 *out)
{
   int i;
   rstbtt__buf operands = rstbtt__dict_get(b, key);
   for (i = 0; i < outcount && operands.cursor < operands.size; i++)
      out[i] = rstbtt__cff_int(&operands);
}

RFONT_API int rstbtt__cff_index_count(rstbtt__buf *b)
{
   rstbtt__buf_seek(b, 0);
   return rstbtt__buf_get16(b);
}

RFONT_API rstbtt__buf rstbtt__cff_index_get(rstbtt__buf b, int i)
{
   int count, offsize, start, end;
   rstbtt__buf_seek(&b, 0);
   count = rstbtt__buf_get16(&b);
   offsize = rstbtt__buf_get8(&b);
   assert(i >= 0 && i < count);
   assert(offsize >= 1 && offsize <= 4);
   rstbtt__buf_skip(&b, i*offsize);
   start = (int)rstbtt__buf_get(&b, offsize);
   end = (int)rstbtt__buf_get(&b, offsize);
   return rstbtt__buf_range(&b, 2+(count+1)*offsize+start, end - start);
}

#define rstbtt_tag4(p,c0,c1,c2,c3) ((p)[0] == (c0) && (p)[1] == (c1) && (p)[2] == (c2) && (p)[3] == (c3))
#define rstbtt_tag(p,str)          rstbtt_tag4(p,str[0],str[1],str[2],str[3])

/* @OPTIMIZE: binary search */
RFONT_API u32 rstbtt__find_table(u8 *data, u32 fontstart, const char *tag)
{
   i32 num_tables = RFONT_USHORT(data, fontstart+4);
   u32 tabledir = fontstart + 12;
   i32 i;
   for (i=0; i < num_tables; ++i) {
      u32 loc = tabledir + 16 * (u32)i;
      if (rstbtt_tag(data+loc+0, tag))
         return RFONT_ULONG(data, loc+8);
   }
   return 0;
}

RFONT_API rstbtt__buf rstbtt__get_subrs(rstbtt__buf cff, rstbtt__buf fontdict)
{
   u32 subrsoff = 0, private_loc[2] = { 0, 0 };
   rstbtt__buf pdict;
   rstbtt__dict_get_ints(&fontdict, 18, 2, private_loc);
   if (!private_loc[1] || !private_loc[0]) return rstbtt__new_buf(NULL, 0);
   pdict = rstbtt__buf_range(&cff, (int)private_loc[1], (int)private_loc[0]);
   rstbtt__dict_get_ints(&pdict, 19, 1, &subrsoff);
   if (!subrsoff) return rstbtt__new_buf(NULL, 0);
   rstbtt__buf_seek(&cff, (int)(private_loc[1] + subrsoff));
   return rstbtt__cff_get_index(&cff);
}

RFONT_API void rstbtt_setvertex(rstbtt_vertex *v, u8 type, i32 x, i32 y, i32 cx, i32 cy)
{
   v->type = type;
   v->x = (i16) x;
   v->y = (i16) y;
   v->cx = (i16) cx;
   v->cy = (i16) cy;
}

RFONT_API int rstbtt__close_shape(rstbtt_vertex *vertices, int num_vertices, int was_off, int start_off,
    i32 sx, i32 sy, i32 scx, i32 scy, i32 cx, i32 cy)
{
   if (start_off) {
      if (was_off)
         rstbtt_setvertex(&vertices[num_vertices++], rstbtt_vcurve, (cx+scx)>>1, (cy+scy)>>1, cx,cy);
      rstbtt_setvertex(&vertices[num_vertices++], rstbtt_vcurve, sx,sy,scx,scy);
   } else {
      if (was_off)
         rstbtt_setvertex(&vertices[num_vertices++], rstbtt_vcurve,sx,sy,cx,cy);
      else
         rstbtt_setvertex(&vertices[num_vertices++], rstbtt_vline,sx,sy,0,0);
   }
   return num_vertices;
}

RFONT_API int rstbtt__GetGlyfOffset(const rstbtt_fontinfo *info, int glyph_index);

RFONT_API int rstbtt__GetGlyphShapeTT(const rstbtt_fontinfo *info, int glyph_index, rstbtt_vertex **pvertices)
{
   i16 numberOfContours;
   u8 *endPtsOfContours;
   u8 *data = info->data;
   rstbtt_vertex *vertices=0;
   int num_vertices=0;
   int g = rstbtt__GetGlyfOffset(info, glyph_index);

   *pvertices = NULL;

   if (g < 0) return 0;

   numberOfContours = RFONT_SHORT(data, g);

   if (numberOfContours > 0) {
      u8 flags=0,flagcount;
      i32 ins, i,j=0,m,n, next_move, was_off=0, off, start_off=0;
      i32 x,y,cx,cy,sx,sy, scx,scy;
      u8 *points;
      endPtsOfContours = (data + g + 10);
      ins = RFONT_USHORT(data, g + 10 + numberOfContours * 2);
      points = data + g + 10 + numberOfContours * 2 + 2 + ins;

      n = 1+RFONT_USHORT(endPtsOfContours, numberOfContours*2-2);

      m = n + 2*numberOfContours;  /* a loose bound on how many vertices we might need */
      vertices = (rstbtt_vertex *) RFONT_MALLOC((size_t)m * sizeof(vertices[0]));
      if (vertices == 0)
         return 0;

      next_move = 0;
      flagcount=0;

      off = m - n; /* starting offset for uninterpreted data, regardless of how m ends up being calculated */

      /* first load flags */

      for (i=0; i < n; ++i) {
         if (flagcount == 0) {
            flags = *points++;
            if (flags & 8)
               flagcount = *points++;
         } else
            --flagcount;
         vertices[off+i].type = flags;
      }

      /* now load x coordinates */
      x=0;
      for (i=0; i < n; ++i) {
         flags = vertices[off+i].type;
         if (flags & 2) {
            i16 dx = *points++;
            x += (flags & 16) ? dx : -dx; /* ??? */
         } else {
            if (!(flags & 16)) {
               x = x + (i16) (points[0]*256 + points[1]);
               points += 2;
            }
         }
         vertices[off+i].x = (i16) x;
      }

      /* now load y coordinates */
      y=0;
      for (i=0; i < n; ++i) {
         flags = vertices[off+i].type;
         if (flags & 4) {
            i16 dy = *points++;
            y += (flags & 32) ? dy : -dy; /* ??? */
         } else {
            if (!(flags & 32)) {
               y = y + (i16) (points[0]*256 + points[1]);
               points += 2;
            }
         }
         vertices[off+i].y = (i16) y;
      }

      /* now convert them to our format */
      num_vertices=0;
      sx = sy = cx = cy = scx = scy = 0;
      for (i=0; i < n; ++i) {
         flags = vertices[off+i].type;
         x     = (i16) vertices[off+i].x;
         y     = (i16) vertices[off+i].y;

         if (next_move == i) {
            if (i != 0)
               num_vertices = rstbtt__close_shape(vertices, num_vertices, was_off, start_off, sx,sy,scx,scy,cx,cy);

            /* now start the new one */
            start_off = !(flags & 1);
            if (start_off) {
               /* if we start off with an off-curve point, then when we need to find a point on the curve */
               /* where we can start, and we need to save some state for when we wraparound. */
               scx = x;
               scy = y;
               if (!(vertices[off+i+1].type & 1)) {
                  /* next point is also a curve point, so interpolate an on-point curve */
                  sx = (x + (i32) vertices[off+i+1].x) >> 1;
                  sy = (y + (i32) vertices[off+i+1].y) >> 1;
               } else {
                  /* otherwise just use the next point as our start point */
                  sx = (i32) vertices[off+i+1].x;
                  sy = (i32) vertices[off+i+1].y;
                  ++i; /* we're using point i+1 as the starting point, so skip it */
               }
            } else {
               sx = x;
               sy = y;
            }
            rstbtt_setvertex(&vertices[num_vertices++], rstbtt_vmove,sx,sy,0,0);
            was_off = 0;
            next_move = 1 + RFONT_USHORT(endPtsOfContours, j*2);
            ++j;
         } else {
            if (!(flags & 1)) { /* if it's a curve */
               if (was_off) /* two off-curve control points in a row means interpolate an on-curve midpoint */
                  rstbtt_setvertex(&vertices[num_vertices++], rstbtt_vcurve, (cx+x)>>1, (cy+y)>>1, cx, cy);
               cx = x;
               cy = y;
               was_off = 1;
            } else {
               if (was_off)
                  rstbtt_setvertex(&vertices[num_vertices++], rstbtt_vcurve, x,y, cx, cy);
               else
                  rstbtt_setvertex(&vertices[num_vertices++], rstbtt_vline, x,y,0,0);
               was_off = 0;
            }
         }
      }
      num_vertices = rstbtt__close_shape(vertices, num_vertices, was_off, start_off, sx,sy,scx,scy,cx,cy);
   } else if (numberOfContours < 0) {
      /* Compound shapes. */
      int more = 1;
      u8 *comp = data + g + 10;
      num_vertices = 0;
      vertices = 0;
      while (more) {
         u16 flags, gidx;
         int comp_num_verts = 0, i;
         rstbtt_vertex *comp_verts = 0, *tmp = 0;
         float mtx[6] = {1,0,0,1,0,0}, m, n;

         flags = (u16)RFONT_SHORT(comp, 0); comp+=2;
         gidx = (u16)RFONT_SHORT(comp, 0); comp+=2;

         if (flags & 2) { /* XY values */
            if (flags & 1) { /* shorts */
               mtx[4] = RFONT_SHORT(comp, 0); comp+=2;
               mtx[5] = RFONT_SHORT(comp, 0); comp+=2;
            } else {
               mtx[4] = RFONT_CHAR(comp, 0); comp+=1;
               mtx[5] = RFONT_CHAR(comp, 0); comp+=1;
            }
         }
         else {
            /* @TODO handle matching point */
            assert(0);
         }
         if (flags & (1<<3)) { /* WE_HAVE_A_SCALE */
            mtx[0] = mtx[3] = RFONT_SHORT(comp, 0 )/16384.0f; comp+=2;
            mtx[1] = mtx[2] = 0;
         } else if (flags & (1<<6)) { /* WE_HAVE_AN_X_AND_YSCALE */
            mtx[0] = RFONT_SHORT(comp, 0)/16384.0f; comp+=2;
            mtx[1] = mtx[2] = 0;
            mtx[3] = RFONT_SHORT(comp, 0)/16384.0f; comp+=2;
         } else if (flags & (1<<7)) { /* WE_HAVE_A_TWO_BY_TWO */
            mtx[0] = RFONT_SHORT(comp, 0)/16384.0f; comp+=2;
            mtx[1] = RFONT_SHORT(comp, 0)/16384.0f; comp+=2;
            mtx[2] = RFONT_SHORT(comp, 0)/16384.0f; comp+=2;
            mtx[3] = RFONT_SHORT(comp, 0)/16384.0f; comp+=2;
         }

         /* Find transformation scales. */
         m = (float) sqrt(mtx[0]*mtx[0] + mtx[1]*mtx[1]);
         n = (float) sqrt(mtx[2]*mtx[2] + mtx[3]*mtx[3]);

         /* Get indexed glyph. */
         comp_num_verts = rstbtt_GetGlyphShape(info, gidx, &comp_verts);
         if (comp_num_verts > 0) {
            /* Transform vertices. */
            for (i = 0; i < comp_num_verts; ++i) {
               rstbtt_vertex* v = &comp_verts[i];
               float x,y;
               x=v->x; y=v->y;
               v->x = (float)(m * (mtx[0]*x + mtx[2]*y + mtx[4]));
               v->y = (float)(n * (mtx[1]*x + mtx[3]*y + mtx[5]));
               x=v->cx; y=v->cy;
               v->cx = (float)(m * (mtx[0] * (float)x + mtx[2] * (float)y + mtx[4]));
               v->cy = (float)(n * (mtx[1] * (float)x + mtx[3] * (float)y + mtx[5]));
            }
            /* Append vertices. */
            tmp = (rstbtt_vertex*)RFONT_MALLOC((size_t)(num_vertices + comp_num_verts) * sizeof(rstbtt_vertex));
            if (!tmp) {
               if (vertices) RFONT_FREE(vertices);
               if (comp_verts) RFONT_FREE(comp_verts);
               return 0;
            }
            if (num_vertices > 0) RFONT_MEMCPY(tmp, vertices, (size_t)num_vertices * sizeof(rstbtt_vertex));
            RFONT_MEMCPY(tmp+num_vertices, comp_verts, (size_t)comp_num_verts * sizeof(rstbtt_vertex));
            if (vertices) RFONT_FREE(vertices);
            vertices = tmp;
            RFONT_FREE(comp_verts);
            num_vertices += comp_num_verts;
         }
         /* More components ? */
         more = flags & (1<<5);
      }
   } else {
      /* numberOfCounters == 0, do nothing */
   }

   *pvertices = vertices;
   return num_vertices;
}

typedef struct
{
   int bounds;
   int started;
   float first_x, first_y;
   float x, y;
   i32 min_x, max_x, min_y, max_y;

   rstbtt_vertex *pvertices;
   int num_vertices;
} rstbtt__csctx;

#define rstbtt__CSCTX_INIT(bounds) {bounds,0, 0,0, 0,0, 0,0,0,0, NULL, 0}

RFONT_API void rstbtt__track_vertex(rstbtt__csctx *c, i32 x, i32 y)
{
   if (x > c->max_x || !c->started) c->max_x = x;
   if (y > c->max_y || !c->started) c->max_y = y;
   if (x < c->min_x || !c->started) c->min_x = x;
   if (y < c->min_y || !c->started) c->min_y = y;
   c->started = 1;
}

RFONT_API void rstbtt__csctx_v(rstbtt__csctx *c, u8 type, i32 x, i32 y, i32 cx, i32 cy, i32 cx1, i32 cy1)
{
   if (c->bounds) {
      rstbtt__track_vertex(c, x, y);
      if (type == rstbtt_vcubic) {
         rstbtt__track_vertex(c, cx, cy);
         rstbtt__track_vertex(c, cx1, cy1);
      }
   } else {
      rstbtt_setvertex(&c->pvertices[c->num_vertices], type, x, y, cx, cy);
      c->pvertices[c->num_vertices].cx1 = (i16) cx1;
      c->pvertices[c->num_vertices].cy1 = (i16) cy1;
   }
   c->num_vertices++;
}

RFONT_API void rstbtt__csctx_close_shape(rstbtt__csctx *ctx)
{
   if (ctx->first_x != ctx->x || ctx->first_y != ctx->y)
      rstbtt__csctx_v(ctx, rstbtt_vline, (int)ctx->first_x, (int)ctx->first_y, 0, 0, 0, 0);
}

RFONT_API void rstbtt__csctx_rmove_to(rstbtt__csctx *ctx, float dx, float dy)
{
   rstbtt__csctx_close_shape(ctx);
   ctx->first_x = ctx->x = ctx->x + dx;
   ctx->first_y = ctx->y = ctx->y + dy;
   rstbtt__csctx_v(ctx, rstbtt_vmove, (int)ctx->x, (int)ctx->y, 0, 0, 0, 0);
}

RFONT_API void rstbtt__csctx_rline_to(rstbtt__csctx *ctx, float dx, float dy)
{
   ctx->x += dx;
   ctx->y += dy;
   rstbtt__csctx_v(ctx, rstbtt_vline, (int)ctx->x, (int)ctx->y, 0, 0, 0, 0);
}

RFONT_API void rstbtt__csctx_rccurve_to(rstbtt__csctx *ctx, float dx1, float dy1, float dx2, float dy2, float dx3, float dy3)
{
   float cx1 = ctx->x + dx1;
   float cy1 = ctx->y + dy1;
   float cx2 = cx1 + dx2;
   float cy2 = cy1 + dy2;
   ctx->x = cx2 + dx3;
   ctx->y = cy2 + dy3;
   rstbtt__csctx_v(ctx, rstbtt_vcubic, (int)ctx->x, (int)ctx->y, (int)cx1, (int)cy1, (int)cx2, (int)cy2);
}

RFONT_API rstbtt__buf rstbtt__get_subr(rstbtt__buf idx, int n)
{
   int count = rstbtt__cff_index_count(&idx);
   int bias = 107;
   if (count >= 33900)
      bias = 32768;
   else if (count >= 1240)
      bias = 1131;
   n += bias;
   if (n < 0 || n >= count)
      return rstbtt__new_buf(NULL, 0);
   return rstbtt__cff_index_get(idx, n);
}

RFONT_API rstbtt__buf rstbtt__cid_get_glyph_subrs(const rstbtt_fontinfo *info, int glyph_index)
{
   rstbtt__buf fdselect = info->fdselect;
   int nranges, start, end, v, fmt, fdselector = -1, i;

   rstbtt__buf_seek(&fdselect, 0);
   fmt = rstbtt__buf_get8(&fdselect);
   if (fmt == 0) {
      /* untested */
      rstbtt__buf_skip(&fdselect, glyph_index);
      fdselector = rstbtt__buf_get8(&fdselect);
   } else if (fmt == 3) {
      nranges = rstbtt__buf_get16(&fdselect);
      start = rstbtt__buf_get16(&fdselect);
      for (i = 0; i < nranges; i++) {
         v = rstbtt__buf_get8(&fdselect);
         end = rstbtt__buf_get16(&fdselect);
         if (glyph_index >= start && glyph_index < end) {
            fdselector = v;
            break;
         }
         start = end;
      }
   }
   if (fdselector == -1) rstbtt__new_buf(NULL, 0);
   return rstbtt__get_subrs(info->cff, rstbtt__cff_index_get(info->fontdicts, fdselector));
}

RFONT_API int rstbtt__run_charstring(const rstbtt_fontinfo *info, int glyph_index, rstbtt__csctx *c)
{
   int in_header = 1, maskbits = 0, subr_stack_height = 0, sp = 0, v, i, b0;
   int has_subrs = 0, clear_stack;
   float s[48];
   rstbtt__buf subr_stack[10], subrs = info->subrs, b;
   float f;

#define rstbtt__CSERR(s) (0)

   /* this currently ignores the initial width value, which isn't needed if we have hmtx */
   b = rstbtt__cff_index_get(info->charstrings, glyph_index);
   while (b.cursor < b.size) {
      i = 0;
      clear_stack = 1;
      b0 = rstbtt__buf_get8(&b);
      switch (b0) {
      /* @TODO implement hinting */
      case 0x13: /* hintmask */
      case 0x14: /* cntrmask */
         if (in_header)
            maskbits += (sp / 2); /* implicit "vstem" */
         in_header = 0;
         rstbtt__buf_skip(&b, (maskbits + 7) / 8);
         break;

      case 0x01: /* hstem */
      case 0x03: /* vstem */
      case 0x12: /* hstemhm */
      case 0x17: /* vstemhm */
         maskbits += (sp / 2);
         break;

      case 0x15: /* rmoveto */
         in_header = 0;
         if (sp < 2) return rstbtt__CSERR("rmoveto stack");
         rstbtt__csctx_rmove_to(c, s[sp-2], s[sp-1]);
         break;
      case 0x04: /* vmoveto */
         in_header = 0;
         if (sp < 1) return rstbtt__CSERR("vmoveto stack");
         rstbtt__csctx_rmove_to(c, 0, s[sp-1]);
         break;
      case 0x16: /* hmoveto */
         in_header = 0;
         if (sp < 1) return rstbtt__CSERR("hmoveto stack");
         rstbtt__csctx_rmove_to(c, s[sp-1], 0);
         break;

      case 0x05: /* rlineto */
         if (sp < 2) return rstbtt__CSERR("rlineto stack");
         for (; i + 1 < sp; i += 2)
            rstbtt__csctx_rline_to(c, s[i], s[i+1]);
         break;

      /* hlineto/vlineto and vhcurveto/hvcurveto alternate horizontal and vertical
        starting from a different place. */

      case 0x07: /* vlineto */
         if (sp < 1) return rstbtt__CSERR("vlineto stack");
         goto vlineto;
      case 0x06: /* hlineto */
         if (sp < 1) return rstbtt__CSERR("hlineto stack");
         for (;;) {
            if (i >= sp) break;
            rstbtt__csctx_rline_to(c, s[i], 0);
            i++;
      vlineto:
            if (i >= sp) break;
            rstbtt__csctx_rline_to(c, 0, s[i]);
            i++;
         }
         break;

      case 0x1F: /* hvcurveto */
         if (sp < 4) return rstbtt__CSERR("hvcurveto stack");
         goto hvcurveto;
      case 0x1E: /* vhcurveto */
         if (sp < 4) return rstbtt__CSERR("vhcurveto stack");
         for (;;) {
            if (i + 3 >= sp) break;
            rstbtt__csctx_rccurve_to(c, 0, s[i], s[i+1], s[i+2], s[i+3], (sp - i == 5) ? s[i + 4] : 0.0f);
            i += 4;
      hvcurveto:
            if (i + 3 >= sp) break;
            rstbtt__csctx_rccurve_to(c, s[i], 0, s[i+1], s[i+2], (sp - i == 5) ? s[i+4] : 0.0f, s[i+3]);
            i += 4;
         }
         break;

      case 0x08: /* rrcurveto */
         if (sp < 6) return rstbtt__CSERR("rcurveline stack");
         for (; i + 5 < sp; i += 6)
            rstbtt__csctx_rccurve_to(c, s[i], s[i+1], s[i+2], s[i+3], s[i+4], s[i+5]);
         break;

      case 0x18: /* rcurveline */
         if (sp < 8) return rstbtt__CSERR("rcurveline stack");
         for (; i + 5 < sp - 2; i += 6)
            rstbtt__csctx_rccurve_to(c, s[i], s[i+1], s[i+2], s[i+3], s[i+4], s[i+5]);
         if (i + 1 >= sp) return rstbtt__CSERR("rcurveline stack");
         rstbtt__csctx_rline_to(c, s[i], s[i+1]);
         break;

      case 0x19: /* rlinecurve */
         if (sp < 8) return rstbtt__CSERR("rlinecurve stack");
         for (; i + 1 < sp - 6; i += 2)
            rstbtt__csctx_rline_to(c, s[i], s[i+1]);
         if (i + 5 >= sp) return rstbtt__CSERR("rlinecurve stack");
         rstbtt__csctx_rccurve_to(c, s[i], s[i+1], s[i+2], s[i+3], s[i+4], s[i+5]);
         break;

      case 0x1A: /* vvcurveto */
      case 0x1B: /* hhcurveto */
         if (sp < 4) return rstbtt__CSERR("(vv|hh)curveto stack");
         f = 0.0;
         if (sp & 1) { f = s[i]; i++; }
         for (; i + 3 < sp; i += 4) {
            if (b0 == 0x1B)
               rstbtt__csctx_rccurve_to(c, s[i], f, s[i+1], s[i+2], s[i+3], 0.0);
            else
               rstbtt__csctx_rccurve_to(c, f, s[i], s[i+1], s[i+2], 0.0, s[i+3]);
            f = 0.0;
         }
         break;

      case 0x0A: /* callsubr */
         if (!has_subrs) {
            if (info->fdselect.size)
               subrs = rstbtt__cid_get_glyph_subrs(info, glyph_index);
            has_subrs = 1;
         }
         /* fallthrough */
      case 0x1D: /* callgsubr */
         if (sp < 1) return rstbtt__CSERR("call(g|)subr stack");
         v = (int) s[--sp];
         if (subr_stack_height >= 10) return rstbtt__CSERR("recursion limit");
         subr_stack[subr_stack_height++] = b;
         b = rstbtt__get_subr(b0 == 0x0A ? subrs : info->gsubrs, v);
         if (b.size == 0) return rstbtt__CSERR("subr not found");
         b.cursor = 0;
         clear_stack = 0;
         break;

      case 0x0B: /* return */
         if (subr_stack_height <= 0) return rstbtt__CSERR("return outside subr");
         b = subr_stack[--subr_stack_height];
         clear_stack = 0;
         break;

      case 0x0E: /* endchar */
         rstbtt__csctx_close_shape(c);
         return 1;

      case 0x0C: { /* two-byte escape */
         float dx1, dx2, dx3, dx4, dx5, dx6, dy1, dy2, dy3, dy4, dy5, dy6;
         float dx, dy;
         int b1 = rstbtt__buf_get8(&b);
         switch (b1) {
         /* @TODO These "flex" implementations ignore the flex-depth and resolution,
             and always draw beziers. */
         case 0x22: /* hflex */
            if (sp < 7) return rstbtt__CSERR("hflex stack");
            dx1 = s[0];
            dx2 = s[1];
            dy2 = s[2];
            dx3 = s[3];
            dx4 = s[4];
            dx5 = s[5];
            dx6 = s[6];
            rstbtt__csctx_rccurve_to(c, dx1, 0, dx2, dy2, dx3, 0);
            rstbtt__csctx_rccurve_to(c, dx4, 0, dx5, -dy2, dx6, 0);
            break;

         case 0x23: /* flex */
            if (sp < 13) return rstbtt__CSERR("flex stack");
            dx1 = s[0];
            dy1 = s[1];
            dx2 = s[2];
            dy2 = s[3];
            dx3 = s[4];
            dy3 = s[5];
            dx4 = s[6];
            dy4 = s[7];
            dx5 = s[8];
            dy5 = s[9];
            dx6 = s[10];
            dy6 = s[11];
            /* fd is s[12] */
            rstbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, dy3);
            rstbtt__csctx_rccurve_to(c, dx4, dy4, dx5, dy5, dx6, dy6);
            break;

         case 0x24: /* hflex1 */
            if (sp < 9) return rstbtt__CSERR("hflex1 stack");
            dx1 = s[0];
            dy1 = s[1];
            dx2 = s[2];
            dy2 = s[3];
            dx3 = s[4];
            dx4 = s[5];
            dx5 = s[6];
            dy5 = s[7];
            dx6 = s[8];
            rstbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, 0);
            rstbtt__csctx_rccurve_to(c, dx4, 0, dx5, dy5, dx6, -(dy1+dy2+dy5));
            break;

         case 0x25: /* flex1 */
            if (sp < 11) return rstbtt__CSERR("flex1 stack");
            dx1 = s[0];
            dy1 = s[1];
            dx2 = s[2];
            dy2 = s[3];
            dx3 = s[4];
            dy3 = s[5];
            dx4 = s[6];
            dy4 = s[7];
            dx5 = s[8];
            dy5 = s[9];
            dx6 = dy6 = s[10];
            dx = dx1+dx2+dx3+dx4+dx5;
            dy = dy1+dy2+dy3+dy4+dy5;
            if (fabs(dx) > fabs(dy))
               dy6 = -dy;
            else
               dx6 = -dx;
            rstbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, dy3);
            rstbtt__csctx_rccurve_to(c, dx4, dy4, dx5, dy5, dx6, dy6);
            break;

         default:
            return rstbtt__CSERR("unimplemented");
         }
      } break;

      default:
         if (b0 != 255 && b0 != 28 && (b0 < 32 || b0 > 254))
            return rstbtt__CSERR("reserved operator");

         /* push immediate */
         if (b0 == 255) {
            f = (float)(i32)rstbtt__buf_get32(&b) / 0x10000;
         } else {
            rstbtt__buf_skip(&b, -1);
            f = (float)(i16)rstbtt__cff_int(&b);
         }
         if (sp >= 48) return rstbtt__CSERR("push stack overflow");
         s[sp++] = f;
         clear_stack = 0;
         break;
      }
      if (clear_stack) sp = 0;
   }
   return rstbtt__CSERR("no endchar");

#undef rstbtt__CSERR
}

RFONT_API int rstbtt__GetGlyphShapeT2(const rstbtt_fontinfo *info, int glyph_index, rstbtt_vertex **pvertices)
{
   /* runs the charstring twice, once to count and once to output (to avoid realloc) */
   rstbtt__csctx count_ctx = rstbtt__CSCTX_INIT(1);
   rstbtt__csctx output_ctx = rstbtt__CSCTX_INIT(0);
   if (rstbtt__run_charstring(info, glyph_index, &count_ctx)) {
      *pvertices = (rstbtt_vertex*)RFONT_MALLOC((size_t)count_ctx.num_vertices * sizeof(rstbtt_vertex));
      output_ctx.pvertices = *pvertices;
      if (rstbtt__run_charstring(info, glyph_index, &output_ctx)) {
         assert(output_ctx.num_vertices == count_ctx.num_vertices);
         return output_ctx.num_vertices;
      }
   }
   *pvertices = NULL;
   return 0;
}

RFONT_API int rstbtt_GetGlyphShape(const rstbtt_fontinfo *info, int glyph_index, rstbtt_vertex **pvertices)
{
   if (!info->cff.size)
      return rstbtt__GetGlyphShapeTT(info, glyph_index, pvertices);
   else
      return rstbtt__GetGlyphShapeT2(info, glyph_index, pvertices);
}

RFONT_API int  rstbtt__GetGlyphKernInfoAdvance(const rstbtt_fontinfo *info, int glyph1, int glyph2)
{
   u8 *data = info->data + info->kern;
   u32 needle, straw;
   int l, r, m;

   /* we only look at the first table. it must be 'horizontal' and format 0. */
   if (!info->kern)
      return 0;
   if (RFONT_USHORT(data, 2) < 1) /* number of tables, need at least 1 */
      return 0;
   if (RFONT_USHORT(data, 8) != 1) /* horizontal flag must be set in format */
      return 0;

   l = 0;
   r = RFONT_USHORT(data, 10) - 1;
   needle = (u32)(glyph1 << 16 | glyph2);
   while (l <= r) {
      m = (l + r) >> 1;
      straw = RFONT_ULONG(data, 18+(m*6)); /* note: unaligned read */
      if (needle < straw)
         r = m - 1;
      else if (needle > straw)
         l = m + 1;
      else
         return RFONT_SHORT(data, 22+(m*6));
   }
   return 0;
}

RFONT_API i32  rstbtt__GetCoverageIndex(u8 *coverageTable, int glyph)
{
    u16 coverageFormat = RFONT_USHORT(coverageTable, 0);
    switch(coverageFormat) {
        case 1: {
            u16 glyphCount = RFONT_USHORT(coverageTable, 2);

            /* Binary search. */
            i32 l=0, r=glyphCount-1, m;
            int straw, needle=glyph;
            while (l <= r) {
                u8 *glyphArray = coverageTable + 4;
                u16 glyphID;
                m = (l + r) >> 1;
                glyphID = RFONT_USHORT(glyphArray, 2 * m);
                straw = glyphID;
                if (needle < straw)
                    r = m - 1;
                else if (needle > straw)
                    l = m + 1;
                else {
                     return m;
                }
            }
        } break;

        case 2: {
            u16 rangeCount = RFONT_USHORT(coverageTable, 2);
            u8 *rangeArray = coverageTable + 4;

            /* Binary search. */
            i32 l=0, r=rangeCount-1, m;
            int strawStart, strawEnd, needle=glyph;
            while (l <= r) {
                u8 *rangeRecord;
                m = (l + r) >> 1;
                rangeRecord = rangeArray + 6 * m;
                strawStart = RFONT_USHORT(rangeRecord, 0);
                strawEnd = RFONT_USHORT(rangeRecord, 2);
                if (needle < strawStart)
                    r = m - 1;
                else if (needle > strawEnd)
                    l = m + 1;
                else {
                    u16 startCoverageIndex = RFONT_USHORT(rangeRecord, 4);
                    return startCoverageIndex + glyph - strawStart;
                }
            }
        } break;

        default: {
            /* There are no other cases. */
            assert(0);
        } break;
    }

    return -1;
}

RFONT_API i32  rstbtt__GetGlyphClass(u8 *classDefTable, int glyph)
{
    u16 classDefFormat = RFONT_USHORT(classDefTable, 0);
    switch(classDefFormat)
    {
        case 1: {
            u16 startGlyphID = RFONT_USHORT(classDefTable, 2);
            u16 glyphCount = RFONT_USHORT(classDefTable, 4);
            u8 *classDef1ValueArray = classDefTable + 6;

            if (glyph >= startGlyphID && glyph < startGlyphID + glyphCount)
                return (i32)RFONT_USHORT(classDef1ValueArray, 2 * (glyph - startGlyphID));

            classDefTable = classDef1ValueArray + 2 * glyphCount;
        } break;

        case 2: {
            u16 classRangeCount = RFONT_USHORT(classDefTable, 2);
            u8 *classRangeRecords = classDefTable + 4;

            /* Binary search. */
            i32 l=0, r=classRangeCount-1, m;
            int strawStart, strawEnd, needle=glyph;
            while (l <= r) {
                u8 *classRangeRecord;
                m = (l + r) >> 1;
                classRangeRecord = classRangeRecords + 6 * m;
                strawStart = RFONT_USHORT(classRangeRecord, 0);
                strawEnd = RFONT_USHORT(classRangeRecord, 2);
                if (needle < strawStart)
                    r = m - 1;
                else if (needle > strawEnd)
                    l = m + 1;
                else
                    return (i32)RFONT_USHORT(classRangeRecord, 4);
            }

            classDefTable = classRangeRecords + 6 * classRangeCount;
        } break;

        default: {
            /* There are no other cases. */
            assert(0);
        } break;
    }

    return -1;
}

/* Define to assert(x) if you want to break on unimplemented formats. */
#define rstbtt_GPOS_TODO_assert(x) assert(x)

RFONT_API i32  rstbtt__GetGlyphGPOSInfoAdvance(const rstbtt_fontinfo *info, int glyph1, int glyph2)
{
    u16 lookupListOffset;
    u8 *lookupList;
    u16 lookupCount;
    u8 *data;
    i32 i;

    if (!info->gpos) return 0;

    data = info->data + info->gpos;

    if (RFONT_USHORT(data, 0) != 1) return 0; /* Major version 1 */
    if (RFONT_USHORT(data, 2) != 0) return 0; /* Minor version 0 */

    lookupListOffset = RFONT_USHORT(data, 8);
    lookupList = data + lookupListOffset;
    lookupCount = RFONT_USHORT(lookupList, 0);

    for (i=0; i<lookupCount; ++i) {
        u16 lookupOffset = RFONT_USHORT(lookupList, 2 + 2 * i);
        u8 *lookupTable = lookupList + lookupOffset;

        u16 lookupType = RFONT_USHORT(lookupTable, 0);
        u16 subTableCount = RFONT_USHORT(lookupTable, 4);
        u8 *subTableOffsets = lookupTable + 6;
        switch(lookupType) {
            case 2: { /* Pair Adjustment Positioning Subtable */
                i32 sti;
                for (sti=0; sti<subTableCount; sti++) {
                    u16 subtableOffset = RFONT_USHORT(subTableOffsets, 2 * sti);
                    u8 *table = lookupTable + subtableOffset;
                    u16 posFormat = RFONT_USHORT(table, 0);
                    u16 coverageOffset = RFONT_USHORT(table, 2);
                    i32 coverageIndex = rstbtt__GetCoverageIndex(table + coverageOffset, glyph1);
                    if (coverageIndex == -1) continue;

                    switch (posFormat) {
                        case 1: {
                            i32 l, r, m;
                            int straw, needle;
                            u16 valueFormat1 = RFONT_USHORT(table, 4);
                            u16 valueFormat2 = RFONT_USHORT(table, 6);
                            i32 valueRecordPairSizeInBytes = 2;
                            u16 pairSetCount = RFONT_USHORT(table, 8);
                            u16 pairPosOffset = RFONT_USHORT(table, 10 + 2 * coverageIndex);
                            u8 *pairValueTable = table + pairPosOffset;
                            u16 pairValueCount = RFONT_USHORT(pairValueTable, 0);
                            u8 *pairValueArray = pairValueTable + 2;
                            /* TODO: Support more formats. */
                            rstbtt_GPOS_TODO_assert(valueFormat1 == 4);
                            if (valueFormat1 != 4) return 0;
                            rstbtt_GPOS_TODO_assert(valueFormat2 == 0);
                            if (valueFormat2 != 0) return 0;

                            assert(coverageIndex < pairSetCount);
                            rstbtt__NOTUSED(pairSetCount);

                            needle=glyph2;
                            r=pairValueCount-1;
                            l=0;

                            /* Binary search. */
                            while (l <= r) {
                                u16 secondGlyph;
                                u8 *pairValue;
                                m = (l + r) >> 1;
                                pairValue = pairValueArray + (2 + valueRecordPairSizeInBytes) * m;
                                secondGlyph = RFONT_USHORT(pairValue, 0);
                                straw = secondGlyph;
                                if (needle < straw)
                                    r = m - 1;
                                else if (needle > straw)
                                    l = m + 1;
                                else {
                                    i16 xAdvance = RFONT_SHORT(pairValue, 2);
                                    return xAdvance;
                                }
                            }
                        } break;

                        case 2: {
                            u16 valueFormat1 = RFONT_USHORT(table, 4);
                            u16 valueFormat2 = RFONT_USHORT(table, 6);

                            u16 classDef1Offset = RFONT_USHORT(table, 8);
                            u16 classDef2Offset = RFONT_USHORT(table, 10);
                            int glyph1class = rstbtt__GetGlyphClass(table + classDef1Offset, glyph1);
                            int glyph2class = rstbtt__GetGlyphClass(table + classDef2Offset, glyph2);

                            u16 class1Count = RFONT_USHORT(table, 12);
                            u16 class2Count = RFONT_USHORT(table, 14);
                            assert(glyph1class < class1Count);
                            assert(glyph2class < class2Count);

                            /* TODO: Support more formats. */
                            rstbtt_GPOS_TODO_assert(valueFormat1 == 4);
                            if (valueFormat1 != 4) return 0;
                            rstbtt_GPOS_TODO_assert(valueFormat2 == 0);
                            if (valueFormat2 != 0) return 0;

                            if (glyph1class >= 0 && glyph1class < class1Count && glyph2class >= 0 && glyph2class < class2Count) {
                                u8 *class1Records = table + 16;
                                u8 *class2Records = class1Records + 2 * (glyph1class * class2Count);
                                i16 xAdvance = RFONT_SHORT(class2Records, 2 * glyph2class);
                                return xAdvance;
                            }
                        } break;

                        default: {
                            /* There are no other cases. */
                            assert(0);
                            break;
                        };
                    }
                }
                break;
            };

            default:
                /* TODO: Implement other stuff. */
                break;
        }
    }

    return 0;
}

RFONT_API int  rstbtt_GetGlyphKernAdvance(const rstbtt_fontinfo *info, int g1, int g2)
{
   int xAdvance = 0;

   if (info->gpos)
      xAdvance += rstbtt__GetGlyphGPOSInfoAdvance(info, g1, g2);
   else if (info->kern)
      xAdvance += rstbtt__GetGlyphKernInfoAdvance(info, g1, g2);

   return xAdvance;
}

typedef struct rstbtt__hheap_chunk
{
   struct rstbtt__hheap_chunk *next;
} rstbtt__hheap_chunk;

typedef struct rstbtt__hheap
{
   struct rstbtt__hheap_chunk *head;
   void   *first_free;
   int    num_remaining_in_head_chunk;
} rstbtt__hheap;

RFONT_API void *rstbtt__hheap_alloc(rstbtt__hheap *hh, size_t size)
{
   if (hh->first_free) {
      void *p = hh->first_free;
      hh->first_free = * (void **) p;
      return p;
   } else {
      if (hh->num_remaining_in_head_chunk == 0) {
         int count = (size < 32 ? 2000 : size < 128 ? 800 : 100);
         rstbtt__hheap_chunk *c = (rstbtt__hheap_chunk *) RFONT_MALLOC(sizeof(rstbtt__hheap_chunk) + size * (size_t)count);
         if (c == NULL)
            return NULL;
         c->next = hh->head;
         hh->head = c;
         hh->num_remaining_in_head_chunk = count;
      }
      --hh->num_remaining_in_head_chunk;
      return (char *) (hh->head) + sizeof(rstbtt__hheap_chunk) + size * (size_t)hh->num_remaining_in_head_chunk;
   }
}

RFONT_API void rstbtt__hheap_free(rstbtt__hheap *hh, void *p)
{
   *(void **) p = hh->first_free;
   hh->first_free = p;
}

RFONT_API void rstbtt__hheap_cleanup(rstbtt__hheap *hh)
{
   rstbtt__hheap_chunk *c = hh->head;
   while (c) {
      rstbtt__hheap_chunk *n = c->next;
      RFONT_FREE(c);
      c = n;
   }
}

typedef struct rstbtt__edge {
   float x0,y0, x1,y1;
   int invert;
} rstbtt__edge;


typedef struct rstbtt__active_edge
{
   struct rstbtt__active_edge *next;
   float fx,fdx,fdy;
   float direction;
   float sy;
   float ey;
} rstbtt__active_edge;


RFONT_API rstbtt__active_edge *rstbtt__new_active(rstbtt__hheap *hh, rstbtt__edge *e, int off_x, float start_point) {
   rstbtt__active_edge *z = (rstbtt__active_edge *) rstbtt__hheap_alloc(hh, sizeof(*z));
   float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
   assert(z != NULL);
   /* assert(e->y0 <= start_point); */
   if (!z) return z;
   z->fdx = dxdy;
   z->fdy = dxdy != 0.0f ? (1.0f/dxdy) : 0.0f;
   z->fx = e->x0 + dxdy * (start_point - e->y0);
   z->fx -= (float)off_x;
   z->direction = e->invert ? 1.0f : -1.0f;
   z->sy = e->y0;
   z->ey = e->y1;
   z->next = 0;
   return z;
}

RFONT_API void rstbtt__handle_clipped_edge(float *scanline, int x, rstbtt__active_edge *e, float x0, float y0, float x1, float y1) {
   if (y0 == y1) return;
   assert(y0 < y1);
   assert(e->sy <= e->ey);
   if (y0 > e->ey) return;
   if (y1 < e->sy) return;
   if (y0 < e->sy) {
      x0 += (x1-x0) * (e->sy - y0) / (y1-y0);
      y0 = e->sy;
   }
   if (y1 > e->ey) {
      x1 += (x1-x0) * (e->ey - y1) / (y1-y0);
      y1 = e->ey;
   }

   if (x0 == x)
      assert(x1 <= x+1);
   else if (x0 == x+1)
      assert(x1 >= x);
   else if (x0 <= x)
      assert(x1 <= x);
   else if (x0 >= x+1)
      assert(x1 >= x+1);
   else
      assert(x1 >= x && x1 <= x+1);

   if (x0 <= x && x1 <= x)
      scanline[x] += e->direction * (y1-y0);
   else if (x0 >= x+1 && x1 >= x+1)
      ;
   else {
      assert(x0 >= x && x0 <= x+1 && x1 >= x && x1 <= x+1);
      scanline[x] += e->direction * (y1-y0) * (1-((x0 - (float)x)+(x1 - (float)x))/2.0f); /* coverage = 1 - average x position */
   }
}

RFONT_API void rstbtt__fill_active_edges_new(float *scanline, float *scanline_fill, int len, rstbtt__active_edge *e, float y_top)
{
   float y_bottom = y_top+1;

   while (e) {
      /* brute force every pixel
       compute intersection points with top & bottom */
      assert(e->ey >= y_top);

      if (e->fdx == 0) {
         float x0 = e->fx;
         if (x0 < len) {
            if (x0 >= 0) {
               rstbtt__handle_clipped_edge(scanline,(int) x0,e, x0,y_top, x0,y_bottom);
               rstbtt__handle_clipped_edge(scanline_fill-1,(int) x0+1,e, x0,y_top, x0,y_bottom);
            } else {
               rstbtt__handle_clipped_edge(scanline_fill-1,0,e, x0,y_top, x0,y_bottom);
            }
         }
      } else {
         float x0 = e->fx;
         float dx = e->fdx;
         float xb = x0 + dx;
         float x_top, x_bottom;
         float sy0,sy1;
         float dy = e->fdy;
         assert(e->sy <= y_bottom && e->ey >= y_top);

         if (e->sy > y_top) {
            x_top = x0 + dx * (e->sy - y_top);
            sy0 = e->sy;
         } else {
            x_top = x0;
            sy0 = y_top;
         }
         if (e->ey < y_bottom) {
            x_bottom = x0 + dx * (e->ey - y_top);
            sy1 = e->ey;
         } else {
            x_bottom = xb;
            sy1 = y_bottom;
         }

         if (x_top >= 0 && x_bottom >= 0 && x_top < len && x_bottom < len) {
            /* from here on, we don't have to range check x values */

            if ((int) x_top == (int) x_bottom) {
               float height;
               /* simple case, only spans one pixel */
               int x = (int) x_top;
               height = sy1 - sy0;
               assert(x >= 0 && x < len);
               scanline[x] += e->direction * (float)((1-((x_top - (float)x) + (x_bottom-(float)x))/2)) * height;
               scanline_fill[x] += e->direction * height; /* everything right of this pixel is filled */
            } else {
               int x,x1,x2;
               float y_crossing, step, sign, area;
               /* covers 2+ pixels */
               if (x_top > x_bottom) {
                  /* flip scanline vertically; signed area is the same */
                  float t;
                  sy0 = y_bottom - (sy0 - y_top);
                  sy1 = y_bottom - (sy1 - y_top);
                  t = sy0, sy0 = sy1, sy1 = t;
                  t = x_bottom, x_bottom = x_top, x_top = t;
                  dx = -dx;
                  dy = -dy;
                  t = x0, x0 = xb, xb = t;
               }

               x1 = (int) x_top;
               x2 = (int) x_bottom;
               /* compute intersection with y axis at x1+1 */
               y_crossing = ((float)x1 + 1.0f - (float)x0) * dy + y_top;

               sign = e->direction;
               /* area of the rectangle covered from y0..y_crossing */
               area = sign * (y_crossing-sy0);
               /* area of the triangle (x_top,y0), (x+1,y0), (x+1,y_crossing) */
               scanline[x1] += area * (1-(((float)x_top - (float)x1)+ ((float)(x1+1-x1)) / 2.0f));

               step = sign * dy;
               for (x = x1+1; x < x2; ++x) {
                  scanline[x] += area + step/2;
                  area += step;
               }
               y_crossing += dy * ((float)x2 - (float)(x1+1));

               assert(fabs(area) <= 1.01f);

               scanline[x2] += area + sign * (1.0f-((float)((float)x2 - (float)x2) + ((float)x_bottom - (float)x2)) / 2.0f) * (float)(sy1-y_crossing);

               scanline_fill[x2] += sign * (sy1-sy0);
            }
         } else {
            int x;
            for (x=0; x < len; ++x) {

               float y0 = y_top;
               float x1 = (float) (x);
               float x2 = (float) (x+1);
               float x3 = xb;
               float y3 = y_bottom;

               float y1 = ((float)x - x0) / dx + y_top;
               float y2 = ((float)x + 1 - x0) / dx + y_top;

               if (x0 < x1 && x3 > x2) {         /*three segments descending down-right */
                  rstbtt__handle_clipped_edge(scanline,x,e, x0,y0, x1,y1);
                  rstbtt__handle_clipped_edge(scanline,x,e, x1,y1, x2,y2);
                  rstbtt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
               } else if (x3 < x1 && x0 > x2) {  /* three segments descending down-left */
                  rstbtt__handle_clipped_edge(scanline,x,e, x0,y0, x2,y2);
                  rstbtt__handle_clipped_edge(scanline,x,e, x2,y2, x1,y1);
                  rstbtt__handle_clipped_edge(scanline,x,e, x1,y1, x3,y3);
               } else if (x0 < x1 && x3 > x1) {  /* two segments across x, down-right */
                  rstbtt__handle_clipped_edge(scanline,x,e, x0,y0, x1,y1);
                  rstbtt__handle_clipped_edge(scanline,x,e, x1,y1, x3,y3);
               } else if (x3 < x1 && x0 > x1) {  /* two segments across x, down-left */
                  rstbtt__handle_clipped_edge(scanline,x,e, x0,y0, x1,y1);
                  rstbtt__handle_clipped_edge(scanline,x,e, x1,y1, x3,y3);
               } else if (x0 < x2 && x3 > x2) {  /* two segments across x+1, down-right */
                  rstbtt__handle_clipped_edge(scanline,x,e, x0,y0, x2,y2);
                  rstbtt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
               } else if (x3 < x2 && x0 > x2) {  /* two segments across x+1, down-left */
                  rstbtt__handle_clipped_edge(scanline,x,e, x0,y0, x2,y2);
                  rstbtt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
               } else {  /* one segment */
                  rstbtt__handle_clipped_edge(scanline,x,e, x0,y0, x3,y3);
               }
            }
         }
      }
      e = e->next;
   }
}

/* directly AA rasterize edges w/o supersampling */
RFONT_API void rstbtt__rasterize_sorted_edges(rstbtt__bitmap *result, rstbtt__edge *e, int n, int vsubsample, int off_x, int off_y)
{
   rstbtt__hheap hh = { 0, 0, 0 };
   rstbtt__active_edge *active = NULL;
   int y,j=0, i;
   float scanline_data[129], *scanline, *scanline2;

   rstbtt__NOTUSED(vsubsample);

   if (result->w > 64)
      scanline = (float *) RFONT_MALLOC((size_t)(result->w*2+1) * sizeof(float));
   else
      scanline = scanline_data;

   scanline2 = scanline + result->w;

   y = off_y;
   e[n].y0 = (float) (off_y + result->h) + 1;

   while (j < result->h) {
      /* find center of pixel for this scanline */
      float scan_y_top    = (float)y + 0.0f;
      float scan_y_bottom = (float)y + 1.0f;
      rstbtt__active_edge **step = &active;

      RFONT_MEMSET(scanline , 0, (size_t)result->w * sizeof(scanline[0]));
      RFONT_MEMSET(scanline2, 0, (size_t)(result->w + 1) * sizeof(scanline[0]));

      /* update all active edges;
        remove all active edges that terminate before the top of this scanline */
      while (*step) {
         rstbtt__active_edge * z = *step;
         if (z->ey <= scan_y_top) {
            *step = z->next; /* delete from list (/) */
            assert(z->direction);
            z->direction = 0;
            rstbtt__hheap_free(&hh, z);
         } else {
            step = &((*step)->next); /* advance through list */
         }
      }

      /* insert all edges that start before the bottom of this scanline */
      while (e->y0 <= scan_y_bottom) {
         if (e->y0 != e->y1) {
            rstbtt__active_edge *z = rstbtt__new_active(&hh, e, off_x, scan_y_top);
            if (z != NULL) {
               if (j == 0 && off_y != 0) {
                  if (z->ey < scan_y_top) {
                     /* this can happen due to subpixel positioning and some kind of fp rounding error i think */
                     z->ey = scan_y_top;
                  }
               }
               assert(z->ey >= scan_y_top); /* if we get really unlucky a tiny bit of an edge can be out of bounds */
               /* insert at front */
               z->next = active;
               active = z;
            }
         }
         ++e;
      }

      /* now process all active edges */
      if (active)
         rstbtt__fill_active_edges_new(scanline, scanline2+1, result->w, active, scan_y_top);

      {
         float sum = 0;
         for (i=0; i < result->w; ++i) {
            float k;
            int m;
            sum += scanline2[i];
            k = scanline[i] + sum;
            k = (float) fabs(k)*255 + 0.5f;
            m = (int) k;
            if (m > 255) m = 255;
            result->pixels[j*result->stride + i] = (unsigned char) m;
         }
      }
      /* advance all the edges */
      step = &active;
      while (*step) {
         rstbtt__active_edge *z = *step;
         z->fx += z->fdx; /* advance to position for current scanline */
         step = &((*step)->next); /* advance through list */
      }

      ++y;
      ++j;
   }

   rstbtt__hheap_cleanup(&hh);

   if (scanline != scanline_data)
      RFONT_FREE(scanline);
}

#define rstbtt__COMPARE(a,b)  ((a)->y0 < (b)->y0)

RFONT_API void rstbtt__sort_edges_ins_sort(rstbtt__edge *p, int n)
{
   int i,j;
   for (i=1; i < n; ++i) {
      rstbtt__edge t = p[i], *a = &t;
      j = i;
      while (j > 0) {
         rstbtt__edge *b = &p[j-1];
         int c = rstbtt__COMPARE(a,b);
         if (!c) break;
         p[j] = p[j-1];
         --j;
      }
      if (i != j)
         p[j] = t;
   }
}

RFONT_API void rstbtt__sort_edges_quicksort(rstbtt__edge *p, int n)
{
   /* threshold for transitioning to insertion sort */
   while (n > 12) {
      rstbtt__edge t;
      int c01,c12,c,m,i,j;

      /* compute median of three */
      m = n >> 1;
      c01 = rstbtt__COMPARE(&p[0],&p[m]);
      c12 = rstbtt__COMPARE(&p[m],&p[n-1]);
      /* if 0 >= mid >= end, or 0 < mid < end, then use mid */
      if (c01 != c12) {
         /* otherwise, we'll need to swap something else to middle */
         int z;
         c = rstbtt__COMPARE(&p[0],&p[n-1]);
         /* 0>mid && mid<n:  0>n => n; 0<n => 0 */
         /* 0<mid && mid>n:  0>n => 0; 0<n => n */
         z = (c == c12) ? 0 : n-1;
         t = p[z];
         p[z] = p[m];
         p[m] = t;
      }
      /* now p[m] is the median-of-three */
      /* swap it to the beginning so it won't move around */
      t = p[0];
      p[0] = p[m];
      p[m] = t;

      /* partition loop */
      i=1;
      j=n-1;
      for(;;) {
         /* handling of equality is crucial here */
         /* for sentinels & efficiency with duplicates */
         for (;;++i) {
            if (!rstbtt__COMPARE(&p[i], &p[0])) break;
         }
         for (;;--j) {
            if (!rstbtt__COMPARE(&p[0], &p[j])) break;
         }
         /* make sure we haven't crossed */
         if (i >= j) break;
         t = p[i];
         p[i] = p[j];
         p[j] = t;

         ++i;
         --j;
      }
      /* recurse on smaller side, iterate on larger */
      if (j < (n-i)) {
         rstbtt__sort_edges_quicksort(p,j);
         p = p+i;
         n = n-i;
      } else {
         rstbtt__sort_edges_quicksort(p+i, n-i);
         n = j;
      }
   }
}

RFONT_API void rstbtt__sort_edges(rstbtt__edge *p, int n)
{
   rstbtt__sort_edges_quicksort(p, n);
   rstbtt__sort_edges_ins_sort(p, n);
}

typedef struct
{
   float x,y;
} rstbtt__point;

RFONT_API void rstbtt__rasterize(rstbtt__bitmap *result, rstbtt__point *pts, int *wcount, int windings, float scale_x, float scale_y, float shift_x, float shift_y, int off_x, int off_y, int invert)
{
   float y_scale_inv = invert ? -scale_y : scale_y;
   rstbtt__edge *e;
   int n,i,j,k,m;
#if rstbtt_RASTERIZER_VERSION == 1
   int vsubsample = result->h < 8 ? 15 : 5;
#elif rstbtt_RASTERIZER_VERSION == 2
   int vsubsample = 1;
#else
   #error "Unrecognized value of rstbtt_RASTERIZER_VERSION"
#endif
   /* vsubsample should divide 255 evenly; otherwise we won't reach full opacity

    now we have to blow out the windings into explicit edge lists */
   n = 0;
   for (i=0; i < windings; ++i)
      n += wcount[i];

   e = (rstbtt__edge *) RFONT_MALLOC(sizeof(*e) * (size_t)(n+1)); /* add an extra one as a sentinel */
   if (e == 0) return;
   n = 0;

   m=0;
   for (i=0; i < windings; ++i) {
      rstbtt__point *p = pts + m;
      m += wcount[i];
      j = wcount[i]-1;
      for (k=0; k < wcount[i]; j=k++) {
         int a=k,b=j;
         /* skip the edge if horizontal */
         if (p[j].y == p[k].y)
            continue;
         /* add edge from j to k to the list */
         e[n].invert = 0;
         if (invert ? p[j].y > p[k].y : p[j].y < p[k].y) {
            e[n].invert = 1;
            a=j,b=k;
         }
         e[n].x0 = p[a].x * scale_x + shift_x;
         e[n].y0 = (p[a].y * y_scale_inv + shift_y) * (float)vsubsample;
         e[n].x1 = p[b].x * scale_x + shift_x;
         e[n].y1 = (p[b].y * y_scale_inv + shift_y) * (float)vsubsample;
         ++n;
      }
   }

   /* now sort the edges by their highest point (should snap to integer, and then by x)
    rstbtt_sort(e, n, sizeof(e[0]), rstbtt__edge_compare); */
   rstbtt__sort_edges(e, n);

   /* now, traverse the scanlines and find the intersections on each scanline, use xor winding rule */
   rstbtt__rasterize_sorted_edges(result, e, n, vsubsample, off_x, off_y);

   RFONT_FREE(e);
}

RFONT_API void rstbtt__add_point(rstbtt__point *points, int n, float x, float y)
{
   if (!points) return; /* during first pass, it's unallocated */
   points[n].x = x;
   points[n].y = y;
}

/* tessellate until threshold p is happy... @TODO warped to compensate for non-linear stretching */
RFONT_API int rstbtt__tesselate_curve(rstbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float objspace_flatness_squared, int n)
{
   /* midpoint */
   float mx = (x0 + 2*x1 + x2)/4;
   float my = (y0 + 2*y1 + y2)/4;
   /* versus directly drawn line */
   float dx = (x0+x2)/2 - mx;
   float dy = (y0+y2)/2 - my;
   if (n > 16) /* 65536 segments on one curve better be enough! */
      return 1;
   if (dx*dx+dy*dy > objspace_flatness_squared) { /* half-pixel error allowed... need to be smaller if AA */
      rstbtt__tesselate_curve(points, num_points, x0,y0, (x0+x1)/2.0f,(y0+y1)/2.0f, mx,my, objspace_flatness_squared,n+1);
      rstbtt__tesselate_curve(points, num_points, mx,my, (x1+x2)/2.0f,(y1+y2)/2.0f, x2,y2, objspace_flatness_squared,n+1);
   } else {
      rstbtt__add_point(points, *num_points,x2,y2);
      *num_points = *num_points+1;
   }
   return 1;
}

RFONT_API void rstbtt__tesselate_cubic(rstbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float objspace_flatness_squared, int n)
{
   /* @TODO this "flatness" calculation is just made-up nonsense that seems to work well enough */
   float dx0 = x1-x0;
   float dy0 = y1-y0;
   float dx1 = x2-x1;
   float dy1 = y2-y1;
   float dx2 = x3-x2;
   float dy2 = y3-y2;
   float dx = x3-x0;
   float dy = y3-y0;
   float longlen = (float) (sqrt(dx0*dx0+dy0*dy0)+sqrt(dx1*dx1+dy1*dy1)+sqrt(dx2*dx2+dy2*dy2));
   float shortlen = (float) sqrt(dx*dx+dy*dy);
   float flatness_squared = longlen*longlen-shortlen*shortlen;

   if (n > 16) /* 65536 segments on one curve better be enough! */
      return;

   if (flatness_squared > objspace_flatness_squared) {
      float x01 = (x0+x1)/2;
      float y01 = (y0+y1)/2;
      float x12 = (x1+x2)/2;
      float y12 = (y1+y2)/2;
      float x23 = (x2+x3)/2;
      float y23 = (y2+y3)/2;

      float xa = (x01+x12)/2;
      float ya = (y01+y12)/2;
      float xb = (x12+x23)/2;
      float yb = (y12+y23)/2;

      float mx = (xa+xb)/2;
      float my = (ya+yb)/2;

      rstbtt__tesselate_cubic(points, num_points, x0,y0, x01,y01, xa,ya, mx,my, objspace_flatness_squared,n+1);
      rstbtt__tesselate_cubic(points, num_points, mx,my, xb,yb, x23,y23, x3,y3, objspace_flatness_squared,n+1);
   } else {
      rstbtt__add_point(points, *num_points,x3,y3);
      *num_points = *num_points+1;
   }
}

/* returns number of contours */
RFONT_API rstbtt__point *rstbtt_FlattenCurves(rstbtt_vertex *vertices, int num_verts, float objspace_flatness, int **contour_lengths, int *num_contours)
{
   rstbtt__point *points=0;
   int num_points=0;

   float objspace_flatness_squared = objspace_flatness * objspace_flatness;
   int i,n=0,start=0, pass;

   /* count how many "moves" there are to get the contour count */
   for (i=0; i < num_verts; ++i)
      if (vertices[i].type == rstbtt_vmove)
         ++n;

   *num_contours = n;
   if (n == 0) return 0;

   *contour_lengths = (int *) RFONT_MALLOC((size_t)(sizeof(**contour_lengths) * (size_t)n));

   if (*contour_lengths == 0) {
      *num_contours = 0;
      return 0;
   }

   /* make two passes through the points so we don't need to realloc */
   for (pass=0; pass < 2; ++pass) {
      float x=0,y=0;
      if (pass == 1) {
         points = (rstbtt__point *) RFONT_MALLOC((size_t)num_points * sizeof(points[0]));
         if (points == NULL) goto error;
      }
      num_points = 0;
      n= -1;
      for (i=0; i < num_verts; ++i) {
         switch (vertices[i].type) {
            case rstbtt_vmove:
               /* start the next contour */
               if (n >= 0)
                  (*contour_lengths)[n] = num_points - start;
               ++n;
               start = num_points;

               x = vertices[i].x, y = vertices[i].y;
               rstbtt__add_point(points, num_points++, x,y);
               break;
            case rstbtt_vline:
               x = vertices[i].x, y = vertices[i].y;
               rstbtt__add_point(points, num_points++, x, y);
               break;
            case rstbtt_vcurve:
               rstbtt__tesselate_curve(points, &num_points, x,y,
                                        vertices[i].cx, vertices[i].cy,
                                        vertices[i].x,  vertices[i].y,
                                        objspace_flatness_squared, 0);
               x = vertices[i].x, y = vertices[i].y;
               break;
            case rstbtt_vcubic:
               rstbtt__tesselate_cubic(points, &num_points, x,y,
                                        vertices[i].cx, vertices[i].cy,
                                        vertices[i].cx1, vertices[i].cy1,
                                        vertices[i].x,  vertices[i].y,
                                        objspace_flatness_squared, 0);
               x = vertices[i].x;
               y = vertices[i].y;
               break;
         }
      }
      (*contour_lengths)[n] = num_points - start;
   }

   return points;
error:
   RFONT_FREE(points);
   RFONT_FREE(*contour_lengths);
   *contour_lengths = 0;
   *num_contours = 0;
   return NULL;
}

RFONT_API void rstbtt_Rasterize(rstbtt__bitmap *result, float flatness_in_pixels, rstbtt_vertex *vertices, int num_verts, float scale_x, float scale_y, float shift_x, float shift_y, int x_off, int y_off, int invert)
{
   float scale            = scale_x > scale_y ? scale_y : scale_x;
   int winding_count      = 0;
   int *winding_lengths   = NULL;
   rstbtt__point *windings = rstbtt_FlattenCurves(vertices, num_verts, flatness_in_pixels / scale, &winding_lengths, &winding_count);
   if (windings) {
      rstbtt__rasterize(result, windings, winding_lengths, winding_count, scale_x, scale_y, shift_x, shift_y, x_off, y_off, invert);
      RFONT_FREE(winding_lengths);
      RFONT_FREE(windings);
   }
}

RFONT_API void rstbtt_GetGlyphBitmapBoxSubpixel(const rstbtt_fontinfo *font, int glyph, float scale_x, float scale_y,float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
   int x0=0,y0=0,x1,y1; /* =0 suppresses compiler warning */
   if (!rstbtt_GetGlyphBox(font, glyph, &x0,&y0,&x1,&y1)) {
      /* e.g. space character */
      if (ix0) *ix0 = 0;
      if (iy0) *iy0 = 0;
      if (ix1) *ix1 = 0;
      if (iy1) *iy1 = 0;
   } else {
      /* move to integral bboxes (treating pixels as little squares, what pixels get touched)? */
      if (ix0) *ix0 = (int)floor( (float)x0 * scale_x + shift_x);
      if (iy0) *iy0 = (int)floor( (float)-y1 * scale_y + shift_y);
      if (ix1) *ix1 = (int)ceil ( (float)x1 * scale_x + shift_x);
      if (iy1) *iy1 = (int)ceil ((float)-y0 * scale_y + shift_y);
   }
}

RFONT_API unsigned char *rstbtt_GetGlyphBitmapSubpixel(const rstbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int *width, int *height, int *xoff, int *yoff)
{
   int ix0,iy0,ix1,iy1;
   rstbtt__bitmap gbm;
   rstbtt_vertex *vertices;
   int num_verts = rstbtt_GetGlyphShape(info, glyph, &vertices);

   if (scale_x == 0) scale_x = scale_y;
   if (scale_y == 0) {
      if (scale_x == 0) {
         RFONT_FREE(vertices);
         return NULL;
      }
      scale_y = scale_x;
   }

   rstbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0,&iy0,&ix1,&iy1);

   /* now we get the size */
   gbm.w = (ix1 - ix0);
   gbm.h = (iy1 - iy0);
   gbm.pixels = NULL; /* in case we error */

   if (width ) *width  = gbm.w;
   if (height) *height = gbm.h;
   if (xoff  ) *xoff   = ix0;
   if (yoff  ) *yoff   = iy0;

   if (gbm.w && gbm.h) {
      gbm.pixels = (unsigned char *) RFONT_MALLOC((size_t)(gbm.w * gbm.h));
      if (gbm.pixels) {
         gbm.stride = gbm.w;

         rstbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1);
      }
   }
   RFONT_FREE(vertices);
   return gbm.pixels;
}


RFONT_API int rstbtt_InitFont(rstbtt_fontinfo *info, const unsigned char* const_data, int fontstart)
{
   unsigned char* data = (unsigned char*)const_data;

   u32 cmap, t;
   i32 i,numTables;

   info->data = (unsigned char*)data;
   info->fontstart = fontstart;
   info->cff = rstbtt__new_buf(NULL, 0);

   cmap = rstbtt__find_table(data, (u32)fontstart, "cmap");       /* required */
   info->loca = (int)rstbtt__find_table(data, (u32)fontstart, "loca"); /* required */
   info->head = (int)rstbtt__find_table(data, (u32)fontstart, "head"); /* required */
   info->glyf = (int)rstbtt__find_table(data, (u32)fontstart, "glyf"); /* required */
   info->hhea = (int)rstbtt__find_table(data, (u32)fontstart, "hhea"); /* required */
   info->hmtx = (int)rstbtt__find_table(data, (u32)fontstart, "hmtx"); /* required */
   info->kern = (int)rstbtt__find_table(data, (u32)fontstart, "kern"); /* not required */
   info->gpos = (int)rstbtt__find_table(data, (u32)fontstart, "GPOS"); /* not required */

   if (!cmap || !info->head || !info->hhea || !info->hmtx)
      return 0;
   if (info->glyf) {
      /* required for truetype */
      if (!info->loca) return 0;
   } else {
      /* initialization for CFF / Type2 fonts (OTF) */
      rstbtt__buf b, topdict, topdictidx;
      u32 cstype = 2, charstrings = 0, fdarrayoff = 0, fdselectoff = 0;
      u32 cff;

      cff = rstbtt__find_table(data, (u32)fontstart, "CFF ");
      if (!cff) return 0;

      info->fontdicts = rstbtt__new_buf(NULL, 0);
      info->fdselect = rstbtt__new_buf(NULL, 0);

      /* @TODO this should use size from table (not 512MB) */
      info->cff = rstbtt__new_buf(data+cff, 512*1024*1024);
      b = info->cff;

      /* read the header */
      rstbtt__buf_skip(&b, 2);
      rstbtt__buf_seek(&b, rstbtt__buf_get8(&b)); /* hdrsize */

      /* @TODO the name INDEX could list multiple fonts,
        but we just use the first one. */
      rstbtt__cff_get_index(&b);  /* name INDEX */
      topdictidx = rstbtt__cff_get_index(&b);
      topdict = rstbtt__cff_index_get(topdictidx, 0);
      rstbtt__cff_get_index(&b);  /* string INDEX */
      info->gsubrs = rstbtt__cff_get_index(&b);

      rstbtt__dict_get_ints(&topdict, 17, 1, &charstrings);
      rstbtt__dict_get_ints(&topdict, 0x100 | 6, 1, &cstype);
      rstbtt__dict_get_ints(&topdict, 0x100 | 36, 1, &fdarrayoff);
      rstbtt__dict_get_ints(&topdict, 0x100 | 37, 1, &fdselectoff);
      info->subrs = rstbtt__get_subrs(b, topdict);

      /* weonly support Type 2 charstrings */
      if (cstype != 2) return 0;
      if (charstrings == 0) return 0;

      if (fdarrayoff) {
         /* looks like a CID font */
         if (!fdselectoff) return 0;
         rstbtt__buf_seek(&b, (int)fdarrayoff);
         info->fontdicts = rstbtt__cff_get_index(&b);
         info->fdselect = rstbtt__buf_range(&b, (int)fdselectoff, (int)b.size - (int)fdselectoff);
      }

      rstbtt__buf_seek(&b, (int)charstrings);
      info->charstrings = rstbtt__cff_get_index(&b);
   }

   t = rstbtt__find_table(data, (u32)fontstart, "maxp");
   if (t)
      info->numGlyphs = RFONT_USHORT(data, t + 4);
   else
      info->numGlyphs = 0xffff;

   info->svg = -1;

   numTables = RFONT_USHORT(data, cmap + 2);
   info->index_map = 0;
   for (i=0; i < numTables; ++i) {
      u32 encoding_record = (u32)(cmap + 4 + 8 * (u32)i);
      /* find an encoding we understand: */
      switch(RFONT_USHORT(data, encoding_record)) {
         case rstbtt_PLATFORM_ID_MICROSOFT:
            switch (RFONT_USHORT(data, (int)encoding_record+2)) {
               case rstbtt_MS_EID_UNICODE_BMP:
               case rstbtt_MS_EID_UNICODE_FULL:
                  /* MS/Unicode */
                  info->index_map = (int)(cmap + RFONT_ULONG(data, encoding_record + 4));
                  break;
            }
            break;
        case rstbtt_PLATFORM_ID_UNICODE:
            /* Mac/iOS has these */
            /* all the encodingIDs are unicode, so we don't bother to check it */
            info->index_map = (int)(cmap + RFONT_ULONG(data, encoding_record+4));
            break;
      }
   }
   if (info->index_map == 0)
      return 0;

   info->indexToLocFormat = RFONT_USHORT(((u8*)data), info->head + 50);
   return 1;
}

RFONT_API int rstbtt_FindGlyphIndex(const rstbtt_fontinfo *info, int unicode_codepoint) {
   u8 *data = info->data;
   u32 index_map = (u32)info->index_map;

   u16 format = RFONT_USHORT(data, index_map);
   if (format == 0) { /* apple byte encoding */
      i32 bytes = RFONT_USHORT(data, index_map + 2);
      if (unicode_codepoint < bytes-6)
         return RFONT_BYTE(data, (int)index_map + 6 + unicode_codepoint);
      return 0;
   } else if (format == 6) {
      u32 first = RFONT_USHORT(data, index_map + 6);
      u32 count = RFONT_USHORT(data, index_map + 8);
      if ((u32) unicode_codepoint >= first && (u32) unicode_codepoint < (u32)(first + count))
         return RFONT_USHORT(data, (int)index_map + 10 + (unicode_codepoint - (int)first)*2);
      return 0;
   } else if (format == 2) {
      assert(0); /* @TODO: high-byte mapping for japanese/chinese/korean */
      return 0;
   } else if (format == 4) { /* standard mapping for windows fonts: binary search collection of ranges */
      u16 segcount = RFONT_USHORT(data, index_map+6) >> 1;
      u16 searchRange = RFONT_USHORT(data, index_map+8) >> 1;
      u16 entrySelector = RFONT_USHORT(data, index_map+10);
      u16 rangeShift = RFONT_USHORT(data, index_map+12) >> 1;

      /* do a binary search of the segments */
      u32 endCount = index_map + 14;
      u32 search = endCount;

      if (unicode_codepoint > 0xffff)
         return 0;

      /* they lie from endCount .. endCount + segCount
        but searchRange is the nearest power of two, so... */
      if (unicode_codepoint >= RFONT_USHORT(data, search + rangeShift*2))
         search += rangeShift*2;

      /* now decrement to bias correctly to find smallest */
      search -= 2;
      while (entrySelector) {
         u16 end;
         searchRange >>= 1;
         end = RFONT_USHORT(data, search + searchRange*2);
         if (unicode_codepoint > end)
            search += searchRange*2;
         --entrySelector;
      }
      search += 2;

      {
         u16 offset, start;
         u16 item = (u16) ((search - endCount) >> 1);

         assert(unicode_codepoint <= RFONT_USHORT(data, endCount + 2*item));
         start = RFONT_USHORT(data, index_map + 14 + segcount*2 + 2 + 2*item);
         if (unicode_codepoint < start)
            return 0;

         offset = RFONT_USHORT(data, index_map + 14 + segcount*6 + 2 + 2*item);
         if (offset == 0)
            return (u16) (unicode_codepoint + RFONT_SHORT(data, index_map + 14 + segcount*4 + 2 + 2*item));

         return RFONT_USHORT(data, offset + (unicode_codepoint-start)*2 + (int)index_map + 14 + segcount*6 + 2 + 2*item);
      }
   } else if (format == 12 || format == 13) {
      u32 ngroups = RFONT_ULONG(data, index_map+12);
      i32 low,high;
      low = 0; high = (i32)ngroups;
      /* Binary search the right group. */
      while (low < high) {
         i32 mid = low + ((high-low) >> 1); /* rounds down, so low <= mid < high */
         u32 start_char = RFONT_ULONG(data, (int)index_map + 16 + mid * 12);
         u32 end_char = RFONT_ULONG(data, (int)index_map + 16 + mid * 12 + 4);
         if ((u32) unicode_codepoint < start_char)
            high = mid;
         else if ((u32) unicode_codepoint > end_char)
            low = mid+1;
         else {
            u32 start_glyph = RFONT_ULONG(data, (int)index_map + 16 + mid * 12 + 8);
            if (format == 12)
               return (int)((int)start_glyph + unicode_codepoint - (int)start_char);
            else /* format == 13 */
               return (int)start_glyph;
         }
      }
      return 0; /* not found */
   }
   /* @TODO */
   assert(0);
   return 0;
}

RFONT_API int rstbtt__GetGlyfOffset(const rstbtt_fontinfo *info, int glyph_index)
{
   int g1,g2;

   assert(!info->cff.size);

   if (glyph_index >= info->numGlyphs) return -1; /* glyph index out of range */
   if (info->indexToLocFormat >= 2)    return -1; /* unknown index->glyph map format */

   if (info->indexToLocFormat == 0) {
      g1 = info->glyf + RFONT_USHORT(info->data, info->loca + glyph_index * 2) * 2;
      g2 = info->glyf + RFONT_USHORT(info->data, info->loca + glyph_index * 2 + 2) * 2;
   } else {
      g1 = info->glyf + (int)RFONT_ULONG (info->data, info->loca + glyph_index * 4);
      g2 = info->glyf + (int)RFONT_ULONG (info->data, info->loca + glyph_index * 4 + 4);
   }

   return g1==g2 ? -1 : g1; /* if length is 0, return -1 */
}

RFONT_API int rstbtt__GetGlyphInfoT2(const rstbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1)
{
   rstbtt__csctx c = rstbtt__CSCTX_INIT(1);
   int r = rstbtt__run_charstring(info, glyph_index, &c);
   if (x0)  *x0 = r ? c.min_x : 0;
   if (y0)  *y0 = r ? c.min_y : 0;
   if (x1)  *x1 = r ? c.max_x : 0;
   if (y1)  *y1 = r ? c.max_y : 0;
   return r ? c.num_vertices : 0;
}

RFONT_API int rstbtt_GetGlyphBox(const rstbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1)
{
   if (info->cff.size) {
      rstbtt__GetGlyphInfoT2(info, glyph_index, x0, y0, x1, y1);
   } else {
      int g = rstbtt__GetGlyfOffset(info, glyph_index);
      if (g < 0) return 0;

      if (x0) *x0 = RFONT_SHORT(info->data, g + 2);
      if (y0) *y0 = RFONT_SHORT(info->data, g + 4);
      if (x1) *x1 = RFONT_SHORT(info->data, g + 6);
      if (y1) *y1 = RFONT_SHORT(info->data, g + 8);
   }
   return 1;
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
#endif /* n RFONT_EXTERNAL_STB */

/*
END of stb_truetype defines and source code required by RFont
*/

#endif /* RFONT_IMPLEMENTATION */
