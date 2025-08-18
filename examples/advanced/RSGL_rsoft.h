#ifndef RSGL_RSOFT_H
#define RSGL_RSOFT_H

RSGLDEF RSGL_renderer RSGL_RSoft_renderer(void);

RSGLDEF void RSGL_RSoft_batch(RSGL_RENDER_INFO* info);
RSGLDEF void RSGL_RSoft_init(void* proc, RSGL_RENDER_INFO* info); /* init render backend */
RSGLDEF void RSGL_RSoft_free(void); /* free render backend */
RSGLDEF void RSGL_RSoft_clear(float r, float g, float b, float a);
RSGLDEF void RSGL_RSoft_viewport(i32 x, i32 y, i32 w, i32 h);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_RSoft_createTexture(u8* bitmap, RSGL_area memsize,  u8 channels);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_RSoft_updateTexture(RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels);
/* delete a texture */
RSGLDEF void RSGL_RSoft_deleteTexture(RSGL_texture tex);
/* starts scissoring */
RSGLDEF void RSGL_RSoft_scissorStart(RSGL_rectF scissor);
/* stops scissoring */
RSGLDEF void RSGL_RSoft_scissorEnd(void);
/* program loading */
RSGLDEF RSGL_programInfo RSGL_RSoft_createProgram(const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName);
RSGLDEF void RSGL_RSoft_deleteProgram(RSGL_programInfo program);
RSGLDEF void RSGL_RSoft_setShaderValue(u32 program, char* var, float value[], u8 len);
/* RFont */
RSGL_texture RSGL_RSoft_create_atlas(u32 atlasWidth, u32 atlasHeight);
u8 RSGL_RSoft_resize_atlas(RSGL_texture* atlas, u32 newWidth, u32 newHeight);
void RSGL_RSoft_bitmap_to_atlas(RSGL_texture atlas, u8* bitmap, float x, float y, float w, float h);

#endif


#ifdef RSGL_IMPLEMENTATION
#define RGFW_BUFFER
#include "RSGL.h"

#define RSOFT_IMPLEMENTATION
#include "RSoft.h"

RSGL_renderer RSGL_RSoft_renderer() {
    RSGL_renderer renderer;
    renderer.render = RSGL_RSoft_batch;
    renderer.initPtr = RSGL_RSoft_init;
    renderer.freePtr = RSGL_RSoft_free;
    renderer.clear = RSGL_RSoft_clear;
    renderer.viewport = RSGL_RSoft_viewport;
    renderer.createTexture = RSGL_RSoft_createTexture;
    renderer.updateTexture = RSGL_RSoft_updateTexture;
    renderer.deleteTexture = RSGL_RSoft_deleteTexture;
    renderer.scissorStart = RSGL_RSoft_scissorStart;
    renderer.scissorEnd =  RSGL_RSoft_scissorEnd;
    renderer.createProgram = RSGL_RSoft_createProgram;
    renderer.deleteProgram = RSGL_RSoft_deleteProgram;
    renderer.setShaderValue = RSGL_RSoft_setShaderValue;
    renderer.createAtlas = RSGL_RSoft_create_atlas;
    renderer.resizeAtlas = RSGL_RSoft_resize_atlas;
    renderer.bitmapToAtlas = RSGL_RSoft_bitmap_to_atlas;
    return renderer;
}



typedef struct RSGL_INFO {
	u8* buffer;
} RSGL_INFO;

RSGL_INFO RSGL_rsoft;

typedef struct RSGL_rsoft_texture {
	u8* bitmap;
	RSGL_area memsize;
	u8 channels;

	struct RSGL_rsoft_texture* next;
	struct RSGL_rsoft_texture* prev;
} RSGL_rsoft_texture;

RSGL_rsoft_texture* RSGL_rsoft_textures = NULL;

void RSGL_RSoft_deleteTexture(RSGL_texture tex) {
	RSGL_rsoft_texture* texture = (RSGL_rsoft_texture*)tex;
	if (texture->prev != NULL)
		texture->prev->next = texture->next;
	else
		RSGL_rsoft_textures = NULL;

	free(texture->bitmap);
	free(texture);
}
void RSGL_RSoft_viewport(i32 x, i32 y, i32 w, i32 h) {
	RSoft_setCanvasSize(RSOFT_AREA(w, h));
}

void RSGL_RSoft_clear(float r, float g, float b, float a) {
	u8 color[4] = {r * 255, g * 255, b * 255, a * 255};

	RSoft_clear(RSGL_rsoft.buffer, color);
}

void RSGL_RSoft_init(void* buffer, RSGL_RENDER_INFO* info) {
    RSGL_UNUSED(info);
	RSGL_rsoft.buffer = buffer;
}

void RSGL_RSoft_free(void) {
	if (RSGL_rsoft_textures != NULL) {
		RSGL_rsoft_texture* texture = RSGL_rsoft_textures;
		while (texture != NULL) {
			RSGL_rsoft_texture* next = texture->next;
			free(texture->bitmap);
			free(texture);
			texture = next;
		}
	}
}

void RSGL_RSoft_batch(RSGL_RENDER_INFO* info) {
        size_t i, j;
        size_t tIndex = 0, cIndex = 0, vIndex = 0;

		for (i = 0; i < info->len; i++) {
            u32 mode = info->batches[i].type;
            if (mode > 0x0100)
                mode -= 0x0100;
            if (mode > 0x0010)
                mode -= 0x0010;

            // glDrawArrays(mode, info->batches[i].start, info->batches[i].len);
			RSGL_rsoft_texture* tex = (RSGL_rsoft_texture*)info->batches[i].tex;

			switch (mode) {
				case RSGL_TRIANGLES:
					for (j = info->batches[i].start; j < info->batches[i].len ; j += 3) {
						// info->texCoords[tIndex], info->texCoords[tIndex + 1]
						RSoft_vector npoints[3] = {
													RSOFT_VECTOR3D(info->verts[vIndex], info->verts[vIndex + 1],  info->verts[vIndex + 2]),
													RSOFT_VECTOR3D(info->verts[vIndex + 3], info->verts[vIndex + 4],  info->verts[vIndex + 5]),
													RSOFT_VECTOR3D(info->verts[vIndex + 6], info->verts[vIndex + 7],  info->verts[vIndex + 8])
												 };
						u8 color[4] = {info->colors[cIndex] * 255, info->colors[cIndex + 1] * 255,
										info->colors[cIndex + 2] * 255, info->colors[cIndex + 3] * 255};

//						RSoft_rect texRect = {info->texCoords[tIndex] * tex->memsize.w, info->texCoords[tIndex + 1] * tex->memsize.h,
//											tex->memsize.w, tex->memsize.h};
	//					RSoft_setTexture(tex->bitmap, texRect, *(RGFW_area*)&tex->memsize);

                        RSoft_drawTriangleF(RSGL_rsoft.buffer, npoints, color);


						tIndex += 6;
						vIndex += 9;
						cIndex += 12;
					}
					break;
				case RSGL_LINES:
					for (j = info->batches[i].start; j < info->batches[i].len; j += 2) {
						// info->texCoords[tIndex], info->texCoords[tIndex + 1]
						u8 color[4] = {info->colors[cIndex] * 255, info->colors[cIndex + 1] * 255,
										info->colors[cIndex + 2] * 255, info->colors[cIndex + 3] * 255};

						RSoft_drawLineF(RSGL_rsoft.buffer,
										RSOFT_VECTOR3D(info->verts[vIndex], info->verts[vIndex + 1],  info->verts[vIndex + 2]),
										RSOFT_VECTOR3D(info->verts[vIndex + 3], info->verts[vIndex + 4],  info->verts[vIndex + 5]),
										color);

						tIndex += 4;
						vIndex += 6;
						cIndex += 8;
					}
					break;
				case RSGL_TRIANGLE_FAN: {
					size_t start = vIndex + 3;
					for (j = info->batches[i].start; j < info->batches[i].len; j += 3) {
						// info->texCoords[tIndex], info->texCoords[tIndex + 1]
						RSoft_vector npoints[3] = {
							RSOFT_VECTOR3D(300, 300, info->verts[start + 2]),
							RSOFT_VECTOR3D(info->verts[vIndex], info->verts[vIndex + 1], info->verts[vIndex + 2]),
							RSOFT_VECTOR3D(info->verts[vIndex + 3], info->verts[vIndex + 4], info->verts[vIndex + 5])
						};

						u8 color[4] = {info->colors[cIndex] * 255, info->colors[cIndex + 1] * 255,
									info->colors[cIndex + 2] * 255, info->colors[cIndex + 3] * 255};

						/*printf("{%f %f %f}, {%f %f %f}, {%f %f %f}\n",
							info->verts[vIndex], info->verts[vIndex + 1], info->verts[vIndex + 2],
							info->verts[vIndex + 3], info->verts[vIndex + 4], info->verts[vIndex + 5],
							info->verts[vIndex + 6], info->verts[vIndex + 7], info->verts[vIndex + 8]);*/

						RSoft_drawTriangleF(RSGL_rsoft.buffer, npoints, color);

						tIndex += 6;
						vIndex += 9;
						cIndex += 12;
					}
					break;
				}
				default: break;
			}
		}

    info->len = 0;
    info->vert_len = 0;
}

void RSGL_RSoft_scissorStart(RSGL_rectF scissor) {
    RSGL_draw();

	// TODO
}

void RSGL_RSoft_scissorEnd(void) {
    RSGL_draw();
	// TODO
}

/* textures / images */
RSGL_texture RSGL_RSoft_createTexture(u8* bitmap, RSGL_area memsize, u8 channels) {
	RSGL_rsoft_texture* tex = (RSGL_rsoft_texture*)RSGL_MALLOC(sizeof(RSGL_rsoft_texture));
	*tex = (RSGL_rsoft_texture){(u8*)RSGL_MALLOC(memsize.w * memsize.h * channels), memsize, channels, NULL, NULL};

	if (bitmap != NULL)
		memcpy(tex->bitmap, bitmap, memsize.w * memsize.h * channels);

	if (RSGL_rsoft_textures == NULL)
		RSGL_rsoft_textures = tex;
	else {
		RSGL_rsoft_textures->prev = RSGL_rsoft_textures;
		RSGL_rsoft_textures->next = tex;
	}

    return (u64)tex;
}

void RSGL_RSoft_updateTexture(RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels) {
	RSGL_rsoft_texture* tex = (RSGL_rsoft_texture*)texture;
	tex->memsize = memsize;
	tex->channels = channels;
	tex->bitmap = RSGL_REALLOC(tex->bitmap, memsize.w * memsize.h * channels);
	memcpy(tex->bitmap, bitmap, memsize.w * memsize.h * channels);

	free(tex->bitmap);
}

RSGL_texture RSGL_RSoft_create_atlas(u32 atlasWidth, u32 atlasHeight) {
	return (u64)RSGL_renderCreateTexture(NULL, RSGL_AREA(atlasWidth, atlasHeight), 4);
}

u8 RSGL_RSoft_resize_atlas(RSGL_texture* atlas, u32 newWidth, u32 newHeight) {
	RSGL_rsoft_texture* texture = (RSGL_rsoft_texture*)*atlas;
	RSGL_rsoft_texture* newTexture = (RSGL_rsoft_texture*)RSGL_MALLOC(sizeof(RSGL_rsoft_texture));
	*newTexture = (RSGL_rsoft_texture){(u8*)RSGL_MALLOC(newWidth * newHeight * texture->channels), RSGL_AREA(newWidth, newHeight), texture->channels, NULL, NULL};

	memcpy(newTexture->bitmap, texture->bitmap, texture->memsize.w * texture->memsize.h * texture->channels);
	RSGL_renderDeleteTexture(*atlas);
	*atlas = (u64)newTexture;

	return 1;
}

void RSGL_RSoft_bitmap_to_atlas(RSGL_texture atlas, u8* bitmap, float x, float y, float w, float h) {
	RSGL_rsoft_texture* tex = (RSGL_rsoft_texture*)atlas;

	for (float i = 0; i < h; i++) {
		memcpy(tex->bitmap + (u32)(((i + y) * tex->memsize.w * 4) + x), bitmap + (u32)(i * w * 4),
				tex->memsize.w * tex->channels);
	}
}

void RSGL_RSoft_setShaderValue(u32 program, char* var, float value[], u8 len) { RSGL_UNUSED(program); RSGL_UNUSED(value); RSGL_UNUSED(len);}
void RSGL_RSoft_deleteProgram(RSGL_programInfo program) { }
RSGL_programInfo RSGL_RSoft_createProgram(const char* VShaderCode, const char* FShaderCode, const char* posName, const char* texName, const char* colorName) {
	RSGL_UNUSED(VShaderCode); RSGL_UNUSED(FShaderCode); RSGL_UNUSED(posName); RSGL_UNUSED(texName); RSGL_UNUSED(colorName);
	return (RSGL_programInfo) {};
}
#endif
