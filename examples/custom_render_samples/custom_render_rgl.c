#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#include "custom_render_rgl.h"

int main(void) {
    RGFW_window* win = RGFW_createWindow("name", RGFW_RECT(0, 0, 500, 500), RGFW_windowCenter);
	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

    u32 comicSans = RSGL_loadFont("COMICSANS.ttf");

    while (RGFW_window_shouldClose(win)) {
        while (RGFW_window_checkEvent(win));

        RSGL_clear(RSGL_RGB(255, 255, 255));

        RSGL_setFont(comicSans);
        
        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
        RSGL_drawRect((RSGL_rect){20, 20, 200, 200}, RSGL_RGB(0, 255, 0));
        // RSGL_drawText("text", RSGL_CIRCLE(250, 250, 20), RSGL_RGB(100, 100, 100));
       
        RSGL_draw();
        RGFW_window_swapBuffers(win);
    }
    
    RSGL_free();
    RGFW_window_close(win);
}