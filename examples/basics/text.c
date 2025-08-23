#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

int main(void) {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* win = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), RSGL_AREA(500, 500), RGFW_getProcAddress_OpenGL);

	RSGL_font* font = RSGL_loadFont(renderer, "Super Easy.ttf", 20, 500, 500);

    RSGL_renderer_setFont(renderer, font);

    for (; RGFW_window_shouldClose(win) == false;) {
		RGFW_pollEvents();

		RSGL_renderer_clear(renderer, RSGL_RGB(255, 255, 255));

		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
        RSGL_drawText(renderer, "Text example\nnewlines too", RSGL_CIRCLE(200, 200, 20));

        RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(win);
    }

	RSGL_renderer_free(renderer);
	RGFW_window_close(win);
}
