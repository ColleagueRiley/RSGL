#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION

#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#define RSGL_NO_AUDIO
#define RSGLDEF 
#include "RSGL.h"
#include <stdio.h>

int main(void) {
    RSGL_window* win = RSGL_createWindow("RSGL event example", (RSGL_rect){0, 200, 800, 500}, RSGL_ALLOW_DND | RSGL_CENTER);

    bool running = true;

    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            switch (win->event.type) {
                case RSGL_keyPressed:
                    printf("key pressed %i %s %i\n", win->event.keyCode, win->event.keyName);
                    break;
                case RSGL_keyReleased:
                    printf("key released %i %s\n", win->event.keyCode, win->event.keyName);
                    break;
                case RSGL_mouseButtonPressed:
                    printf("mouse button pressed %i\n", win->event.button);
                    break;
                case RSGL_mouseButtonReleased:
                    printf("mouse button released %i\n", win->event.button);
                case RSGL_mousePosChanged:
                    if (RSGL_isPressedI(win, RGFW_ShiftL))
                        printf("mouse moved %i %i\n", win->event.x, win->event.y);
                    break;
                case RSGL_jsButtonPressed:
                    printf("joystick pressed %i\n", win->event.joystick);
                case RSGL_jsButtonReleased:
                    printf("joystick released %i\n", win->event.joystick);
                case RSGL_jsAxisMove:
                    printf("axis moved %i %i\n", win->event.axis[0][0], win->event.axis[0][1]);
                    break;
                case RSGL_dnd:
                    printf("dropped file : %i %i %i\n", win->event.x, win->event.y);
                    int i;
                    for (i = 0; i < win->event.droppedFilesCount; i++)
                        printf("%s\n", win->event.droppedFiles[i]);
                    printf("\n");
                    break;
                case RSGL_windowAttribsChange:
                    printf("window moved or resized\n");
                    break;
                default: break;
            }
        }

        RSGL_window_clear(win, RSGL_RGB(125, 125, 125));
    }

    RSGL_window_close(win);
    return 0;
}