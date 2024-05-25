#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main(void) {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RSGL_CENTER);

    RSGL_button button = RSGL_initButton(); 
    RSGL_button_setPolygon(&button, RSGL_RECT(200, 200, 200, 200), 8);
    RSGL_button_setColor(&button, RSGL_RGB(200, 0, 0));
    RSGL_button_setOutline(&button, 10, RSGL_RGB(100, 0, 0));

    RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));
    RSGL_button_setText(&button, "text", 5, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(0, 0, 0));
    RSGL_button_alignText(&button, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);

    RSGL_button press = RSGL_copyButton(button);

    RSGL_button_setColor(&press, RSGL_RGB(255, 0, 0));
    RSGL_button_setText(&press, "other text", 11, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(0, 0, 0));
    RSGL_button_alignText(&press, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);

    RSGL_button_setOnIdle(&button, button.src);
    RSGL_button_setOnHover(&button, button.src);
    RSGL_button_setOnPress(&button, press.src);

    /* generic button */
    RSGL_button generic = RSGL_initButton();
    /* this can be a rect or polygon */
    RSGL_button_setRect(&generic, RSGL_RECT(50, 50, 100, 50));
    
    RSGL_button_setText(&generic, "generic", 8, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(100, 100, 100));
    RSGL_button_alignText(&generic, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
    
    RSGL_button_setStyle(&generic, RSGL_STYLE_DARK | RSGL_STYLE_ROUNDED);

    RSGL_button_setKeybind(&generic, (u32[]){RGFW_F, RGFW_ControlL}, 2);
    RSGL_button_setWindow(&generic, win);

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

    RSGL_button slider = RSGL_initButton();
    RSGL_button_setRect(&slider, RSGL_RECT(200, 450, 200, 15));    
    RSGL_button_setStyle(&slider, RSGL_STYLE_DARK | RSGL_STYLE_ROUNDED | RSGL_STYLE_SLIDER_HORIZONTAL | RSGL_STYLE_SLIDER_CIRCLE);

    float slider_value;

    bool running = true;
    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            RSGL_button_update(&button, win->event);
            RSGL_button_update(&generic, win->event);
            RSGL_button_update(&genericToggle, win->event);
            RSGL_button_update(&checkbox, win->event);
            RSGL_button_update(&radioButtons, win->event);
            RSGL_button_update(&comboBox, win->event);
            slider_value = RSGL_slider_update(&slider, win->event);
        }

        RSGL_drawButton(button);
        RSGL_drawButton(generic);
        RSGL_drawButton(genericToggle);
        RSGL_drawButton(checkbox);
        RSGL_drawButton(radioButtons);
        RSGL_drawButton(comboBox);
        RSGL_drawButton(slider);

        RSGL_drawText(RSGL_strFmt("slider : %i", (i32)slider_value), RSGL_CIRCLE(100, 440, 25), RSGL_RGB(255, 255, 255));

        RSGL_window_clear(win, RSGL_RGB(20, 20, 60));
    }

    RSGL_window_close(win);
}