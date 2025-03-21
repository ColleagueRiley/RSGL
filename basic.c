#define RGFW_IMPLEMENTATION
#include "RGFW.h"



#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

int main() {
    // create window with your API choice here
    
    RGFW_window* win = RGFW_createWindow("window", RGFW_RECT(0, 0, 500, 500), RGFW_windowCenter);
    u32 WIDTH = win->r.w; //??? window width
    u32 HEIGHT = win->r.h;//??? window height
   
    void* procAddress = (void*)RGFW_getProcAddress; //??? renderer proc address (ex. RGFW_getProcAddress/glfwGetProcAddress)

    RSGL_init(RSGL_AREA(WIDTH, HEIGHT), (void*)procAddress);	
    
    bool running = true;
    while (RGFW_window_shouldClose(win) == RGFW_FALSE) {
      // (handle your API's events here)
      while (RGFW_window_checkEvent(win));

      RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
      RSGL_clear(RSGL_RGB(255, 255, 255));
      RSGL_draw();

      // swap draw buffer with API here 
      RGFW_window_swapBuffers(win);
    }

    RSGL_free();
    RGFW_window_close(win);
}

