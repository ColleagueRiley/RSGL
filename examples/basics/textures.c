#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_IMPLEMENTATION

#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#include "RSGL.h"
#include <stdio.h>

unsigned char icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};

int main(void) {
    RGFW_window* win = RGFW_createWindow("RSGL texture example", (RGFW_rect){0, 0, 500, 500}, RGFW_ALLOW_DND | RGFW_CENTER);
		
	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

    RGFW_window_setIcon(win, icon, RGFW_AREA(3, 3), 4);

    RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);

    u32 texture = RSGL_loadImage("logo.png").tex;
    u32 iconTex = RSGL_renderCreateTexture(icon, RSGL_AREA(3, 3), 4);

    size_t texTimer = (size_t)time(0);
    size_t texTimer2 = 0;

    while (RGFW_window_shouldClose(win) == false) {
        while (RGFW_window_checkEvent(win))
            if (win->event.type == RGFW_quit) {
                break;
            }
        
        rotate.z += 1;

        RSGL_rotate(rotate);
        RSGL_drawRect(RSGL_RECT(200, 200, 200, 200), RSGL_RGB(255, 0, 0));

        RSGL_setTexture(iconTex);
        RSGL_rotate(rotate);
        RSGL_drawPolygon(RSGL_RECT(20, 20, 50, 50), 8, RSGL_RGB(255, 255, 255));
        
        RSGL_setTexture(texture);
        
        if (texTimer2 && texTimer2 + 5 > (size_t)time(0))
            RSGL_setTexture(0);
        else if (texTimer2 && texTimer2 + 5 <= (size_t)time(0)) {
            texTimer = (size_t)time(0);
            texTimer2 = 0;
        }

        else if (texTimer2 == 0 && texTimer + 5 <= (size_t)time(0)) {
            texTimer2 = (size_t)time(0);

            RSGL_setTexture(0);
        }

        RSGL_rotate(rotate);
        RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(0, 500), RSGL_POINT(200, 500), RSGL_POINT(100, 250)), RSGL_RGB(255, 255, 255));

        RSGL_clear(RSGL_RGB(65, 65, 65));
		RGFW_window_swapBuffers(win);

		RGFW_window_checkFPS(win, 60);
	}
	
    RSGL_renderDeleteTexture(iconTex);
	RSGL_free();

	RGFW_window_close(win);
    return 0;
}
