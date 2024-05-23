#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main(void) {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RSGL_CENTER);

    RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));
    
    for (;;) {
        RSGL_window_checkEvent(win);

        if (win->event.type == RSGL_quit)
            break;

        RSGL_drawText("Text example\nnewlines too", RSGL_CIRCLE(200, 200, 20), RSGL_RGB(255, 0, 0));
        RSGL_drawText(RSGL_strFmt("FPS : %i\nOpenGL %s", win->event.fps, RGLinfo.legacy ? "legacy (2-)" : "modern (3.3 +)"), RSGL_CIRCLE(0, 40, 40), RSGL_RGB(255, 0, 0));
        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_window_close(win);
}
