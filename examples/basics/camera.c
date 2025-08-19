#include <stdio.h>
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#ifdef __APPLE__
	#ifndef GL_SILENCE_DEPRECATION
		#define GL_SILENCE_DEPRECATION
	#endif
	#include <OpenGL/gl.h>
	#include <OpenGL/OpenGL.h>
#else
    #include <GL/gl.h>
#endif


int main(void) {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* win = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), RSGL_AREA(500, 500), RGFW_getProcAddress_OpenGL);

	RSGL_camera camera = {.fov = 60};
    RGFW_window_holdMouse(win);
    RGFW_window_showMouse(win, RGFW_FALSE);
    glEnable(GL_DEPTH_TEST);

    while (RGFW_window_shouldClose(win) == 0) {
		RGFW_event event;
		while (RGFW_window_checkEvent(win, &event)) {
            switch (event.type) {
                case RGFW_mousePosChanged: {
                    camera.yaw += event.mouse.vecX / 15.0;
                    camera.pitch += event.mouse.vecY / 15.0;
                    break;
                }
                case RGFW_keyPressed:
                    switch (event.key.type) {
                       default: break;
                    }
                default: break;
            }
        }

        if (RGFW_isKeyDown(RGFW_left))
            camera.yaw--;
        if (RGFW_isKeyDown(RGFW_right))
            camera.yaw++;
        if (RGFW_isKeyDown(RGFW_up))
            camera.pitch--;
        if (RGFW_isKeyDown(RGFW_down))
            camera.pitch++;

        if (RGFW_isKeyDown(RGFW_w)) {
            camera.pos.x += cos((camera.yaw + 90) * DEG2RAD)/5.0;
            camera.pos.z -= sin((camera.yaw + 90) * DEG2RAD)/5.0;
        }
        if (RGFW_isKeyDown(RGFW_s)) {
            camera.pos.x += cos((camera.yaw + 270) * DEG2RAD)/5.0;
            camera.pos.z -= sin((camera.yaw + 270) * DEG2RAD)/5.0;
        }

        if (RGFW_isKeyDown(RGFW_a)) {
            camera.pos.x += cos(camera.yaw * DEG2RAD)/5.0;
            camera.pos.z -= sin(camera.yaw * DEG2RAD)/5.0;
        }

        if (RGFW_isKeyDown(RGFW_d)) {
            camera.pos.x += cos((camera.yaw + 180) * DEG2RAD)/5.0;
            camera.pos.z -= sin((camera.yaw + 180) * DEG2RAD)/5.0;
        }

        if (RGFW_isKeyDown(RGFW_controlL))
            camera.pos.y += 0.1;
        if (RGFW_isKeyDown(RGFW_space))
            camera.pos.y -= 0.1;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RSGL_renderer_clear(renderer, RSGL_RGB(80, 80, 110));

        RSGL_renderer_setMatrix(renderer, RSGL_renderer_getCameraMatrix(renderer, camera));

		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
		RSGL_drawCube(renderer, RSGL_CUBE(49, 20, 5, 200, 200, 0.5));
		RSGL_drawTriangle3D(renderer, RSGL_createTriangle3D(25.0f, 480.0f, 0.0,
															250, 480, 3.0f,
															800, 480.0f, 0.0f));

        RSGL_renderer_resetMatrix(renderer);

        RSGL_renderer_render(renderer);
        RGFW_window_swapBuffers_OpenGL(win);
    }

    RSGL_renderer_free(renderer);
    RGFW_window_close(win);
}
