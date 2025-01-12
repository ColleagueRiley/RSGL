#include "custom_render_template.h"

int main(void) {
    RSGL_window* win = RSGL_createWindow("name", RSGL_RECT(0, 0, 500, 500), RGFW_windowCenter);

    u32 easy_font = RSGL_loadFont("Super Easy.ttf");
    RSGL_setFont(easy_font);

    /* generic button */
    RSGL_button generic = RSGL_initButton();
    
    /* this can be a rect or polygon */
    RSGL_button_setRect(&generic, RSGL_RECT(50, 50, 100, 50));
    
    RSGL_button_setText(&generic, "generic", 8, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(100, 100, 100));
    RSGL_button_alignText(&generic, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
    
    RSGL_button_setStyle(&generic, RSGL_STYLE_DARK | RSGL_STYLE_ROUNDED);

    RSGL_button_setWindow(&generic, win);

    u32 comicSans = RSGL_loadFont("COMICSANS.ttf");

    RSGL_textbox* tb = RSGL_initTextbox(0);
    RSGL_textbox_setTextInfo(tb, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    
    RSGL_textbox_setRect(tb, RSGL_RECT(20, 20, 200, 30));
    RSGL_textbox_setStyle(tb, RSGL_STYLE_DARK);
    RSGL_textbox_alignText(tb, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);

    bool running = true;

    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            RSGL_button_update(&generic, win->event);
            RSGL_textbox_update(tb, win->event);
        }

        RSGL_setFont(easy_font);
        RSGL_drawButton(generic);

        RSGL_setFont(comicSans);
        RSGL_textbox_draw(tb);

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
        
        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_textbox_free(tb);

    RSGL_window_close(win);
}