#define RGFW_ALLOC_DROPFILES
#define RSGL_IMPLEMENTATION
#define RSGL_NO_X11_CURSOR

#include "../RSGL.h"
#include <stdio.h>

void drawLoop(RSGL_window* w); /* I seperate the draw loop only because it's run twice */

static RSGL_window* win2;
void* loop2(void *);

unsigned char icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};
unsigned char running = 1;

RSGL_color gradient[3] = {RSGL_RGB(255, 0, 0), RSGL_RGB(0, 255, 0), RSGL_RGB(0, 0, 255)};

int main(void) {
    RSGL_window* win = RSGL_createWindow("RSGL Example Window", RSGL_RECT(500, 500, 500, 500), RSGL_ALLOW_DND | RSGL_CENTER);
    RSGL_window_makeCurrent(win);
    
    if (win == NULL)
        return 1;
    
    win->fpsCap = 60;

    win2 = RSGL_createWindow("subwindow", RSGL_RECT(200, 200, 200, 200), 0);

    /*unsigned short js = RSGL_registerJoystick(win, 0);*/
    unsigned char i;

    RSGL_window_setIcon(win, icon, RGFW_AREA(3, 3), 4);

    RSGL_window_swapInterval(win, 60);

    RSGL_setClearArgs(true);

    while (running && !RSGL_isPressedI(win, RGFW_Escape)) {

        /* 
            check all of the avaliable events all at once
            this is to avoid any input lag

            this isn't required, RSGL_checkEvents can be used without a loop

            but not using this method could cause input lag
        */

        RSGL_window_checkEvent(win2);
        if (win2->event.type == RGFW_quit)
            running = 0;

        while (RSGL_window_checkEvent(win))  {
            if (win->event.type == RSGL_windowAttribsChange) {
                printf("attribs changed\n");
            }
            if (win->event.type == RSGL_quit) {
                running = 0;  
                break;
            }
            if (RSGL_isPressedI(win, RGFW_Up))
                printf("Pasted : %s\n", RSGL_readClipboard(NULL));
            else if (RSGL_isPressedI(win, RGFW_Down))
                RSGL_writeClipboard("DOWN", 4);
            else if (RSGL_isPressedI(win, RGFW_Space))
                printf("fps : %i\n", win->event.fps);
            else if (RSGL_isPressedI(win, RGFW_w))
                RSGL_window_setMouseDefault(win);
            else if (RSGL_isPressedI(win, RGFW_q))
                RSGL_window_showMouse(win, 0);
            else if (RSGL_isPressedI(win, RGFW_t)) {
                RSGL_window_setMouse(win, icon, RSGL_AREA(3, 3), 4);
            }
            if (win->event.type == RSGL_dnd) {
                for (i = 0; i < win->event.droppedFilesCount; i++)
                    printf("dropped : %s\n", win->event.droppedFiles[i]);
            }

            else if (win->event.type == RSGL_jsButtonPressed)
                printf("pressed %i\n", win->event.button);

            else if (win->event.type == RSGL_jsAxisMove && !win->event.button)
                printf("{%i, %i}\n", win->event.axis[0].x, win->event.axis[0].y);
        }

        RSGL_window_makeCurrent(win);

        RSGL_setGradient(gradient, 3);
        RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(20, win->r.h - 20), RSGL_POINT(win->r.w - 20,win->r.h - 20), RSGL_POINT((win->r.w - 40) / 2, 20)), RSGL_RGB(255, 255, 0));

        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));    

        {                
            float points[] = {
                                        RSGL_GET_WORLD_POINT(20, win->r.h - 20, 0.0f), 
                                        RSGL_GET_WORLD_POINT(win->r.w - 20, win->r.h - 20,  0.0f), 
                                        RSGL_GET_WORLD_POINT((win->r.w - 40) / 2, 20, 0.0f)
                                      };
            
            float texPoints[] = {   
                        0.0f, 1.0f, 
                        1.0f, 1.0f,
                        ((float)(points[6] - points[0])/points[3]< 1) ? (float)(points[6] - points[0]) / points[3] : 0, 0.0f,
            };

            RSGL_setGradient(gradient, 3);

            RSGL_point3DF center = {points[(3 * 2)], (points[(3 * 2) + 1] + points[1]) / 2.0f, 0};
            RSGL_basicDraw(RSGL_TRIANGLES_2D, (float*)points, (float*)texPoints, center, RSGL_RGB(255, 255, 0), 3);



            if (running == false)
                break;
            
            RSGL_window_clear(win2, RSGL_RGB(255, 255, 255));
        }
    }

    RSGL_window_close(win2);
    RSGL_window_close(win);
}