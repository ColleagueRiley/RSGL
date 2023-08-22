#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include <stdio.h>

int main(void) {
    RSGL_window* win = RSGL_createWindow("RSGL event example", (RSGL_rect){0, 200, 800, 500}, RGFW_ALLOW_DND);

    bool running = true;

    while (running) {
        while (RGFW_window_checkEvent(win))
            switch (win->event.type) {
                case RGFW_keyPressed:
                    printf("key pressed %i %s %i\n", win->event.keyCode, win->event.keyName);
                    break;
                case RGFW_keyReleased:
                    printf("key released %i %s\n", win->event.keyCode, win->event.keyName);
                    break;
                case RGFW_mouseButtonPressed:
                    printf("mouse button pressed %i\n", win->event.button);
                    break;
                case RGFW_mouseButtonReleased:
                    printf("mouse button released %i\n", win->event.button);
                case RGFW_mousePosChanged:
                    if (RGFW_isPressedI(win, RGFW_ShiftL))
                        printf("mouse moved %i %i\n", win->event.x, win->event.y);
                    break;
                case RGFW_jsButtonPressed:
                    printf("joystick pressed %i\n", win->event.joystick);
                case RGFW_jsButtonReleased:
                    printf("joystick released %i\n", win->event.joystick);
                case RGFW_jsAxisMove:
                    printf("axis moved %i %i\n", win->event.axis[0][0], win->event.axis[0][1]);
                    break;
                case RGFW_dnd:
                    printf("dropped file : %i %i %i\n", win->event.x, win->event.y);
                    int i;
                    for (i = 0; i < win->event.droppedFilesCount; i++)
                        printf("%s\n", win->event.droppedFiles[i]);
                    printf("\n");
                    break;
                case RGFW_quit:
                    printf("window close event\n");
                    running = false;
                case RGFW_windowAttribsChange:
                    printf("window moved or resized\n");
                    break;
                default: break;
            }

        RSGL_window_clear(win, RSGL_RGB(125, 125, 125));
    }

    return 0;
}