#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <time.h>

unsigned char icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};

int main(void) {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* win = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);


	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, 500, 500));
	RSGL_renderer_updateSize(renderer, RSGL_AREA(500, 500));

	RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);

    int w, h, c;
    u8* logo = stbi_load("logo.png", &w, &h, &c, 4);

	RSGL_textureBlob blob;
	blob.data = logo;
	blob.width = w;
	blob.height = h;
	blob.dataType = RSGL_textureDataInt;
	blob.dataFormat = (c == 4) ? RSGL_formatRGBA : RSGL_formatRGB;
	blob.textureFormat = blob.dataFormat;
    u32 texture = RSGL_renderer_createTexture(renderer, &blob);

    free(logo);

	RSGL_textureBlob blob2;
	blob2.data = icon;
	blob2.width = 3;
	blob2.height = 3;
	blob2.dataType = RSGL_textureDataInt;
	blob2.dataFormat = RSGL_formatRGBA;
	blob2.textureFormat = blob2.dataFormat;
    u32 iconTex = RSGL_renderer_createTexture(renderer, &blob2);

	while (RGFW_window_shouldClose(win) == false) {
		RGFW_pollEvents();

		RSGL_renderer_clear(renderer, RSGL_RGB(65, 65, 65));
        rotate.z += 1;

        RSGL_renderer_setTexture(renderer, texture);
		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
        RSGL_renderer_setRotate(renderer, rotate);
        RSGL_drawRect(renderer, RSGL_RECT(200, 200, 200, 200));


        RSGL_renderer_setRotate(renderer, rotate);

		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 255, 255));
        RSGL_renderer_setTexture(renderer, iconTex);
		RSGL_drawPolygon(renderer, RSGL_RECT(20, 20, 50, 50), 8);

        RSGL_renderer_setTexture(renderer, texture);
		RSGL_renderer_setRotate(renderer, rotate);
        RSGL_drawTriangle(renderer, RSGL_TRIANGLE(RSGL_POINT(0, 500), RSGL_POINT(200, 500), RSGL_POINT(100, 250)));

		RSGL_renderer_render(renderer);
        RGFW_window_swapBuffers_OpenGL(win);
    }

    RSGL_renderer_deleteTexture(renderer, texture);
    RSGL_renderer_deleteTexture(renderer, iconTex);
	RSGL_renderer_free(renderer);

	RGFW_window_close(win);
    return 0;
}
