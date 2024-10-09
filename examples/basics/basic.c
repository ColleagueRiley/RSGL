#define RGFW_ALLOC_DROPFILES
#define RSGL_IMPLEMENTATION
#define RSGL_NO_X11_CURSOR

#include "RSGL.h"
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
    RGFW_window* win = RGFW_createWindow("RSGL Example Window", RSGL_RECT(500, 500, 500, 500), RGFW_ALLOW_DND | RGFW_CENTER);
    RGFW_window_makeCurrent(win);
    
    if (win == NULL)
        return 1;
    

    #ifndef __EMSCRIPTEN__
    win2 = RGFW_createWindow("subwindow", RSGL_RECT(200, 200, 200, 200), 0);
    #endif

    /*unsigned short js = RSGL_registerJoystick(win, 0);*/
    unsigned char i;

    RGFW_window_setIcon(win, icon, RGFW_AREA(3, 3), 4);

    RSGL_setClearArgs(true);

	u32 fps = 0;

    while (RGFW_window_shouldClose(win) == false) {

        /* 
            check all of the avaliable events all at once
            this is to avoid any input lag

            this isn't required, RSGL_checkEvents can be used without a loop

            but not using this method could cause input lag
        */
       
        #ifndef __EMSCRIPTEN__
        RSGL_checkEvent(win2);
        if (win2->event.type == RGFW_quit)
            running = 0;
        #endif

		RSGL_init(RGFW_AREA(win->r.w, win->r.h), RGFW_getProcAddress);	

        while (RSGL_checkEvent(win))  {
			RSGL_updateSize(RGFW_AREA(win->r.w, win->r.h));
			if (win->event.type == RGFW_windowResized) {
                printf("window resized");
            }
            else if (win->event.type == RGFW_windowMoved) {
                printf("window moved");
            }
            else if (win->event.type == RGFW_quit) {
                running = 0;  
                break;
            }
            if (RGFW_isPressed(win, RGFW_Up))
                printf("Pasted : %s\n", RGFW_readClipboard(NULL));
            else if (RGFW_isPressed(win, RGFW_Down))
                RGFW_writeClipboard("DOWN", 4);
            else if (RGFW_isPressed(win, RGFW_Space))
                printf("fps : %i\n", fps);
            else if (RGFW_isPressed(win, RGFW_w))
                RGFW_window_setMouseDefault(win);
            else if (RGFW_isPressed(win, RGFW_q))
                RGFW_window_showMouse(win, 0);
            else if (RGFW_isPressed(win, RGFW_t)) {
                RGFW_window_setMouse(win, icon, RSGL_AREA(3, 3), 4);
            }
            if (win->event.type == RGFW_dnd) {
                for (i = 0; i < win->event.droppedFilesCount; i++)
                    printf("dropped : %s\n", win->event.droppedFiles[i]);
            }

            else if (win->event.type == RGFW_jsButtonPressed)
                printf("pressed %i\n", win->event.button);

            else if (win->event.type == RGFW_jsAxisMove && !win->event.button)
                printf("{%i, %i}\n", win->event.axis[0].x, win->event.axis[0].y);
        }
	
        RGFW_window_makeCurrent(win);
	
        RSGL_updateSize(RGFW_AREA(win->r.w, win->r.h));
        
		RSGL_setGradient(gradient, 3);
        RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(20, win->r.h - 20), RSGL_POINT(win->r.w - 20,win->r.h - 20), RSGL_POINT((win->r.w - 40) / 2, 20)), RSGL_RGB(255, 255, 0));
		
        RSGL_clear(RSGL_RGB(255, 255, 255));    
		RGFW_window_swapBuffers(win);

        {
			RGFW_window_makeCurrent(win2);
			RSGL_updateSize(RSGL_AREA(win2->r.w, win2->r.h));
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

            RSGL_basicDraw(RSGL_TRIANGLES_2D, (float*)points, (float*)texPoints, RSGL_RGB(255, 255, 0), 3);


            if (running == false)
                break;
            
            #ifndef __EMSCRIPTEN__
			RSGL_clear(RSGL_RGB(255, 255, 255));
			RGFW_window_swapBuffers(win2);
			#endif
			fps = RGFW_window_checkFPS(win, 60);
        }
    }

	RSGL_free();

    #ifndef __EMSCRIPTEN__
	RGFW_window_close(win2);
    #endif

    RGFW_window_close(win);
}
