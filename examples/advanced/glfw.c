#define RSGL_IMPLEMENTATION

#include "RSGL_glfw.h" 

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

	RSGL_glfw_init(window);

    u32 easy_font = RSGL_loadFont("Super Easy.ttf");
    RSGL_setFont(easy_font);

    u32 comicSans = RSGL_loadFont("COMICSANS.ttf");

    while (!glfwWindowShouldClose(window)) {
		RSGL_checkEvent(window);

        RSGL_setFont(easy_font);

		RSGL_openBlankContainer(RSGL_RECT(0, 0, 640, 480));
			
		RSGL_widgetAlign(RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
		RSGL_labeledButton("generic", RSGL_RECTF(50, 50, 100, 50), RSGL_STYLE_DARK | RSGL_STYLE_ROUND);		

        RSGL_setFont(comicSans);

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
        RSGL_clear(RSGL_RGB(20, 20, 60));
		
		glfwSwapBuffers(window);
	}

    glfwDestroyWindow(window);

    if (stdCursor != NULL)
        glfwDestroyCursor(stdCursor);

    RSGL_free();

    glfwTerminate();
}
