#define RGL_LEGACY
#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main() {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RSGL_CENTER);
    win->fpsCap = 120;
    RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));

    RSGL_legacy(true);

    RSGL_button toggleLegacy = RSGL_initButton();
    RSGL_button_setPolygon(&toggleLegacy, RSGL_RECT(50, 125, 100, 50), 36);
    RSGL_button_setStyle(&toggleLegacy, RSGL_STYLE_LIGHT | RSGL_STYLE_TOGGLE | RSGL_STYLE_ROUNDED);

    RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);

    bool running = true;

    while (running) {
        while (RSGL_window_checkEvent(win)) {

            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            RSGL_button_update(&toggleLegacy, win->event);
            if ((win->event.type == RSGL_keyPressed && RSGL_isPressedI(win, RGFW_Space)) || 
                toggleLegacy.status == RSGL_pressed
            ) {
                RSGL_legacy(!RSGL_args.legacy);
                toggleLegacy.toggle = !RSGL_args.legacy;
            }
        }

        RSGL_drawText(RSGL_strFmt("FPS : %i\nOpenGL %s", win->event.fps, RGLinfo.legacy ? "legacy (2-)" : "modern (3.3 +)"), RSGL_CIRCLE(0, 40, 40), RSGL_RGB(255, 0, 0));
        
        rotate.z++;
        RSGL_rotate(rotate);
        RSGL_drawRect(RSGL_RECT(200, 200, 200, 200), RSGL_RGB(255, 0, 0));

        RSGL_drawButton(toggleLegacy);
        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_window_close(win);
}
