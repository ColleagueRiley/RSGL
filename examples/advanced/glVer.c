#define RGL_LEGACY
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main(void) {
	RGFW_setGLVersion(RGFW_GL_COMPATIBILITY, 3, 3);
	RGFW_window* win = RGFW_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RGFW_CENTER);

	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

    RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));
	
	b8 legacy = false;
    
	RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);

    bool running = true;

	u32 fps = 0;

    while (running) {
		RSGL_legacy(!legacy);
		
		while (RSGL_checkEvent(win)) {
            if (win->event.type == RGFW_quit) {
                running = false;
                break;
            }
        }
		
		RSGL_openBlankContainer(RSGL_RECT(0, 0, win->r.w, win->r.h));
	
        RSGL_rotate(RSGL_POINT3D(0, 0, 0));

        RSGL_drawText(RSGL_strFmt("FPS : %i\nOpenGL %s", fps, !legacy ? "legacy (2-)" : "modern (3.3 +)"), RSGL_CIRCLE(0, 40, 40), RSGL_RGB(255, 0, 0));
		RSGL_toggleButton(RSGL_RECTF(50, 125, 100, 50), RSGL_STYLE_LIGHT | RSGL_STYLE_ROUND, &legacy);
        
		rotate.z++;
        RSGL_rotate(rotate);
		RSGL_drawRect(RSGL_RECT(200, 200, 200, 200), RSGL_RGB(255, 0, 0));
		
	
		RSGL_clear(RSGL_RGB(255, 255, 255));
		RGFW_window_swapBuffers(win);

		fps = RGFW_window_checkFPS(win, 60);
	}
	
	RSGL_free();
    RGFW_window_close(win);
}
