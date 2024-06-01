

/* use buffer rendering instead of opengl */
#define RGFW_BUFFER

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

void RSGL_renderUpdateTexture(u32 texture, u8* bitmap, RSGL_area memsize, u8 channels) {
    
}

u32 RFont_create_atlas(u32 atlasWidth, u32 atlasHeight) {
   u32 id = 0;
   return id;
}

void RFont_bitmap_to_atlas(u32 atlas, u8* bitmap, float x, float y, float w, float h) {

}