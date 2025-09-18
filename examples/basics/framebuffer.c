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

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, 500, 500));
	RSGL_renderer_updateSize(renderer, 500, 500);


	RSGL_textureBlob blob;
	blob.data = NULL;
	blob.width = 300;
	blob.height = 300;
	blob.dataType = RSGL_textureDataInt;
	blob.dataFormat = RSGL_formatRGBA;
	blob.textureFormat = blob.dataFormat;
    u32 tex = RSGL_renderer_createTexture(renderer, &blob);

	RSGL_framebuffer framebuffer = RSGL_renderer_createFramebuffer(renderer, 300, 300);
	RSGL_renderer_attachFramebuffer(renderer, framebuffer, tex, 0, 0);

	while (RGFW_window_shouldClose(win) == RGFW_FALSE) {
		RGFW_pollEvents();

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        RSGL_renderer_clear(renderer, RSGL_RGB(255, 255, 255));
		RSGL_renderer_setColor(renderer, RSGL_RGB(0, 255, 0));
		RSGL_drawRect(renderer, RSGL_RECT(20, 300, 40, 40));
        RSGL_renderer_render(renderer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		RSGL_renderer_clear(renderer, RSGL_RGB(0, 0, 255));
		RSGL_renderer_setTexture(renderer, tex);
		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 255, 255));
		RSGL_drawRect(renderer, RSGL_RECT(200, 200, 250, 250));

        RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(win);
	}

	RSGL_renderer_deleteFramebuffer(renderer, framebuffer);
	RSGL_renderer_deleteTexture(renderer, tex);

	RSGL_renderer_free(renderer);
    RGFW_window_close(win);
}
