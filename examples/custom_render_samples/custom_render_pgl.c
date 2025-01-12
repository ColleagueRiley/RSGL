#define RGFW_BUFFER
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_IMPLEMENTATION
#include "custom_render_pgl.h"

int main() {
    RGFW_window* win = RGFW_createWindow("name", RSGL_RECT(0, 0, 500, 500), RGFW_windowCenter);

	RSGL_init(RSGL_AREA(win->r.w, win->r.h), win->buffer);    
    RSGL_setFont(RSGL_loadFont("COMICSANS.ttf"));

    while (RGFW_window_shouldClose(win)) {
        while (RGFW_window_checkEvent(win));
        
        RSGL_clear(RSGL_RGB(255, 255, 255));
        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
        
        RSGL_draw();
		RGFW_window_swapBuffers(win);
    }

    RSGL_free();
    RGFW_window_close(win);
}
