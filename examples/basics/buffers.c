#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#define RGFW_INT_DEFINED
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "examples/deps/RGFW.h"

int main() {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* window = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, 500, 500));
	RSGL_renderer_updateSize(renderer, 500, 500);

	RSGL_renderBuffers buffers;
	RSGL_renderer_createRenderBuffers(renderer, 20, &buffers);
	RSGL_renderer_setRenderBuffers(renderer, &buffers);

	RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
	i32 batch = RSGL_drawRect(renderer, RSGL_RECT(100, 100, 20, 20));
	RSGL_renderer_updateRenderBuffers(renderer);
	while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
		RGFW_pollEvents();

		buffers.batches[batch].matrix = RSGL_mat4_translate(buffers.batches[batch].matrix.m, 1, 0, 0);

		RSGL_renderer_clear(renderer, RSGL_RGB(10, 50, 99));

		RSGL_renderer_setRenderBuffers(renderer, &buffers);
		RSGL_renderer_renderBuffers(renderer);

		RSGL_renderer_setRenderBuffers(renderer, NULL);
		RSGL_drawRect(renderer, RSGL_RECT(200, 200, 200, 200));
		RSGL_renderer_render(renderer);

		RGFW_window_swapBuffers_OpenGL(window);
	}

	RSGL_renderer_deleteRenderBuffers(renderer, &buffers);

	RSGL_renderer_free(renderer);
	RGFW_window_close(window);
}
