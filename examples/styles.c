#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main(void) {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RSGL_CENTER);

    RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));
     
    /* 7 rows of 3 buttons */
    RSGL_button buttons[7 * 3];

    /* init the buttons */
    u8 i = 0;
    for (i = 0; i < 7 * 3; i++)
        buttons[i] = RSGL_initButton();

    /* this button enables / disables darkmode */
    RSGL_button darkMode = RSGL_initButton();
    RSGL_button_setPolygon(&darkMode, RSGL_RECT(20, 20, 50, 25), 36);
    RSGL_button_setStyle(&darkMode, RSGL_STYLE_DARK | RSGL_STYLE_TOGGLE | RSGL_STYLE_ROUNDED | RSGL_STYLE_PURPLE);

    u8 drawMode = RSGL_STYLE_DARK;

    bool running = true;
    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            /* check all the buttons*/
            RSGL_button_update(&darkMode, win->event);
            drawMode = (darkMode.toggle) ? RSGL_STYLE_LIGHT : RSGL_STYLE_DARK;

            for (i = 0; i < sizeof(buttons) / sizeof(RSGL_button); i += 3) {
                size_t row = (i / 3);

                RSGL_button_setRect(&buttons[i], RSGL_RECT(20, 50 + (row * 55), 100, 50));
                RSGL_button_setPolygon(&buttons[i + 1], RSGL_RECT(140, 50 + (row * 55), 100, 50), 36);
                RSGL_button_setRect(&buttons[i + 2], RSGL_RECT(260, 50 + (row * 55), 50, 50));

                RSGL_button_update(&buttons[i], win->event);
                RSGL_button_update(&buttons[i + 1], win->event);
                RSGL_button_update(&buttons[i + 2], win->event);
            }
        }

        RSGL_drawButton(darkMode);

        size_t startColor = log2(RSGL_STYLE_RED) - 1;

        for (i = 0; i < sizeof(buttons) / sizeof(RSGL_button); i += 3) {
            size_t row = (i / 3);

            u32 styleColor = 0;
            if (i != 0)
                styleColor = (1 << (startColor + row)); /* 
                    this sets the style to RSGL_STYLE_RED (the first color) + the current row
                    making it so it loops through each color
                */
                 
            /* set the button data */

            RSGL_button_setRect(&buttons[i], RSGL_RECT(20, 50 + (row * 55), 100, 50));
            RSGL_button_setText(&buttons[i], "generic", 8, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(100, 100, 100));
            RSGL_button_alignText(&buttons[i], RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
            RSGL_button_setStyle(&buttons[i], styleColor | drawMode | RSGL_STYLE_ROUNDED);
            
            RSGL_button_setPolygon(&buttons[i + 1], RSGL_RECT(140, 50 + (row * 55), 100, 50), 36);
            RSGL_button_setStyle(&buttons[i + 1], styleColor | drawMode | RSGL_STYLE_TOGGLE | RSGL_STYLE_ROUNDED);
            
            RSGL_button_setRect(&buttons[i + 2], RSGL_RECT(260, 50 + (row * 55), 50, 50));
            RSGL_button_setStyle(&buttons[i + 2], styleColor | drawMode | RSGL_STYLE_CHECKBOX);
            
            /* draw the buttons */
            RSGL_drawButton(buttons[i]);
            RSGL_drawButton(buttons[i + 1]);
            RSGL_drawButton(buttons[i + 2]);
        }

        if (drawMode == RSGL_STYLE_DARK)
            RSGL_window_clear(win, RSGL_RGB(20, 20, 60));
        else
            RSGL_window_clear(win, RSGL_RGB(190, 190, 230));
    }

    RSGL_window_close(win);
}