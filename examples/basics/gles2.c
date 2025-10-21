#define RSGL_DEBUG
#define RSGL_GLES2
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#define RGFW_INT_DEFINED
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "examples/deps/RGFW.h"

int main() {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 2;
	hints->minor = 0;
	hints->profile = RGFW_glES;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* window = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);

    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, 500, 500));
	RSGL_renderer_updateSize(renderer, 500, 500);

	while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
		RGFW_pollEvents();

		RSGL_renderer_clear(renderer, RSGL_RGB(10, 50, 100));

		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
		RSGL_drawRect(renderer, RSGL_RECT(200, 200, 200, 200));
		RSGL_renderer_render(renderer);

		RGFW_window_swapBuffers_OpenGL(window);
	}

	RSGL_renderer_free(renderer);
	RGFW_window_close(window);
}
