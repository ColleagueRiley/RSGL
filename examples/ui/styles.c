#define RSGL_IMPLEMENTATION
#include "RSGL_rgfw.h"

int main(void) {
    RGFW_window* win = RGFW_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RGFW_CENTER);
	
	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

    RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));
     
    /* this button enables / disables darkmode */
    b8 darkToggle = 0;

	b8 toggle[7] = {0};
	b8 checkbox[7] = {0};

	float slider_value[7] = {0};
	b8 grabbed[7] = {0}; 

	
	size_t startColor = log2(RSGL_STYLE_RED) - 1;

    while (RGFW_window_shouldClose(win) == false) {
        while (RSGL_checkEvent(win)) {
            if (win->event.type == RGFW_quit) {
                break;
            }
		}

        /* check all the buttons*/
        u32 drawMode = (!darkToggle) ? RSGL_STYLE_DARK : RSGL_STYLE_LIGHT;
		RSGL_toggleButton(RSGL_RECTF(20, 20, 50, 25), drawMode |  RSGL_STYLE_ROUND, &darkToggle);

		u32 i;
        for (i = 0; i < 7; i++) {
			/* 
                    this sets the style to RSGL_STYLE_RED (the first color) + the current row
                    making it so it loops through each color
            */
            u32 styleColor = (1 << (startColor + i)) |  drawMode;     

			RSGL_openBlankContainer(RSGL_RECT(0, 0, win->r.w, win->r.h));
			
			RSGL_widgetAlign(RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
			RSGL_labeledButton("generic", RSGL_RECTF(20, 50 + (i * 55), 100, 50), styleColor | RSGL_STYLE_ROUND);

			RSGL_widgetRounding(RSGL_POINT(20, 20));
			RSGL_toggleButton(RSGL_RECTF(140, 50 + (i * 55), 100, 50), styleColor |  RSGL_STYLE_ROUND, &toggle[i]);	
			RSGL_checkbox(RSGL_RECTF(260, 50 + (i * 55), 50, 50), styleColor | RSGL_STYLE_ROUND, &checkbox[i]);

			RSGL_slider(RSGL_RECTF(340, 60 + (i * 55), 100, 20), styleColor, &slider_value[i], &grabbed[i]);
		}

        if (drawMode == RSGL_STYLE_DARK)
            RSGL_clear(RSGL_RGB(20, 20, 60));
        else
            RSGL_clear(RSGL_RGB(190, 190, 230));
    
		RGFW_window_swapBuffers(win);
	}
	
	RSGL_free();
    RGFW_window_close(win);
}
