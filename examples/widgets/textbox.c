#define RSGL_IMPLEMENTATION
#include "RSGL.h"


int main(void) {
    RGFW_window* win = RGFW_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RGFW_CENTER);

    // RSGL_textbox_setTextInfo(tb, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    // RSGL_textbox_setRect(tb, RSGL_RECT(20, 20, 200, 30));

    // RSGL_textbox_setTextInfo(tb2, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    // RSGL_textbox_setRect(tb2, RSGL_RECT(20, 100, 300, 300));
    
	bool running = true;
    while (running) {
        while (RSGL_checkEvent(win)) {
            if (win->event.type == RGFW_quit) {
                running = false;
                break;
            }
			
            if (win->event.type == RGFW_keyPressed && !(RSGL_between(win->event.keyCode, '0', '9') || win->event.keyCode == RGFW_BackSpace))
                continue;
		} 
		 
        RSGL_clear(RSGL_RGB(20, 20, 60));
		RGFW_window_swapBuffers(win);
	}

	RSGL_free();
    RGFW_window_close(win);
}
