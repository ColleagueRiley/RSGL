#ifndef RSGL_BACKEND /* all backends must start like this */
#define RSGL_BACKEND /* this is show RSGL knows if a backend has been loaded or not*/

#include <GLFW/glfw3.h>
#include "RSGL.h"
#include "RSGL_ui.h"

#define GLFW_UNUSED(x) (void)(x);


RSGLDEF void RSGL_glfw_init(GLFWwindow* window);

#ifdef RSGL_IMPLEMENTATION

GLFWcursor* stdCursor = NULL;

void RSGL_setMouseStandard(void* userPtr, u32 cursorIcon) {
    static u32 lastIcon = 0; 
    if (cursorIcon == lastIcon)
        return;
    
    if (stdCursor != NULL)
        glfwDestroyCursor(stdCursor);
    
    stdCursor = glfwCreateStandardCursor(cursorIcon);

    glfwSetCursor(userPtr, stdCursor);
    lastIcon = cursorIcon;
}



u8 RSGL_convertKey(u32 key) {
	switch (key) {
		case GLFW_KEY_UP: return RSGL_Up;
		case GLFW_KEY_DOWN: return RSGL_Down;
		case GLFW_KEY_LEFT: return RSGL_Left;
		case GLFW_KEY_RIGHT: return RSGL_Right;
		case GLFW_KEY_TAB: return RSGL_Tab;
		case GLFW_KEY_BACKSPACE: return RSGL_BackSpace;  
		default: return 0;
	}
}


GLFWkeyfun GLFW_old_key = NULL; 
GLFWcharfun GLFW_old_char = NULL;
GLFWwindowsizefun GLFW_old_resize = NULL;
GLFWmousebuttonfun GLFW_old_button = NULL;
GLFWcursorposfun GLFW_old_cursor = NULL;
GLFWscrollfun GLFW_old_scroll = NULL;


void windowSizeCallback(GLFWwindow *window, int width, int height) {
    GLFW_UNUSED(window)
    RSGL_updateSize(RSGL_AREA(width, height));

	if (GLFW_old_resize != NULL)
		GLFW_old_resize(window, width, height);
}

static void charCallback(GLFWwindow *window, unsigned int codepoint) {
    GLFW_UNUSED(window)
	
	RSGL_Event ev;
	ev.type = RSGL_keyPressed;
	ev.keyCode = 0;
	ev.ch = codepoint;

	RSGL_sendEvent(window, ev);
	
	if (GLFW_old_char != NULL)
		GLFW_old_char(window, codepoint);

}


static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLFW_UNUSED(window)
    GLFW_UNUSED(mods)
    GLFW_UNUSED(scancode)
	
	RSGL_Event ev;
	if (action == GLFW_PRESS) {
        ev.type = RSGL_keyPressed;
        ev.keyCode = key;
    }

    else if (action == GLFW_RELEASE) {
        ev.type = RSGL_keyReleased;
        ev.keyCode = key;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	
	RSGL_sendEvent(window, ev);
	
	if (GLFW_old_key != NULL)
		GLFW_old_key(window, key, scancode, action, mods);

}

static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    GLFW_UNUSED(window)
    GLFW_UNUSED(mods)
    button++;

	RSGL_Event ev;
    if (action == GLFW_PRESS) {
        ev.type = RSGL_mouseButtonPressed;
        ev.button = button;
    }

    else if (action == GLFW_RELEASE) {
        ev.type = RSGL_mouseButtonReleased;
        ev.button = button;
    }
	
	RSGL_sendEvent(window, ev);

	if (GLFW_old_button != NULL)
		GLFW_old_button(window, button, action, mods);
}

static void mouseCursorPosCallback(GLFWwindow *window, double x, double y) {
    GLFW_UNUSED(window)
    GLFW_UNUSED(window)
	
	RSGL_Event ev;
	ev.type = RSGL_mousePosChanged;
    ev.point = RSGL_POINT((i32)x, (i32)y);

	RSGL_sendEvent(window, ev);

	if (GLFW_old_cursor != NULL)
		GLFW_old_cursor(window, x, y);

}

static void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    GLFW_UNUSED(window)

    double scroll = yoffset; 
    if (fabsf((float)xoffset) > fabsf((float)yoffset))
        scroll = xoffset;

	RSGL_Event ev;
    ev.type = RSGL_mouseButtonPressed;

    if (scroll > 0)
        ev.button = RSGL_mouseScrollUp;
    else
        ev.button = RSGL_mouseScrollDown;
    
    ev.scroll = scroll;
	
	RSGL_sendEvent(window, ev);

	if (GLFW_old_scroll != NULL)
		GLFW_old_scroll(window, xoffset, yoffset);

}

void RSGL_glfw_init(GLFWwindow* window) {
	GLFW_old_key = glfwSetKeyCallback(window, keyCallback);
    GLFW_old_char = glfwSetCharCallback(window, charCallback);
    GLFW_old_resize = glfwSetWindowSizeCallback(window, windowSizeCallback);
    GLFW_old_button = glfwSetMouseButtonCallback(window, mouseButtonCallback);
    GLFW_old_cursor = glfwSetCursorPosCallback(window, mouseCursorPosCallback);
    GLFW_old_scroll = glfwSetScrollCallback(window, mouseScrollCallback);
}

b8 RSGL_checkEvent(void* userPtr) {	
	glfwPollEvents();
	return 0;
}

#endif
#endif
