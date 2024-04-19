#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main() {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RSGL_CENTER);

    RSGL_setFont(RSGL_loadFont("COMICSANS.ttf"));
    
    RSGL_textbox* tb = RSGL_initTextbox(0);
    RSGL_textbox_setTextInfo(tb, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    
    RSGL_textbox_setRect(tb, RSGL_RECT(20, 20, 200, 30));
    RSGL_textbox_setStyle(tb, RSGL_STYLE_DARK);
    RSGL_textbox_alignText(tb, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);

    RSGL_textbox* tb2 = RSGL_initTextbox(0);
    RSGL_textbox_setTextInfo(tb2, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    
    RSGL_textbox_setRect(tb2, RSGL_RECT(20, 100, 300, 300));
    RSGL_textbox_setStyle(tb2, RSGL_STYLE_DARK);
    RSGL_textbox_alignText(tb2, RSGL_ALIGN_LEFT | RSGL_ALIGN_UP);

    bool running = true;
    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            RSGL_textbox_update(tb2, win->event);

            if (win->event.type == RGFW_keyPressed && !(RSGL_between(win->event.keyCode, '0', '9') || win->event.keyCode == RGFW_BackSpace))
                continue;
            
            RSGL_textbox_update(tb, win->event);
        } 

        RSGL_textbox_draw(tb);
        RSGL_textbox_draw(tb2);
        
        RSGL_window_clear(win, RSGL_RGB(20, 20, 60));
    }

    RSGL_textbox_free(tb);
    RSGL_textbox_free(tb2);

    RSGL_window_close(win);
}