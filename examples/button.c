#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main() {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RSGL_CENTER);

    RSGL_button button = RSGL_initButton(); 
    RSGL_button_setPolygon(&button, RSGL_RECT(200, 200, 200, 200), 8);
    RSGL_button_setColor(&button, RSGL_RGB(200, 0, 0));
    RSGL_button_setOutline(&button, 10, RSGL_RGB(100, 0, 0));

    RSGL_setFont(RSGL_loadFont("SansPosterBold.ttf"));
    RSGL_button_setText(&button, "text", 5, RSGL_CIRCLE(0, 0, 15), RSGL_RGB(0, 0, 0));
    RSGL_button_alignText(&button, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);

    RSGL_rect rect = RSGL_RECT(100, 100, 50, 50);

    bool running = true;
    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            RSGL_button_update(&button, win->event);
            switch (button.status) {                
                case RSGL_pressed:
                    RSGL_button_setColor(&button, RSGL_RGB(255, 0, 0));
                    RSGL_button_setText(&button, "other text", 11, RSGL_CIRCLE(0, 0, 15), RSGL_RGB(0, 0, 0));
                    RSGL_button_alignText(&button, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
                    break;
                default: 
                    RSGL_button_setText(&button, "text", 5, RSGL_CIRCLE(0, 0, 15), RSGL_RGB(0, 0, 0));
                    RSGL_button_alignText(&button, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
                    RSGL_button_setColor(&button, RSGL_RGB(220, 0, 0));
                    break;
            }
        }

        RSGL_drawRect(rect, RSGL_RGB(255, 0, 0));
        RSGL_drawButton(button);

        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_window_close(win);
}