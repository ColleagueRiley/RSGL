#define RSGL_NO_AUDIO
#define RSGL_NO_RGFW
#define RSGL_IMPLEMENTATION

#include <GLFW/glfw3.h>

#define RSGL_Up GLFW_KEY_UP
#define RSGL_Down GLFW_KEY_DOWN
#define RSGL_Left GLFW_KEY_LEFT
#define RSGL_Right GLFW_KEY_RIGHT
#define RSGL_Tab GLFW_KEY_TAB
#define RSGL_BackSpace GLFW_KEY_BACKSPACE

#include "RSGL.h"

u8 GLFW_keys[512];
RGFW_Event GLFW_event;

u8 RSGL_isPressedI(void* win, u32 key) {
    return GLFW_keys[key];
}

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    RSGL_graphics_updateSize(RSGL_AREA(width, height));
}

static void charCallback(GLFWwindow *window, unsigned int codepoint) {
    GLFW_event.keyName[0] = codepoint;
    GLFW_event.keyName[1] = '\0';
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        GLFW_keys[key] = 1;
        GLFW_event.type = RSGL_keyPressed;
        GLFW_event.keyCode = key;
    }

    else if (action == GLFW_RELEASE) {
        GLFW_keys[key] = 0;

        GLFW_event.type = RSGL_keyReleased;
        GLFW_event.keyCode = key;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    button++;

    if (action == GLFW_PRESS) {
        GLFW_event.type = RSGL_mouseButtonPressed;
        GLFW_event.button = button;
    }

    else if (action == GLFW_RELEASE) {
        GLFW_event.type = RSGL_mouseButtonReleased;
        GLFW_event.button = button;
    }
}

static void mouseCursorPosCallback(GLFWwindow *window, double x, double y) {
    GLFW_event.type = RSGL_mousePosChanged;
    GLFW_event.point = RSGL_POINT((i32)x, (i32)y);
}

static void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    double scroll = yoffset; 
    if (abs(xoffset) > abs(yoffset))
        scroll = xoffset;

    GLFW_event.type = RSGL_mouseButtonPressed;

    if (scroll > 0)
        GLFW_event.button = RSGL_mouseScrollUp;
    else
        GLFW_event.button = RSGL_mouseScrollDown;
    
    GLFW_event.scroll = scroll;
}

int main(void)
{
    GLFW_event.type = 0;

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "RSGL GLFW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mouseCursorPosCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    RSGL_initGraphics((RSGL_area){640, 480}, glfwGetProcAddress);

    u32 easy_font = RSGL_loadFont("Super Easy.ttf");
    RSGL_setFont(easy_font);

    /* generic button */
    RSGL_button generic = RSGL_initButton();
    
    /* this can be a rect or polygon */
    RSGL_button_setRect(&generic, RSGL_RECT(50, 50, 100, 50));
    
    RSGL_button_setText(&generic, "generic", 8, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(100, 100, 100));
    RSGL_button_alignText(&generic, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
    
    RSGL_button_setStyle(&generic, RSGL_STYLE_DARK | RSGL_STYLE_ROUNDED);

    RSGL_button_setKeybind(&generic, (u32[]){GLFW_KEY_F, GLFW_KEY_LEFT_CONTROL}, 2);
    RSGL_button_setWindow(&generic, window);

    u32 comicSans = RSGL_loadFont("COMICSANS.ttf");

    RSGL_textbox* tb = RSGL_initTextbox(0);
    RSGL_textbox_setTextInfo(tb, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    
    RSGL_textbox_setRect(tb, RSGL_RECT(20, 20, 200, 30));
    RSGL_textbox_setStyle(tb, RSGL_STYLE_DARK);
    RSGL_textbox_alignText(tb, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();

        RSGL_button_update(&generic, GLFW_event);
        RSGL_textbox_update(tb, GLFW_event);

        GLFW_event.type = 0;

        RSGL_setFont(easy_font);
        RSGL_drawButton(generic);

        RSGL_setFont(comicSans);
        RSGL_textbox_draw(tb);

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
        RSGL_graphics_clear(RSGL_RGB(20, 20, 60));
    }

    glfwDestroyWindow(window);

    RSGL_graphics_free();

    glfwTerminate();
}