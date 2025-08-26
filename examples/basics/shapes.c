#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#define RSGL_DEBUG
//#define RSGL_RENDER_LEGACY
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#include <stdio.h>

int main(void) {
    RGFW_window* win = RGFW_createWindow("RSGL shapes example", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), RSGL_AREA(500, 500), (void*)RGFW_getProcAddress_OpenGL);
    bool fill = true;

    RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);
	while (RGFW_window_shouldClose(win) == RGFW_FALSE) {
		RGFW_pollEvents();

        RSGL_renderer_clear(renderer, RSGL_RGB(65, 65, 65));

        rotate.z += 1;

        if ((rand() % 200) <= 5)
            fill = !fill;

        RSGL_renderer_setRotate(renderer, rotate);
		RSGL_renderer_setColor(renderer, RSGL_RGB(0, 255, 0));
        RSGL_drawPolygon(renderer, RSGL_RECT(20, 20, 50, 50), 8);

        RSGL_renderer_setRotate(renderer, rotate);
		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
        RSGL_drawRect(renderer, RSGL_RECT(200, 200, 200, 200));

        RSGL_renderer_setRotate(renderer, rotate);
		RSGL_renderer_setColor(renderer, RSGL_RGB(0, 0, 255));
        RSGL_drawTriangle(renderer, RSGL_TRIANGLE(RSGL_POINT(0, 500), RSGL_POINT(200, 500), RSGL_POINT(100, 250)));

        RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(win);
	}

	RSGL_renderer_free(renderer);
    RGFW_window_close(win);
}
