#define RSGL_IMPLEMENTATION
#include "RSGL_rgfw.h"

int main(void) {
    RGFW_setGLSamples(12);
    RGFW_window* win = RGFW_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RGFW_CENTER);

	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

	RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));
	
	b8 checkbox = 0,
	   toggle = 0,
	   combo_open = 0;

	size_t combo_index = 0;
	u8 selected = 0, index;

	RSGL_rect rect = RSGL_RECT(50, 50, 400, 400);
	b8 grabbed = 0, c_toggle = 1;

    while (RGFW_window_shouldClose(win) == false) {
        while (RSGL_checkEvent(win)) {
            if (win->event.type == RGFW_quit) {
                break;
            }
        }
		
		RSGL_openContainer("title", &rect, RSGL_RGB(100, 10, 10), RSGL_STYLE_DARK, &grabbed, &c_toggle);
		
		RSGL_widgetAlign(RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
		RSGL_labeledButton("generic", RSGL_RECTF(0.10, 0.10, 100, 50), RSGL_STYLE_DARK | RSGL_STYLE_ROUND);
		
		RSGL_widgetPolygonPoints(8);
		
		RSGL_widgetAlign(RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE);
		
		char* combos[3] = {"comboBox 0", "comboBox 1", "comboBox 2"};
		RSGL_combobox(RSGL_RECTF(0.10, 0.90, 200, 50), RSGL_STYLE_DARK, combos, 3, &combo_open, &combo_index); 

		RSGL_checkbox(RSGL_RECTF(0.10, 0.30, 50, 50), RSGL_STYLE_DARK, &checkbox);	 		
	
		RSGL_widgetAlign(RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
		RSGL_labeledButton("generic", RSGL_RECTF(0.75, 0.10, 100, 50), RSGL_STYLE_DARK | RSGL_STYLE_ROUND);
		
		RSGL_widgetRounding(RSGL_POINT(20, 20));
		RSGL_toggleButton(RSGL_RECTF(0.75, 0.30, 100, 50), RSGL_STYLE_DARK | RSGL_STYLE_ROUND, &toggle);
		
		RSGL_widgetPolygonPoints(36);
		RSGL_radioButtons(RSGL_RECTF(0.10, 0.50, 30, 30), 3, RSGL_STYLE_DARK | RSGL_SHAPE_POLYGON, &selected, &index); 
			
        RSGL_clear(RSGL_RGB(20, 20, 60));
		RGFW_window_swapBuffers(win);
    }

	RSGL_free();
    RGFW_window_close(win);
}
