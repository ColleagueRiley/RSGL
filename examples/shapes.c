#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#define RSGL_NO_AUDIO
#define RSGLDEF 
#include "RSGL.h"
#include <stdio.h>

int main(void) {
    RSGL_window* win = RSGL_createWindow("RSGL event example", (RSGL_rect){0, 0, 500, 500}, RGFW_ALLOW_DND | RGFW_CENTER);

    bool running = true;

    bool fill = true;

    RSGL_point3D rotate;

    while (running) {
        while (RGFW_window_checkEvent(win))
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }
        
        rotate.x = 180;
        //rotate.x += (rand() % 3) - 1;
        rotate.y += (rand() % 4) - 1;
        rotate.z += (rand() % 4) - 1;

        //if ((rand() % 100) <= 5) 
        //    fill = !fill;

        if (fill) {
            RSGL_rotate(rotate);
            RSGL_drawRect(RSGL_RECT(200, 200, 200, 200), RSGL_RGB(255, 0, 0));

            RSGL_rotate(rotate);
            RSGL_drawPolygon(RSGL_RECT(20, 20, 50, 50), 8, RSGL_RGB(0, 255, 0));

            RSGL_rotate(rotate);
            RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(0, 500), RSGL_POINT(200, 500), RSGL_POINT(100, 250)), RSGL_RGB(0, 0, 255));
        }
        else {
            RSGL_rotate(rotate);
            RSGL_drawRectOutline(RSGL_RECT(200, 200, 200, 200), 5, RSGL_RGB(255, 0, 0));

            RSGL_rotate(rotate);
            RSGL_drawPolygonOutline(RSGL_RECT(20, 20, 50, 50), 8, 5, RSGL_RGB(0, 255, 0));

            RSGL_rotate(rotate);
            RSGL_drawTriangleOutline(RSGL_TRIANGLE(RSGL_POINT(0, 500), RSGL_POINT(200, 500), RSGL_POINT(100, 250)), 5, RSGL_RGB(0, 0, 255));
        }

        RSGL_window_clear(win, RSGL_RGB(65, 65, 65));
    }

    return 0;
}