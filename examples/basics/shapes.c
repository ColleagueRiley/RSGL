#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#define RSGL_GL_USE_GLAD
#define RSGL_DEBUG
//#define RSGL_RENDER_LEGACY
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#include <stdio.h>


int main(void) {
    RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
    hints->major = 3;
    hints->minor = 3;
    RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* win = RGFW_createWindow("RSGL shapes example", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);

    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, 500, 500));
	RSGL_renderer_updateSize(renderer, 500, 500);
	bool fill = true;
    RSGL_vec3D rotate = RSGL_VEC3D(0, 0, 0);
	while (RGFW_window_shouldClose(win) == RGFW_FALSE) {
		RGFW_pollEvents();

        RSGL_renderer_clear(renderer, RSGL_RGB(65, 65, 65));

        rotate.z += 1;

        if ((rand() % 200) <= 5)
            fill = !fill;

		i32 w, h;
		RGFW_window_getSize(win, &w, &h);
		RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, w, h));
		RSGL_renderer_updateSize(renderer, (u32)w, (u32)h);

		RSGL_renderer_setRotate(renderer, rotate);
		RSGL_renderer_setColor(renderer, RSGL_RGB(0, 255, 0));
		RSGL_drawPolygon(renderer, RSGL_RECT(20, 20, 50, 50), 8);

		RSGL_renderer_setRotate(renderer, rotate);
		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
		RSGL_drawRect(renderer, RSGL_RECT(200, 200, 200, 200));

		RSGL_renderer_setRotate(renderer, rotate);
		RSGL_renderer_setColor(renderer, RSGL_RGB(0, 0, 255));

		RSGL_vec3D points[3] = {RSGL_VEC3D(0, 500, 0), RSGL_VEC3D(200, 500, 0), RSGL_VEC3D(100, 250, 0)};
		RSGL_drawTriangle(renderer, points);

        RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(win);
	}

	RSGL_renderer_free(renderer);
    RGFW_window_close(win);
}
