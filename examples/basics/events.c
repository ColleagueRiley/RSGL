#define RSGL_IMPLEMENTATION

#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#include "RSGL.h"
#include <stdio.h>

int main(void) {
    RSGL_window* win = RSGL_createWindow("RSGL event example", (RSGL_rect){0, 200, 800, 500}, RSGL_ALLOW_DND | RSGL_CENTER);

    bool running = true;
    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RGFW_quit) {
                running = false;
                break;
            }

            switch (win->event.type) {
                case RSGL_keyPressed:
                    printf("key pressed %i %s\n", win->event.keyCode, win->event.keyName);
                    break;
                case RSGL_keyReleased:
                    printf("key released %i %s\n", win->event.keyCode, win->event.keyName);
                    break;
                case RSGL_mouseButtonPressed:
                    printf("mouse button pressed %i\n", win->event.button);
                    break;
                case RSGL_mouseButtonReleased:
                    printf("mouse button released %i\n", win->event.button);
                    break;
                case RSGL_mousePosChanged:
                    if (RSGL_isPressed(win, RGFW_ShiftL))
                        printf("mouse moved %i %i\n", win->event.point.x, win->event.point.y);
                    break;
                case RGFW_jsButtonPressed:
                    printf("joystick pressed %i\n", win->event.joystick);
                    break;
                case RGFW_jsButtonReleased:
                    printf("joystick released %i\n", win->event.joystick);
                    break;
                case RGFW_jsAxisMove:
                    printf("axis moved %i %i\n", win->event.axis[0].x, win->event.axis[0].y);
                    break;
                case RGFW_dnd:
                    printf("dropped file : %i %i\n", win->event.point.x, win->event.point.y);
                    size_t i;
                    for (i = 0; i < win->event.droppedFilesCount; i++)
                        printf("%s\n", win->event.droppedFiles[i]);
                    printf("\n");
                    break;
                case RGFW_windowMoved:
                    printf("window moved\n");
                    break;
                case RGFW_windowResized:
                    printf("window resized\n");
                    break;
                default: break;
            }
        }
        
        RSGL_window_clear(win, RSGL_RGB(125, 125, 125));
    }

    RSGL_window_close(win);
    return 0;
}