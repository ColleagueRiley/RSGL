#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main(void) {
    RGFW_window* win = RGFW_createWindow("name", (RGFW_rect){500, 500, 500, 500}, RGFW_CENTER);

	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

    RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));
    
	u32 fps;

    for (; RGFW_window_shouldClose(win) == false;) {
        RGFW_window_checkEvent(win);

        if (win->event.type == RGFW_quit)
            break;

        RSGL_drawText("Text example\nnewlines too", RSGL_CIRCLE(200, 200, 20), RSGL_RGB(255, 0, 0));
        RSGL_drawText(RSGL_strFmt("FPS : %i\nOpenGL %s", fps, RSGL_args.legacy ? "legacy (2-)" : "modern (3.3 +)"), RSGL_CIRCLE(0, 40, 40), RSGL_RGB(255, 0, 0));
        
		RSGL_clear(RSGL_RGB(255, 255, 255));
		RGFW_window_swapBuffers(win);

		fps = RGFW_window_checkFPS(win, 0);
	}
	
	RSGL_free();
	RGFW_window_close(win);
}
