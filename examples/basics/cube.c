#include <stdio.h>
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RGFW_INT_DEFINED
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

	RGFW_window* win = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL | RGFW_windowNoResize);
    RGFW_window_setExitKey(win, RGFW_escape);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, 500, 500));
	RSGL_renderer_updateSize(renderer, RSGL_AREA(500, 500));

	glEnable(GL_DEPTH_TEST);

	RSGL_projection projection;
	projection.type = RSGL_projectionPerspective3D;
	projection.p3D.fov = 40;
	projection.p3D.ratio = 1.0; /* 500 / 500 */
	projection.p3D.pNear = 0.0001;
	projection.p3D.pFar = 1000;

	RSGL_mat4 perMat = RSGL_projection_getMatrix(&projection);

	RSGL_renderer_setPerspectiveMatrix(renderer, perMat);

	RSGL_view view;
	RSGL_MEMSET(&view, 0, sizeof(view));

	view.type = RSGL_viewType3D;
	view.view3D.up = RSGL_POINT3D(0, 10, 0);
	view.view3D.target = RSGL_POINT3D(0, 0, 10);
	view.view3D.pos = RSGL_POINT3D(-1, 0, 1);

	RSGL_mat4 viewMat = RSGL_view_getMatrix(&view);
	RSGL_renderer_setViewMatrix(renderer, viewMat);

    //RSGL_point3D rotate = RSGL_POINT3D(0, 0, 0);

	while (RGFW_window_shouldClose(win) == 0) {
		RGFW_pollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        RSGL_renderer_clear(renderer, RSGL_RGB(80, 80, 110));

		// RSGL_renderer_setRotate(renderer, rotate);
		// rotate.z += 0.01;
		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
		RSGL_drawCube(renderer, RSGL_CUBE(0, 300, 1, 200, 200, 1));

        RSGL_renderer_render(renderer);
        RGFW_window_swapBuffers_OpenGL(win);
    }

    RSGL_renderer_free(renderer);
    RGFW_window_close(win);
}
