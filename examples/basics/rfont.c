#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_RFONT
#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#define RFONT_IMPLEMENTATION
#include "RFont.h"

int main(void) {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* win = RGFW_createWindow("window", 0, 0, 800, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), RSGL_AREA(500, 500), (void*)RGFW_getProcAddress_OpenGL);

	RFont_renderer* renderer_rfont = RFont_RSGL_renderer_init(renderer);
    RFont_font* font = RFont_font_init(renderer_rfont, "COMICSANS.ttf", 60, 500, 500);

	i32 w, h;

    for (; RGFW_window_shouldClose(win) == false;) {
		RGFW_pollEvents();
		RGFW_window_getSize(win, &w, &h);

		RSGL_renderer_clear(renderer, RSGL_RGB(125, 25, 75));

		RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, w, h));
		RFont_renderer_set_framebuffer(renderer_rfont, (u32)w, (u32)h);

		RFont_renderer_set_color(renderer_rfont, 1.0f, 1.0f, 1.0f, 1.0f);
		RFont_draw_text(renderer_rfont, font, "Something about a fast lazy hedgehog.", 0, 10, 60);

		RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(win);
    }

    RFont_font_free(renderer_rfont, font);
	RFont_RSGL_renderer_free(renderer_rfont);

	RSGL_renderer_free(renderer);
	RGFW_window_close(win);
}
