#define RSGL_NO_AUDIO /* RSGL uses miniaudio.h, and I don't want to compile it if I'm not using it */
#define RSGL_IMPLEMENTATION
#define RGL_NO_RENDER
#define RSGL_CUSTOM_DRAW
#define RFONT_RENDER_LEGACY
#define RFONT_NO_OPENGL
#define RSGL_NO_TEXTURE_DEFINE

/* use buffer rendering instead of opengl */
#define RGFW_BUFFER

#define RSGL_viewport customViewport
#define RSGL_api_clear customClear
#define RSGL_api_clearColor customClearColor
#define RSGL_deleteTexture(texture) customDeleteTextures(1, &texture);
#define RSGL_deleteTextures(texture, num) customDeleteTextures(num, &texture);

#include "RSGL.h"


u32 RFont_create_atlas(u32 atlasWidth, u32 atlasHeight) {
    
}

void RFont_bitmap_to_atlas(u32 atlas, u8* bitmap, float x, float y, float w, float h) {

}

void customViewport(int x, int y, int w, int h) {

}

void customClear(int arg) {

}

void customClearColor(int r, int g, int b, int a) {

}

void customDeleteTextures(int count, int* textures) {

}

u32 RSGL_createTexture(u8* bitmap, RSGL_area memsize, u8 channels) {

}

void RSGL_updateTexture(u32 texture, u8* bitmap, RSGL_area memsize, u8 channels) {

}

void RSGL_pushPixelValues(i32 alignment, i32 rowLength, i32 skipPixels, i32 skipRows) {

}

void RSGL_textureSwizzleMask(u32 atlas, u32 param, i32 swizzleRgbaParams[4]) {

}

void RSGL_atlasAddBitmap(u32 atlas, u8* bitmap, float x, float y, float w, float h) {

}

void RSGL_basicDraw(u32 GL_TYPE, float* points, float* texPoints, RSGL_point3DF center, RSGL_color c, size_t len) {  
    i32 i;
    
    if (GL_TYPE > 0x0010)
        GL_TYPE -= 0x0010;
    
    size_t pIndex = 0;
    size_t tIndex = 0;

    for (i = 0; i < len; i++) {
        if (i && i <= RSGL_args.gradient_len) {

        }
        
        pIndex += 3;
        tIndex += 2;
    }

    if (RSGL_argsClear) {
        RSGL_setTexture(0);
        RSGL_clearArgs();
    }
}

void RFont_render_set_color(float r, float g, float b, float a) {

}

void RFont_render_text(u32 atlas, float* verts, float* tcoords, size_t nverts) {

}

void RFont_render_free(u32 atlas) {  }
void RFont_render_legacy(u8 legacy) { }
void RFont_render_init() {}

int main() {
    RSGL_window* win = RSGL_createWindow("name", RSGL_RECT(0, 0, 500, 500), RSGL_CENTER);

    u32 easy_font = RSGL_loadFont("Super Easy.ttf");
    RSGL_setFont(easy_font);

    /* generic button */
    RSGL_button generic = RSGL_initButton();
    
    /* this can be a rect or polygon */
    RSGL_button_setRect(&generic, RSGL_RECT(50, 50, 100, 50));
    
    RSGL_button_setText(&generic, "generic", 8, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(100, 100, 100));
    RSGL_button_alignText(&generic, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
    
    RSGL_button_setStyle(&generic, RSGL_STYLE_DARK | RSGL_STYLE_ROUNDED);

    RSGL_button_setWindow(&generic, win);

    u32 comicSans = RSGL_loadFont("COMICSANS.ttf");

    RSGL_textbox* tb = RSGL_initTextbox(0);
    RSGL_textbox_setTextInfo(tb, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    
    RSGL_textbox_setRect(tb, RSGL_RECT(20, 20, 200, 30));
    RSGL_textbox_setStyle(tb, RSGL_STYLE_DARK);
    RSGL_textbox_alignText(tb, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);

    bool running = true;

    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            RSGL_button_update(&generic, win->event);
            RSGL_textbox_update(tb, win->event);
        }

        RSGL_setFont(easy_font);
        RSGL_drawButton(generic);

        RSGL_setFont(comicSans);
        RSGL_textbox_draw(tb);

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
        
        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_textbox_free(tb);

    RSGL_window_close(win);
}