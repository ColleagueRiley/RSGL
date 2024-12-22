#define RSGL_IMPLEMENTATION

#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#include "RSGL.h"
#include <stdio.h>

int main(void) {
    RGFW_window* win = RGFW_createWindow("RSGL event example", (RSGL_rect){0, 200, 800, 500}, RGFW_ALLOW_DND | RGFW_CENTER);

	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

    while (RGFW_window_shouldClose(win) == false) {
        while (RGFW_window_checkEvent(win)) {
            if (win->event.type == RGFW_quit) {
                break;
            }

            switch (win->event.type) {
                case RGFW_keyPressed:
                    printf("key pressed %i %c %s\n", win->event.key, win->event.keyChar, win->event.keyName);
                    break;
                case RGFW_keyReleased:
                    printf("key released %i %c %s\n", win->event.key, win->event.keyChar, win->event.keyName);
                    break;
                case RGFW_mouseButtonPressed:
                    printf("mouse button pressed %i\n", win->event.button);
                    break;
                case RGFW_mouseButtonReleased:
                    printf("mouse button released %i\n", win->event.button);
                    break;
                case RGFW_mousePosChanged:
                    if (RGFW_isPressed(win, RGFW_ShiftL))
                        printf("mouse moved %i %i\n", win->event.point.x, win->event.point.y);
                    break;
                case RGFW_gpButtonPressed:
                    printf("gamepad pressed %i\n", win->event.gamepad);
                    break;
                case RGFW_gpButtonReleased:
                    printf("gamepad released %i\n", win->event.gamepad);
                    break;
                case RGFW_gpAxisMove:
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
        
        RSGL_clear(RSGL_RGB(125, 125, 125));
		RGFW_window_swapBuffers(win);
    }
	
	RSGL_free();
    RGFW_window_close(win);
    return 0;
}
