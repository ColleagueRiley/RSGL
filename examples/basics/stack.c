#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "examples/deps/RGFW.h"

int main() {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window window;
	RGFW_createWindowPtr("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL, &window);

	RSGL_glRenderer ptr;
	RSGL_renderer renderer;
	RSGL_renderer_initPtr(RSGL_GL_rendererProc(), RSGL_AREA(500, 500), (void*)RGFW_getProcAddress_OpenGL, &ptr, &renderer);


	while (RGFW_window_shouldClose(&window) == RGFW_FALSE) {
		RGFW_pollEvents();

		RSGL_renderer_clear(&renderer, RSGL_RGB(255, 255, 255));

		RSGL_renderer_setColor(&renderer, RSGL_RGB(255, 0, 0));
		RSGL_drawRect(&renderer, RSGL_RECT(200, 200, 200, 200));
		RSGL_renderer_render(&renderer);

		RGFW_window_swapBuffers_OpenGL(&window);
	}

	RSGL_renderer_freePtr(&renderer);
	RGFW_window_closePtr(&window);
}
