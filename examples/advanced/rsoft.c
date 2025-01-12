#define RGFW_IMPLEMENTATION
#define RGFW_BUFFER
#include "RGFW.h"

#define RSoft_area RGFW_area
#include "RSGL_rsoft.h"
#include <stdio.h>

unsigned char icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};

int main(void) {
    RGFW_window* win = RGFW_createWindow("RSGL texture example", (RGFW_rect){0, 0, 500, 500}, RGFW_windowCenter);
	

    RSoft_setBufferSize(RGFW_getScreenSize());
	RSGL_init(RSGL_AREA(win->r.w, win->r.h), win->buffer);

    RGFW_window_setIcon(win, icon, RGFW_AREA(3, 3), 4);

    RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);

    RSGL_texture texture = RSGL_loadImage("logo.png").tex;
    RSGL_texture iconTex = RSGL_renderCreateTexture(icon, RSGL_AREA(3, 3), 4);

    u32 comicSans = RSGL_loadFont("COMICSANS.ttf");
    RSGL_setFont(comicSans);

    size_t texTimer = (size_t)time(0);
    size_t texTimer2 = 0;

    while (RGFW_window_shouldClose(win) == false) {
        while (RGFW_window_checkEvent(win))
            if (win->event.type == RGFW_quit) {
                break;
            }
        
        RSGL_clear(RSGL_RGB(65, 65, 65));
        rotate.z += 1;
        
        RSGL_rotate(rotate);
        //RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(0, 500), RSGL_POINT(200, 500), RSGL_POINT(100, 250)), RSGL_RGB(255, 255, 255));
        
        RSGL_setTexture(texture);
        RSGL_drawRect(RSGL_RECT(200, 200, 200, 200), RSGL_RGB(0, 255, 0));
		//RSGL_drawPolygon(RSGL_RECT(200, 200, 200, 200), 360, RSGL_RGB(0, 255, 0));

        //RSGL_drawText("Hello, RSOFT!", RSGL_CIRCLE(100, 20, 20), RSGL_RGB(255, 255, 255));
        
        RSGL_draw();
		RGFW_window_swapBuffers(win);

		RGFW_window_checkFPS(win, 60);
	}
	
    RSGL_renderDeleteTexture(iconTex);
	RSGL_free();

	RGFW_window_close(win);
    return 0;
}
