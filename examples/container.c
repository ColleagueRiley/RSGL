#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main(void) {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RSGL_CENTER);

    /* generic button */
    RSGL_button generic = RSGL_initButton();
    /* this can be a rect or polygon */
    RSGL_button_setRect(&generic, RSGL_RECT(50, 50, 100, 50));
    
    RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));

    RSGL_button_setText(&generic, "generic", 8, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(100, 100, 100));
    RSGL_button_alignText(&generic, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);

    RSGL_button_setStyle(&generic, RSGL_STYLE_DARK | RSGL_STYLE_ROUNDED);

    RSGL_button comboBox = RSGL_initButton();
    char* combos[3] = {"comboBox 0", "comboBox 1", "comboBox 2"};
    RSGL_button_setCombo(&comboBox, combos, 3);

    /* this can be a rect or polygon */
    RSGL_button_setRect(&comboBox, RSGL_RECT(200, 50, 200, 50));
    RSGL_button_setText(&comboBox, "", 11, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(100, 100, 100));
    RSGL_button_alignText(&comboBox, RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE);
    RSGL_button_setStyle(&comboBox, RSGL_STYLE_DARK | RSGL_STYLE_COMBOBOX);

    /* generic toggle button */
    RSGL_button genericToggle = RSGL_initButton();
    RSGL_button_setPolygon(&genericToggle, RSGL_RECT(50, 125, 100, 50), 36);

    RSGL_button_setStyle(&genericToggle, RSGL_STYLE_DARK | RSGL_STYLE_TOGGLE | RSGL_STYLE_ROUNDED);

    /* generic checkbox button */
    RSGL_button checkbox = RSGL_initButton();
    RSGL_button_setRect(&checkbox, RSGL_RECT(50, 250, 50, 50));
    RSGL_button_setStyle(&checkbox, RSGL_STYLE_DARK | RSGL_STYLE_CHECKBOX);

    /* generic radio buttons */
    RSGL_button radioButtons = RSGL_initButton();
    RSGL_button_setPolygon(&radioButtons, RSGL_RECT(50, 320, 15, 15), 36);
    RSGL_button_setRadioCount(&radioButtons, 3);
    RSGL_button_setStyle(&radioButtons, RSGL_STYLE_DARK | RSGL_STYLE_RADIO);

    /* slider button */
    RSGL_button slider = RSGL_initButton();
    RSGL_button_setRect(&slider, RSGL_RECT(200, 450, 200, 15));    
    RSGL_button_setStyle(&slider, RSGL_STYLE_DARK | RSGL_STYLE_ROUNDED | RSGL_STYLE_SLIDER_HORIZONTAL | RSGL_STYLE_SLIDER_CIRCLE);
    
    RSGL_button null = RSGL_nullButton();
    RSGL_button label = RSGL_label("label", 6, 25);
   
    RSGL_button* buttons[] = {
        &label, &generic, &null, 
        &checkbox, &null, &generic,
        &null, &null, &null,
        &radioButtons, &null, &genericToggle,
        &comboBox, &null, &null, &slider
    };

    RSGL_container* container = RSGL_initContainer(RSGL_RECT(50, 50, 300, 400), buttons, 13);
    RSGL_container_setTitle(container, "title", 6);
    RSGL_container_setStyle(container, RSGL_STYLE_DARK | RSGL_STYLE_ROUNDED | RSGL_STYLE_CONTAINER);

    bool running = true;
    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            if (win->event.type == RSGL_mouseButtonReleased && win->event.button == RSGL_mouseRight)
                RSGL_container_setPos(container, win->event.point);
            
            RSGL_container_update(container, win->event);
        }

        RSGL_drawContainer(container);

        RSGL_window_clear(win, RSGL_RGB(20, 20, 60));
    }

    RSGL_freeContainer(container);
    RSGL_window_close(win);
}
