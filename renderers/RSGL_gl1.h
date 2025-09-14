#ifndef RSGL_H
#include "RSGL.h"
#endif

// WebGL doesn't support compute shaders iirc so yeah
#if defined(__EMSCRIPTEN__) && defined(RSGL_USE_COMPUTE)
#undef RSGL_USE_COMPUTE
#endif

#ifndef RSGL_GL1_H
#define RSGL_GL1_H

typedef struct RSGL_gl1Renderer { u32 vao; } RSGL_gl1Renderer;

RSGLDEF RSGL_rendererProc RSGL_GL1_rendererProc(void);
RSGLDEF size_t RSGL_GL1_size(void);

RSGLDEF RSGL_renderer* RSGL_GL1_renderer_init(RSGL_area r, void* loader);
RSGLDEF void RSGL_GL1_renderer_initPtr(RSGL_area r, void* loader, RSGL_gl1Renderer* ptr, RSGL_renderer* renderer);

RSGLDEF void RSGL_GL1_render(RSGL_gl1Renderer* ctx, const RSGL_programInfo* program, const float* matrix, const RSGL_renderBuffers* buffers);
RSGLDEF void RSGL_GL1_initPtr(RSGL_gl1Renderer* ctx, void* proc); /* init render backend */
RSGLDEF void RSGL_GL1_freePtr(RSGL_gl1Renderer* ctx); /* free render backend */
RSGLDEF void RSGL_GL1_clear(RSGL_gl1Renderer* ctx, float r, float g, float b, float a);
RSGLDEF void RSGL_GL1_viewport(RSGL_gl1Renderer* ctx, i32 x, i32 y, i32 w, i32 h);
RSGLDEF void RSGL_GL1_createBuffer(RSGL_gl1Renderer* ctx, size_t size, const void* data, size_t* buffer);
RSGLDEF void RSGL_GL1_updateBuffer(RSGL_gl1Renderer* ctx, size_t buffer, const void* data, size_t start, size_t end);
RSGLDEF void RSGL_GL1_deleteBuffer(RSGL_gl1Renderer* ctx, size_t buffer);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_GL1_createTexture(RSGL_gl1Renderer* ctx, const RSGL_textureBlob* blob);
RSGLDEF void RSGL_GL1_copyToTexture(RSGL_gl1Renderer* ctx, RSGL_texture texture, size_t x, size_t y, const RSGL_textureBlob* blob);
/* delete a texture */
RSGLDEF void RSGL_GL1_deleteTexture(RSGL_gl1Renderer* ctx, RSGL_texture tex);
/* starts scissoring */
RSGLDEF void RSGL_GL1_scissorStart(RSGL_gl1Renderer* ctx, RSGL_rectF scissor, i32 renderer_height);
/* stops scissoring */
RSGLDEF void RSGL_GL1_scissorEnd(RSGL_gl1Renderer* ctx);

RSGLDEF RSGL_texture RSGL_GL1_create_atlas(RSGL_gl1Renderer* ctx, u32 atlasWidth, u32 atlasHeight);
RSGLDEF void RSGL_GL1_bitmap_to_atlas(RSGL_gl1Renderer* ctx, RFont_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y);
#endif

#ifdef RSGL_IMPLEMENTATION

RSGL_renderer* RSGL_GL1_renderer_init(RSGL_area r, void* loader) { return RSGL_renderer_init(RSGL_GL1_rendererProc(), r, loader); }
void RSGL_GL1_renderer_initPtr(RSGL_area r, void* loader, RSGL_gl1Renderer* ptr, RSGL_renderer* renderer) { return RSGL_renderer_initPtr(RSGL_GL1_rendererProc(), r, loader, ptr, renderer); }


/* prevent winapi conflicts (opengl includes windows.h for some reason) */
#define OEMRESOURCE

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#if defined(_WIN32)
typedef char GLchar;
typedef int	 GLsizei;

#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_ARRAY_BUFFER         0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW  0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_TEXTURE0 0x84C0

#include <GL/glext.h>
#endif

size_t RSGL_GL1_size(void) {
	return sizeof(RSGL_gl1Renderer);
}

RSGL_rendererProc RSGL_GL1_rendererProc() {
	RSGL_rendererProc proc;
	RSGL_MEMSET(&proc, 0, sizeof(proc));

	proc.render = (void (*)(void*, const RSGL_programInfo*, const float*, const RSGL_renderBuffers*))RSGL_GL1_render;
    proc.size = (size_t (*)(void))RSGL_GL1_size;
    proc.initPtr = (void (*)(void*, void*))RSGL_GL1_initPtr;
    proc.freePtr = (void (*)(void*))RSGL_GL1_freePtr;
    proc.clear = (void (*)(void*, float, float, float, float))RSGL_GL1_clear;
    proc.viewport = (void (*)(void*, i32, i32, i32, i32))RSGL_GL1_viewport;
    proc.createTexture = (RSGL_texture (*)(void*, const RSGL_textureBlob*))RSGL_GL1_createTexture;
    proc.copyToTexture = (void (*)(void*, RSGL_texture, size_t, size_t, const RSGL_textureBlob* blob))RSGL_GL1_copyToTexture;
    proc.deleteTexture = (void (*)(void*, RSGL_texture))RSGL_GL1_deleteTexture;
    proc.scissorStart = (void (*)(void*, RSGL_rectF, i32))RSGL_GL1_scissorStart;
    proc.scissorEnd =  (void (*)(void*))RSGL_GL1_scissorEnd;
    proc.createAtlas = (RSGL_texture (*)(void*, u32, u32))RSGL_GL1_create_atlas;
    proc.bitmapToAtlas = (void(*)(void*, RSGL_texture, u32, u32, u32, u8*, float, float, float*, float*))RSGL_GL1_bitmap_to_atlas;
	proc.createBuffer = (void (*)(void*, size_t, const void*, size_t*))RSGL_GL1_createBuffer;
	proc.updateBuffer = (void (*)(void*, size_t, void*, size_t, size_t))RSGL_GL1_updateBuffer;
	proc.deleteBuffer = (void (*)(void*, size_t))RSGL_GL1_deleteBuffer;
	return proc;
}

void RSGL_GL1_deleteTexture(RSGL_gl1Renderer* ctx, RSGL_texture tex) { glDeleteTextures(1, (u32*)&tex); }
void RSGL_GL1_viewport(RSGL_gl1Renderer* ctx, i32 x, i32 y, i32 w, i32 h) { glViewport(x, y, w ,h); }

void RSGL_GL1_clear(RSGL_gl1Renderer* ctx, float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RSGL_GL1_createBuffer(RSGL_gl1Renderer* ctx, size_t size, const void* data, size_t* buffer) {
	void* rawBuffer = RSGL_MALLOC(size);

	if (data)
		RSGL_GL1_updateBuffer(ctx, (size_t)rawBuffer, data, 0, size);

	if (buffer) *buffer = (size_t)rawBuffer;
}

void RSGL_GL1_updateBuffer(RSGL_gl1Renderer* ctx, size_t buffer, const void* data, size_t start, size_t end) {
	RSGL_MEMCPY(&((u8*)buffer)[start], data, end - start);
}

void RSGL_GL1_deleteBuffer(RSGL_gl1Renderer* ctx, size_t buffer) {
	RSGL_FREE((void*)buffer);
}

void RSGL_GL1_initPtr(RSGL_gl1Renderer* ctx, void* proc) {
	RSGL_UNUSED(proc); RSGL_UNUSED(ctx);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RSGL_GL1_freePtr(RSGL_gl1Renderer* ctx) { RSGL_UNUSED(ctx); }

void RSGL_GL1_render(RSGL_gl1Renderer* ctx, const RSGL_programInfo* program, const float* matrix, const RSGL_renderBuffers* buffers) {
	size_t i, j;

	float* colors = (float*)buffers->color;
	float* verts = (float*)buffers->vertex;
	float* texCoords = (float*)buffers->texture;
	u16* elements = (u16*)buffers->elements;

	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf(matrix);

	for (i = 0; i < buffers->batchCount; i++) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, buffers->batches[i].tex);
		glLineWidth(buffers->batches[i].lineWidth);

		u32 mode = GL_TRIANGLES;
		switch (buffers->batches[i].type) {
			case RSGL_TRIANGLES: mode = GL_TRIANGLES; break;
			case RSGL_POINTS: mode = GL_POINTS; break;
			case RSGL_LINES:  mode = GL_LINES; break;
			default: break;
		}

		glPushMatrix();
		glMultMatrixf(buffers->batches[i].matrix.m);
		glBegin(mode);

		for (j = buffers->batches[i].elmStart; j < buffers->batches[i].elmStart + buffers->batches[i].elmCount; j++) {
			size_t vIndex = elements[j] * 3;
			size_t tIndex = elements[j] * 2;
			size_t cIndex = elements[j] * 4;
			glTexCoord2f(texCoords[tIndex], texCoords[tIndex + 1]);
			glColor4f(colors[cIndex], colors[cIndex + 1], colors[cIndex + 2], colors[cIndex + 3]);
			glVertex3f(verts[vIndex], verts[vIndex + 1],  verts[vIndex + 2]);
		}

		glEnd();
		glPopMatrix();
	}

	glPopMatrix();
}

void RSGL_GL1_scissorStart(RSGL_gl1Renderer* ctx, RSGL_rectF scissor, i32 renderer_height) {
    glEnable(GL_SCISSOR_TEST);
    glScissor(scissor.x, renderer_height - (scissor.y + scissor.h), scissor.w, scissor.h);
}

void RSGL_GL1_scissorEnd(RSGL_gl1Renderer* ctx) {
    glDisable(GL_SCISSOR_TEST);
}

#ifndef GL_RG
#define GL_RG                             0x8227
#endif

GLuint RSGL_GL1_textureFormatToNative(RSGL_textureFormat format) {
	switch (format) {
		case RSGL_formatRGB: return GL_RGB;
		case RSGL_formatBGR: return GL_BGR;
		case RSGL_formatRGBA: return GL_RGBA;
		case RSGL_formatBGRA: return GL_BGRA;
		case RSGL_formatRed: return GL_RED;
		default: break;
	}
	return GL_RGBA;
}

GLuint RSGL_GL1_textureDataTypeToNative(RSGL_textureDataType type) {
    switch (type) {
		case RSGL_textureDataInt: return GL_UNSIGNED_BYTE;
		case RSGL_textureDataFloat: return GL_FLOAT;
        default: break;
    }

	return GL_UNSIGNED_BYTE;
}

GLuint RSGL_GL1_textureFilterToNative(RSGL_textureFilter filter) {
    switch (filter) {
		case RSGL_filterNearest:  return GL_NEAREST;
		case RSGL_filterLinear: return GL_LINEAR;
        default: break;
    }

	return GL_LINEAR;
}

RSGL_texture RSGL_GL1_createTexture(RSGL_gl1Renderer* ctx, const RSGL_textureBlob* blob) {
    unsigned int id = 0;

    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, RSGL_GL1_textureFilterToNative(blob->minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, RSGL_GL1_textureFilterToNative(blob->magFilter));

    glPixelStorei(GL_UNPACK_ROW_LENGTH, blob->width);

	u32 dataFormat = RSGL_GL1_textureFormatToNative(blob->dataFormat);
	u32 textureFormat = RSGL_GL1_textureFormatToNative(blob->textureFormat);
	u32 dataType = RSGL_GL1_textureDataTypeToNative(blob->dataType);

	glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, blob->width, blob->height, 0, textureFormat, dataType, blob->data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

void RSGL_GL1_copyToTexture(RSGL_gl1Renderer* ctx, RSGL_texture texture, size_t x, size_t y, const RSGL_textureBlob* blob) {
	glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, blob->width);

	u32 dataFormat = RSGL_GL1_textureFormatToNative(blob->dataFormat);
	u32 dataType = RSGL_GL1_textureDataTypeToNative(blob->dataType);

	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, (i32)blob->width, (i32)blob->height, dataFormat, dataType, blob->data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

#ifndef GL_PERSPECTIVE_CORRECTION_HINT
#define GL_PERSPECTIVE_CORRECTION_HINT		0x0C50
#endif

#ifndef GL_TEXTURE_SWIZZLE_RGBA
#define GL_TEXTURE_SWIZZLE_RGBA           0x8E46
#endif

#ifndef GL_TEXTURE0
#define GL_TEXTURE0				0x84C0
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE			0x812F
#endif

#ifndef GL_UNPACK_ROW_LENGTH
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#endif

RSGL_texture RSGL_GL1_create_atlas(RSGL_gl1Renderer* ctx, u32 atlasWidth, u32 atlasHeight) {
	u32 id = 0;
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	u8* data = (u8*)RSGL_MALLOC(atlasWidth * atlasHeight * 4);
	RSGL_MEMSET(data, 0, atlasWidth * atlasHeight * 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	RSGL_FREE(data);

	glBindTexture(GL_TEXTURE_2D, id);
	static GLint swizzleRgbaParams[4] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);

	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}

void RSGL_GL1_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows);
void RSGL_GL1_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skipRows);
}

void RSGL_GL1_bitmap_to_atlas(RSGL_gl1Renderer* ctx, RFont_texture atlas, u32 atlasWidth, u32 atlasHeight, u32 maxHeight, u8* bitmap, float w, float h, float* x, float* y) {
	glBindTexture(GL_TEXTURE_2D, 0);
	GLint alignment, rowLength, skipPixels, skipRows;
	RSGL_UNUSED(ctx); RSGL_UNUSED(atlasHeight);
	if (((*x) + w) >= atlasWidth) {
		*x = 0;
		*y += (float)maxHeight;
	}

	glEnable(GL_TEXTURE_2D);

	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowLength);
	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skipPixels);
	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skipRows);

	glBindTexture(GL_TEXTURE_2D, (u32)atlas);

	RSGL_GL1_push_pixel_values(1, (i32)w, 0, 0);

	glTexSubImage2D(GL_TEXTURE_2D, 0, (i32)(*x), (i32)*y, (i32)w, (i32)h, GL_RED, GL_UNSIGNED_BYTE, bitmap);

	RSGL_GL1_push_pixel_values(alignment, rowLength, skipPixels, skipRows);

	glBindTexture(GL_TEXTURE_2D, 0);
	*x += w;
}

#endif /* RSGL_IMPLEMENTATION */
