#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"
#include "RSGL_gl1.h"

#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "examples/deps/RGFW.h"

int main() {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* window = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer_opengl11 = RSGL_renderer_init(RSGL_GL1_rendererProc(), RSGL_AREA(500, 500), (void*)RGFW_getProcAddress_OpenGL);
	RSGL_renderer* renderer_opengl = RSGL_renderer_init(RSGL_GL_rendererProc(), RSGL_AREA(500, 500), (void*)RGFW_getProcAddress_OpenGL);

	RSGL_renderer* renderer = renderer_opengl;

	while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
		RGFW_pollEvents();

		if (RGFW_isKeyPressed(RGFW_space))
			renderer = (renderer == renderer_opengl) ? renderer_opengl11 : renderer_opengl;

		RSGL_renderer_clear(renderer, RSGL_RGB(10, 50, 100));

		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
		RSGL_drawRect(renderer, RSGL_RECT(200, 200, 200, 200));
		RSGL_renderer_render(renderer);

		RGFW_window_swapBuffers_OpenGL(window);
	}

	RSGL_renderer_free(renderer);
	RGFW_window_close(window);
}
