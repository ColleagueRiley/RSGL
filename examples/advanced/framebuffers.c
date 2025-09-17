#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_INT_DEFINED
#define RSGLDEF
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#include <stdio.h>

int main(void) {
    RGFW_window* win = RGFW_createWindow("RSGL Framebuffers", RGFW_RECT(0, 0, 500, 500), RGFW_windowCenter);
    RGFW_window_makeCurrent(win);

    RSGL_setClearArgs(true);
    RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress, RSGL_GL_renderer());

    RSGL_framebuffer fbo;
    fbo = RSGL_renderCreateFramebuffer(RSGL_AREA(300, 300));
    
    while (RGFW_window_shouldClose(win) == false) {
        while (RGFW_window_checkEvent(win))  {
            if (win->event.type == RGFW_quit) {
                break;
            }
        }

        RGFW_window_makeCurrent(win);
        RSGL_updateSize(RSGL_AREA(win->r.w, win->r.h));
        
        // framebuffer
        RSGL_setFramebuffer(fbo.id);
        RSGL_clear(RSGL_RGB(255, 0, 0));
        RSGL_drawRect(RSGL_RECT(200, 200, 200, 200), RSGL_RGB(0, 255, 0));
        RSGL_draw(); // commit the FBO

        // back to normal
        RSGL_setFramebuffer(0);
        RSGL_clear(RSGL_RGB(0, 0, 255));

        // render the FBO (flipped)
        RSGL_setTexture(fbo.texture);
        RSGL_drawRect(RSGL_RECT(0, 0, 250, 250), RSGL_RGB(255, 255, 255));
		
        RSGL_draw();
		RGFW_window_swapBuffers(win);
    }

    RSGL_renderDeleteFramebuffer(fbo);
	RSGL_free();

    RGFW_window_close(win);
    
    return 0;
}
