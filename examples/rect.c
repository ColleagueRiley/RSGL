#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION

#include "RSGL.h"

int main () {
    RSGL_window* win = RSGL_createWindow("window", (RSGL_rect){200, 200, 500, 500}, 0);

    while (true) {
        while(RSGL_window_checkEvent(win)) {   
            if (win->event.type == RGFW_quit)
                break;
        }
        
        if (win->event.type == RGFW_quit)
            break;

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));

        RSGL_window_clear(win, RSGL_RGB(100, 100, 100));
    }

    RSGL_window_close(win);
}