#define RSGL_IMPLEMENTATION
#include "RSGL_rgfw.h"

int main(void) {
    RGFW_window* win = RGFW_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RGFW_CENTER);

	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

    // RSGL_textbox_setTextInfo(tb, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    // RSGL_textbox_setRect(tb, RSGL_RECT(20, 20, 200, 30));

    // RSGL_textbox_setTextInfo(tb2, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    // RSGL_textbox_setRect(tb2, RSGL_RECT(20, 100, 300, 300));
    
    while (RGFW_window_shouldClose(win) == false) {
        while (RSGL_checkEvent(win)) {
            if (win->event.type == RGFW_quit) {
                break;
            }	
		} 
		 
        RSGL_clear(RSGL_RGB(20, 20, 60));
		RGFW_window_swapBuffers(win);
	}

	RSGL_free();
    RGFW_window_close(win);
}
