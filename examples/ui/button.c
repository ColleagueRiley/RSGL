#define RSGL_IMPLEMENTATION
#include "RSGL_rgfw.h"

int main(void) {
    RGFW_setGLSamples(12);
    RGFW_window* win = RGFW_createWindow("name", RGFW_RECT(500, 500, 500, 500), RGFW_CENTER);

	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

	RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));
	
    float slider_value = 0.0;	
	b8 slider_grabbed = 0,
	   checkbox = 0,
	   toggle = 0,
	   combo_open = 1;

	size_t combo_index = 0;
	u8 selected = 0, index;


    while (RGFW_window_shouldClose(win) == false) {
        while (RSGL_checkEvent(win)) {
            if (win->event.type == RGFW_quit) {
                break;
            }
        }
		
		RSGL_openBlankContainer(RSGL_RECT(0, 0, win->r.w, win->r.h));
		
		RSGL_widgetAlign(RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
		RSGL_labeledButton("generic", RSGL_RECTF(50, 50, 100, 50), RSGL_OFFSET_Y | RSGL_STYLE_DARK | RSGL_STYLE_ROUND);
		
		RSGL_widgetRounding(RSGL_POINT(20, 20));
		RSGL_toggleButton(RSGL_RECTF(50, 50, 100, 50), RSGL_STYLE_DARK | RSGL_STYLE_ROUND | RSGL_OFFSET_Y, &toggle);
	
		RSGL_widgetPolygonPoints(8);
		
		RSGL_widgetColor(RSGL_RGB(200, 0, 0), RSGL_RGB(200, 0, 0), RSGL_RGB(255, 0, 0), RSGL_RGB(200, 0, 0));
		RSGL_widgetOutline(10, RSGL_RGB(100, 0, 0), RSGL_RGB(100, 0, 0), RSGL_RGB(100, 0, 0), RSGL_RGB(100, 0, 0));

		if (RSGL_button(RSGL_RECTF(200, 200, 200, 200), RSGL_SHAPE_POLYGON) == RSGL_PRESSED) {
			RSGL_label("other text", RSGL_RECTF(200, 200, 200, 200));
		}
		else {
			RSGL_label("text", RSGL_RECTF(200, 200, 200, 200));
		}
				
		RSGL_widgetAlign(RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE);
		
		char* combos[3] = {"comboBox 0", "comboBox 1", "comboBox 2"};
		RSGL_combobox(RSGL_RECTF(200, 50, 200, 50), RSGL_STYLE_DARK, combos, 3, &combo_open, &combo_index); 

		RSGL_checkbox(RSGL_RECTF(50, 250, 50, 50), RSGL_STYLE_DARK, &checkbox);	 		
	
		RSGL_widgetPolygonPoints(100);
		RSGL_radioButtons(RSGL_RECTF(50, 320, 32, 32), 3, RSGL_STYLE_DARK | RSGL_SHAPE_POLYGON, &selected, &index); 
		
		RSGL_widgetRounding(RSGL_POINT(10, 10));
		RSGL_slider(RSGL_RECTF(160, 450, 260, 15), RSGL_STYLE_DARK | RSGL_STYLE_ROUND | RSGL_SLIDER_CIRCLE, &slider_value, &slider_grabbed);
		
		RSGL_widgetOutline(0, RSGL_RGB(255, 255, 255),  RSGL_RGB(255, 255, 255),  RSGL_RGB(255, 255, 255),  RSGL_RGB(255, 255, 255));
		RSGL_widgetAlign(RSGL_ALIGN_LEFT | RSGL_ALIGN_DOWN);

		RSGL_label(RSGL_strFmt("slider : %i", (i32)(slider_value * 100)), RSGL_RECTF(50, 450, 370, 15));
		
        RSGL_clear(RSGL_RGB(20, 20, 60));
		RGFW_window_swapBuffers(win);
    }

	RSGL_free();
    RGFW_window_close(win);
}
