#define RGFW_BUFFER
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_INT_DEFINED
#define RSoft_area RSGL_area
#include "RSGL.h"

#define RSGL_IMPLEMENTATION
#include "RSGL_rsoft.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


unsigned char icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};

int main(void) {
    RGFW_window* win = RGFW_createWindow("RSGL shapes example", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	u8* buffer = (u8*)RSGL_MALLOC(win->w * win->h * 4);
	RGFW_surface* surface = RGFW_createSurface(buffer, 500, 500, RGFW_formatRGBA8);

	RSGL_renderer renderer = RSGL_RSoft_renderer();
	RSGL_renderer_init(&renderer, RSGL_AREA(500, 500), buffer);

    RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);

    int w, h, c;
    u8* logo = stbi_load("logo.png", &w, &h, &c, 4);
    u32 texture = RSGL_renderer_createTexture(&renderer, logo, RSGL_AREA(w, h), c);
    free(logo);

    RSGL_texture iconTex = RSGL_renderer_createTexture(&renderer, icon, RSGL_AREA(3, 3), 4);

    RSGL_font* comicSans = RSGL_loadFont(&renderer, "COMICSANS.ttf");
    RSGL_renderer_setFont(&renderer, comicSans);

    size_t texTimer = (size_t)time(0);
    size_t texTimer2 = 0;

    while (RGFW_window_shouldClose(win) == false) {
		RGFW_pollEvents();

        RSGL_renderer_clear(&renderer, RSGL_RGBA(65, 65, 65, 255));
        rotate.z += 1;

        RSGL_renderer_setRotate(&renderer, rotate);
        //RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(0, 500), RSGL_POINT(200, 500), RSGL_POINT(100, 250)), RSGL_RGB(255, 255, 255));

        RSGL_renderer_setTexture(&renderer, texture);
        RSGL_drawRect(&renderer, RSGL_RECT(200, 200, 200, 200), RSGL_RGB(0, 255, 0));
		//RSGL_drawPolygon(RSGL_RECT(200, 200, 200, 200), 360, RSGL_RGB(0, 255, 0));

        //RSGL_drawText("Hello, RSOFT!", RSGL_CIRCLE(100, 20, 20), RSGL_RGB(255, 255, 255));

        RSGL_renderer_render(&renderer);
        RGFW_window_blitSurface(win, surface);
	}

    RSGL_renderer_deleteTexture(&renderer, texture);
    RSGL_renderer_deleteTexture(&renderer, iconTex);
	RSGL_renderer_free(&renderer);

	RGFW_window_close(win);
    return 0;
}
