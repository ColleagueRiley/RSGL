#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGLDEF
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#include <stdio.h>

void drawLoop(RGFW_window* w); /* I seperate the draw loop only because it's run twice */

#ifndef __EMSCRIPTEN__
static RGFW_window* win2;
#endif

void* loop2(void *);

unsigned char icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};
unsigned char running = 1;

float gradient[3 * 4] = {1, 0, 0, 1,      0, 1, 0, 1,       0, 0, 1, 1};

int main(void) {
    RGFW_window* win = RGFW_createWindow("RSGL Example Window", RGFW_RECT(500, 500, 500, 500), RGFW_windowAllowDND | RGFW_windowCenter);
    RGFW_window_makeCurrent(win);
    
    if (win == NULL)
        return 1;
    

    #ifndef __EMSCRIPTEN__
    win2 = RGFW_createWindow("subwindow", RGFW_RECT(200, 200, 200, 200), 0);
    #endif

    RSGL_setClearArgs(true);

    RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);	
    
    while (RGFW_window_shouldClose(win) == false) {

        /* 
            check all of the avaliable events all at once
            this is to avoid any input lag

            this isn't required, RSGL_checkEvents can be used without a loop

            but not using this method could cause input lag
        */
       
        #ifndef __EMSCRIPTEN__
        RGFW_window_checkEvent(win2);
        if (win2->event.type == RGFW_quit)
            running = 0;
        #endif
        
        while (RGFW_window_checkEvent(win))  {
            if (win->event.type == RGFW_quit) {
                running = 0;
                break;
            }
        }

        RGFW_window_makeCurrent(win);

        RSGL_updateSize(RSGL_AREA(win->r.w, win->r.h));
        
        RSGL_clear(RSGL_RGB(255, 255, 255));    
		RSGL_setGradient(gradient, 3);
        RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(20, win->r.h - 20), RSGL_POINT(win->r.w - 20,win->r.h - 20), RSGL_POINT((win->r.w - 40) / 2, 20)), RSGL_RGB(255, 255, 0));
		
        RSGL_draw();
		RGFW_window_swapBuffers(win);
        
        #ifndef __EMSCRIPTEN__
        {
			RGFW_window_makeCurrent(win2);
			RSGL_updateSize(RSGL_AREA(win2->r.w, win2->r.h));
			RSGL_clear(RSGL_RGB(255, 255, 255));
            
            float points[] = {
                                        RSGL_GET_WORLD_POINT(20, win2->r.h - 20, 0.0f), 
                                        RSGL_GET_WORLD_POINT(win2->r.w - 20, win2->r.h - 20,  0.0f), 
                                        RSGL_GET_WORLD_POINT((win2->r.w - 20) / 2, 20, 0.0f)
                                      };
            
            float texPoints[] = {   
                        0.0f, 1.0f, 
                        1.0f, 1.0f,
                        ((float)(points[6] - points[0])/points[3]< 1) ? (float)(points[6] - points[0]) / points[3] : 0, 0.0f,
            };

            RSGL_setGradient(gradient, 3);

            RSGL_basicDraw(RSGL_TRIANGLES, (float*)points, (float*)texPoints, RSGL_RGB(255, 255, 0), 3);


            if (running == false)
                break;

            RSGL_draw();
			RGFW_window_swapBuffers(win2);
        }
        #endif
    }

	RSGL_free();

    #ifndef __EMSCRIPTEN__
	RGFW_window_close(win2);
    #endif

    RGFW_window_close(win);
    
    return 0;
}
