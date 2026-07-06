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
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#include <stdio.h>

int main(void) {
	i32 width = 500;
	i32 height = 500;

    RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
    hints->major = 3;
    hints->minor = 3;
    RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* win = RGFW_createWindow("RSGL shapes example", 0, 0, width, height, RGFW_windowCenter | RGFW_windowOpenGL);
	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);

    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, width, height));
	RSGL_renderer_updateSize(renderer, width, height);

	while (RGFW_window_shouldClose(win) == RGFW_FALSE) {
		RGFW_pollEvents();

        RSGL_renderer_clear(renderer, RSGL_RGB(65, 65, 65));

		i32 w, h;
		RGFW_window_getSize(win, &w, &h);

		if (w != width || h != height) {
			width = w;
			height = h;

			RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, width, height));
			RSGL_renderer_updateSize(renderer, width, height);
		}

		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
		RSGL_drawRect(renderer, RSGL_RECT(200, 200, 200, 200));

        RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(win);
	}

	RSGL_renderer_free(renderer);
    RGFW_window_close(win);
}
