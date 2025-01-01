#include <GLFW/glfw3.h>

#define RSGL_IMPLEMENTATION
#include "RSGL.h"

#define GLFW_UNUSED(x) if(x){}

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error %i: %s\n", error, description);
}

int main(void)
{
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

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    RSGL_init((RSGL_area){640, 480}, glfwGetProcAddress);

    u32 comicSans = RSGL_loadFont("COMICSANS.ttf");

    while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

        RSGL_setFont(comicSans);
        RSGL_clear(RSGL_RGB(20, 20, 60));
        RSGL_drawText("Hello, GLFW!", RSGL_CIRCLE(100, 100, 20), RSGL_RGB(255, 255, 255));

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
        
        RSGL_draw();
		glfwSwapBuffers(window);
	}

    glfwDestroyWindow(window);

    RSGL_free();

    glfwTerminate();
}
