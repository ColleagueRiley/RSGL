#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main() {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RSGL_CENTER);
    win->fpsCap = 80;

    RSGL_setFont(RSGL_loadFont("SansPosterBold.ttf"));

    RSGL_legacy(true);

    
    RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);

    for (;;) {
        RSGL_window_checkEvent(win);

        if (win->event.type == RSGL_quit)
            break;
        
        if (win->event.type == RSGL_keyPressed && RSGL_isPressedI(win, RGFW_Space))
            RSGL_legacy(!RGLinfo.legacy);
        
        RSGL_drawText(RSGL_strFmt("FPS : %i\nOpenGL %s", win->event.fps, RGLinfo.legacy ? "legacy (2-)" : "modern (3.3 +)"), RSGL_CIRCLE(0, 40, 40), RSGL_RGB(255, 0, 0));
        
        rotate.z++;
        RSGL_rotate(rotate);
        RSGL_drawRect(RSGL_RECT(200, 200, 200, 200), RSGL_RGB(255, 0, 0));

        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_window_close(win);
}
