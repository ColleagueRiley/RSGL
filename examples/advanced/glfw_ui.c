#define RSGL_IMPLEMENTATION
#include "RSGL_glfw.h" 

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

    GLFWwindow* window = glfwCreateWindow(600, 480, "RSGL GLFW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    RSGL_init((RSGL_area){600, 480}, glfwGetProcAddress);

	RSGL_glfw_init(window);

    u32 comicSans = RSGL_loadFont("COMICSANS.ttf");

    float slider_value = 0.0;	
	b8 slider_grabbed = 0,
	   checkbox = 0,
	   toggle = 0,
	   combo_open = 1;

	size_t combo_index = 0;
	u8 selected = 0, index;

    while (!glfwWindowShouldClose(window)) {
		RSGL_checkEvent(window);

        RSGL_setFont(comicSans);
		RSGL_openBlankContainer(RSGL_RECT(0, 0, 600, 480));
		
		RSGL_widgetAlign(RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
		RSGL_labeledButton("generic", RSGL_RECTF(50, 50, 100, 50), RSGL_OFFSET_Y | RSGL_STYLE_DARK | RSGL_STYLE_ROUND);
		
		RSGL_widgetPolygonPoints(8);
		
		RSGL_widgetColor(RSGL_RGB(200, 0, 0), RSGL_RGB(200, 0, 0), RSGL_RGB(255, 0, 0), RSGL_RGB(200, 0, 0));
		RSGL_widgetOutline(10, RSGL_RGB(100, 0, 0), RSGL_RGB(100, 0, 0), RSGL_RGB(100, 0, 0), RSGL_RGB(100, 0, 0));

		if (RSGL_button(RSGL_RECTF(200, 200, 200, 200), RSGL_SHAPE_POLYGON) == RSGL_PRESSED) {
			RSGL_label("other text", RSGL_RECTF(200, 200, 200, 200));
		}
		else {
			RSGL_label("text", RSGL_RECTF(200, 200, 200, 200));
		}
				
		RSGL_widgetAlign(RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE);
        
		RSGL_checkbox(RSGL_RECTF(50, 250, 50, 50), RSGL_STYLE_DARK, &checkbox);	 		
	
		RSGL_widgetPolygonPoints(100);
		RSGL_radioButtons(RSGL_RECTF(50, 320, 32, 32), 3, RSGL_STYLE_DARK | RSGL_SHAPE_POLYGON, &selected, &index); 
		
		RSGL_widgetRounding(RSGL_POINT(10, 10));
		RSGL_slider(RSGL_RECTF(160, 450, 260, 15), RSGL_STYLE_DARK | RSGL_STYLE_ROUND | RSGL_SLIDER_CIRCLE, &slider_value, &slider_grabbed);
		
		RSGL_widgetOutline(0, RSGL_RGB(255, 255, 255),  RSGL_RGB(255, 255, 255),  RSGL_RGB(255, 255, 255),  RSGL_RGB(255, 255, 255));
		RSGL_widgetAlign(RSGL_ALIGN_LEFT | RSGL_ALIGN_DOWN);
        RSGL_clear(RSGL_RGB(20, 20, 60));
		
		glfwSwapBuffers(window);
	}

    glfwDestroyWindow(window);

    if (stdCursor != NULL)
        glfwDestroyCursor(stdCursor);

    RSGL_free();

    glfwTerminate();
}
