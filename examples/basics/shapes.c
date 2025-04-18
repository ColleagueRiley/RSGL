#define RGFW_IMPLEMENTATION
#include "RGFW.h"


#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#include <stdio.h>

int main(void) {
    RGFW_window* win = RGFW_createWindow("RSGL shapes example", (RGFW_rect){0, 0, 500, 500}, RGFW_windowCenter);

	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress, RSGL_GL_renderer());

    bool fill = true;

    RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);

    u32 frameCount = 0;
    float startTime = RGFW_getTime();

    while (RGFW_window_shouldClose(win) == false) {
        while (RGFW_window_checkEvent(win)) {
			if (win->event.type == RGFW_quit) {
                break;
            }
		}

        RSGL_clear(RSGL_RGB(65, 65, 65));
        
        rotate.z += 1;

        if ((rand() % 200) <= 5) 
            fill = !fill;

        RSGL_setRotate(rotate);
        RSGL_drawPolygon(RSGL_RECT(20, 20, 50, 50), 8, RSGL_RGB(0, 255, 0));

        RSGL_setRotate(rotate);
        RSGL_drawRect(RSGL_RECT(200, 200, 200, 200), RSGL_RGB(255, 0, 0));

        RSGL_setRotate(rotate);
        RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(0, 500), RSGL_POINT(200, 500), RSGL_POINT(100, 250)), RSGL_RGB(0, 0, 255));

        RSGL_draw();
		RGFW_window_swapBuffers(win);

		RGFW_checkFPS(startTime, frameCount, 60);
        frameCount++;
	}
	
	RSGL_free();
    RGFW_window_close(win);
    return 0;
}
