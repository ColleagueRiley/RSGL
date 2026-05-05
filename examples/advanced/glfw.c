#include <GLFW/glfw3.h>

#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#define GLFW_UNUSED(x) if(x){}

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error %i: %s\n", error, description);
}

RSGL_renderer* renderer;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//	RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, width, height));
	RSGL_renderer_updateSize(renderer, width, height);
}

int main(void)
{
    glfwSetErrorCallback(errorCallback);
glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "RSGL GLFW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

	renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)glfwGetProcAddress);
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, 500, 500));
	RSGL_renderer_updateSize(renderer, 500, 500);

    while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		RSGL_renderer_clear(renderer, RSGL_RGB(20, 20, 60));

		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
        RSGL_drawRect(renderer, (RSGL_rect){200, 200, 200, 200});

        RSGL_renderer_render(renderer);
		glfwSwapBuffers(window);
	}

    glfwDestroyWindow(window);

    RSGL_renderer_free(renderer);

    glfwTerminate();
}
