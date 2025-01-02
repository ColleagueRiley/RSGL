

/* use buffer rendering instead of opengl */
#define RGFW_NO_API

#define RSGL_CUSTOM_RENDER

#ifdef RSGL_CUSTOM_RENDER
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#endif

void RSGL_renderDeleteTexture(u32 tex) { 

}
void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h) { 

}

void RSGL_renderClear(float r, float g, float b, float a) {

}

void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info) {

}
void RSGL_renderFree(void) {

}

void RSGL_renderBatch(RSGL_RENDER_INFO* info) { 
    size_t i, j;
    size_t tIndex = 0, cIndex = 0, vIndex = 0;
    for (i = 0; i < info->len; i++) {
        for (j = info->batches[i].start; j < info->batches[i].len; j++) {
            tIndex += 2;
            vIndex += 3;
            cIndex += 4;
        }
    }

    info->len = 0;
    info->vert_len = 0;
}

u32 RSGL_renderCreateTexture(u8* bitmap, RSGL_area memsize, u8 channels) {
    unsigned int id = 0;

    return id;
}

void RSGL_renderUpdateTexture(RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels) {
    
}

void RSGL_renderSetShaderValue(u32 program, char* var, float value[], u8 len) {

}

RSGL_programInfo RSGL_renderCreateProgram(const char* VShaderCode, const char* FShaderCode, char* posName, char* texName, char* colorName) {
    RSGL_programInfo program;

    return program;
}

void RSGL_renderDeleteProgram(RSGL_programInfo program) {

}

void RSGL_renderScissorStart(RSGL_rectF scissor) {

}

void RSGL_renderScissorEnd(void) {

}

u32 RFont_create_atlas(RSGL_texture atlasWidth, u32 atlasHeight) {
   u32 id = 0;
   return id;
}

b8 RFont_resize_atlas(RFont_texture* atlas, u32 newWidth, u32 newHeight) {
    return 1;
}

void RFont_bitmap_to_atlas(RSGL_texture atlas, u8* bitmap, float x, float y, float w, float h) {

}