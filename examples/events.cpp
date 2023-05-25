#define RSGL_IMPLEMENTATION

#ifdef _WIN32
#undef __unix__
#undef __linux__
#endif

#include "RSGL.hpp"

int main(void) {
    RSGL::window win("RSGL event example", (RSGL::rect){0, 200, 800, 500}, (RSGL::color){125, 125, 125, 100}, RSGL::allowDnd);

    win.registerJoystick(0);

    while (win.isOpen()) {
        switch (win.event.type) {
            case RSGL::keyPressed:
                printf("key pressed %i %s %i\n", win.event.keyCode, win.event.keyName);
                break;
            case RSGL::keyReleased:
                printf("key released %i %s\n", win.event.keyCode, win.event.keyName);
                break;
            case RSGL::mouseButtonPressed:
                printf("mouse button pressed %i\n", win.event.button);
                break;
            case RSGL::mouseButtonReleased:
                printf("mouse button released %i\n", win.event.button);
            case RSGL::mousePosChanged:
                if (win.isPressed("Shift_L"))
                    printf("mouse moved %i %i\n", win.event.mouse.x, win.event.mouse.y);
                break;
            case RSGL::jsButtonPressed:
                printf("joystick pressed %i\n", win.event.joystick);
            case RSGL::jsButtonReleased:
                printf("joystick released %i\n", win.event.joystick);
            case RSGL::jsAxisMove:
                printf("axis moved %i %i\n", win.event.axis[0][0], win.event.axis[0][1]);
                break;
            case RSGL::dnd:
                printf("dropped file : %i %i %i\n", win.event.mouse.x, win.event.mouse.y);
                for (int i = 0; i < win.event.droppedFilesCount; i++)
                    printf("%s\n", win.event.droppedFiles[i]);
                printf("\n");
                break;
            default: break;
        }
    }

    return 0;
}