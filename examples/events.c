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


void display() {
    rglMatrixMode(RGL_MODELVIEW);
    rglLoadIdentity();
    rglTranslatef(0.0f, 0.0f, -5.0f); // Move the cube back along the z-axis
    
    static float angle = 0.0f;
    angle += 0.5f; // Increment the rotation angle
    
    rglRotatef(angle, 1.0f, 1.0f, 1.0f); // Rotate the cube around the x, y, and z axes
    
    // Render the cube
    rglBegin(RGL_QUADS);
    // Front face
    rglColor3f(1.0, 0.0, 0.0); // Red
    rglVertex3f(-1.0, -1.0, 1.0);
    rglVertex3f(1.0, -1.0, 1.0);
    rglVertex3f(1.0, 1.0, 1.0);
    rglVertex3f(-1.0, 1.0, 1.0);
    // Back face
    rglColor3f(0.0, 1.0, 0.0); // Green
    rglVertex3f(-1.0, -1.0, -1.0);
    rglVertex3f(-1.0, 1.0, -1.0);
    rglVertex3f(1.0, 1.0, -1.0);
    rglVertex3f(1.0, -1.0, -1.0);
    // Top face
    rglColor3f(0.0, 0.0, 1.0); // Blue
    rglVertex3f(-1.0, 1.0, -1.0);
    rglVertex3f(-1.0, 1.0, 1.0);
    rglVertex3f(1.0, 1.0, 1.0);
    rglVertex3f(1.0, 1.0, -1.0);
    // Bottom face
    rglColor3f(1.0, 1.0, 0.0); // Yellow
    rglVertex3f(-1.0, -1.0, -1.0);
    rglVertex3f(1.0, -1.0, -1.0);
    rglVertex3f(1.0, -1.0, 1.0);
    rglVertex3f(-1.0, -1.0, 1.0);
    // Right face
    rglColor3f(1.0, 0.0, 1.0); // Magenta
    rglVertex3f(1.0, -1.0, -1.0);
    rglVertex3f(1.0, 1.0, -1.0);
    rglVertex3f(1.0, 1.0, 1.0);
    rglVertex3f(1.0, -1.0, 1.0);
    // Left face
    rglColor3f(0.0, 1.0, 1.0); // Cyan
    rglVertex3f(-1.0, -1.0, -1.0);
    rglVertex3f(-1.0, -1.0, 1.0);
    rglVertex3f(-1.0, 1.0, 1.0);
    rglVertex3f(-1.0, 1.0, -1.0);
    rglEnd();
}

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
                        printf("mouse moved %i %i\n", win->event.point.x, win->event.point.y);
                    break;
                case RSGL_jsButtonPressed:
                    printf("joystick pressed %i\n", win->event.joystick);
                case RSGL_jsButtonReleased:
                    printf("joystick released %i\n", win->event.joystick);
                case RSGL_jsAxisMove:
                    printf("axis moved %i %i\n", win->event.axis[0].x, win->event.axis[0].y);
                    break;
                case RSGL_dnd:
                    printf("dropped file : %i %i %i\n", win->event.point.x, win->event.point.y);
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

        display();
        RSGL_window_clear(win, RSGL_RGB(125, 125, 125));
    }

    RSGL_window_close(win);
    return 0;
}