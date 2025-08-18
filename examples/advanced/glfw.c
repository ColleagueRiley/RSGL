#include <GLFW/glfw3.h>

#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

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

	RSGL_renderer renderer = RSGL_GL_renderer();
	RSGL_renderer_init(&renderer, RSGL_AREA(640, 480), glfwGetProcAddress);

    RSGL_font* comicSans = RSGL_loadFont(&renderer, "COMICSANS.ttf");

    while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

        RSGL_renderer_setFont(&renderer, comicSans);
        RSGL_renderer_clear(&renderer, RSGL_RGB(20, 20, 60));

		RSGL_renderer_setColor(&renderer, RSGL_RGB(255, 255,  255));
        RSGL_drawText(&renderer, "Hello, GLFW!", RSGL_CIRCLE(100, 100, 20));

		RSGL_renderer_setColor(&renderer, RSGL_RGB(255, 0, 0));
        RSGL_drawRect(&renderer, (RSGL_rect){200, 200, 200, 200});

        RSGL_renderer_render(&renderer);
		glfwSwapBuffers(window);
	}

    glfwDestroyWindow(window);

    RSGL_renderer_free(&renderer);

    glfwTerminate();
}
