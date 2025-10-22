#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#include <time.h>

/*
    this example is designed for Opengl 3.3 GLSL shaders
*/

#ifndef __EMSCRIPTEN__
static const char* MY_VShaderCode = RSGL_MULTILINE_STR(
    \x23version 330                     \n
    in vec3 vertexPosition;            \n
    in vec2 vertexTexCoord;            \n
    in vec4 vertexColor;               \n
    out vec2 fragTexCoord;             \n
    out vec4 fragColor;                \n
    out vec4 position;                \n

    uniform mat4 model; \n
    uniform mat4 pv; \n

    uniform vec2 u_mouse;

    void main() {
        fragTexCoord = vertexTexCoord;
        fragColor = vertexColor;
        gl_Position = model * pv  * (vec4(vertexPosition, 1.0) + vec4(u_mouse.x, u_mouse.y, 0, 0));
        position = gl_Position;
    }
);

static const char* MY_FShaderCode = RSGL_MULTILINE_STR(
    \x23version 330 core \n

    out vec4 FragColor;

    uniform vec2 u_resolution;
    uniform float u_time;

    uniform vec2 pos;

    precision mediump float;
    vec2 pitch  = vec2(50., 50.);

    void main() {
        vec2 uv = gl_FragCoord.xy / u_resolution.xy;
        vec3 color = 0.5 + 0.5 * cos(u_time + uv.xyx + vec3(0, 2, 4));

        if (mod(gl_FragCoord.x, pitch[0]) < 1. ||
            mod(gl_FragCoord.y, pitch[1]) < 1.) {
            gl_FragColor = vec4(color, 1.0);
        } else {
            gl_FragColor = vec4(0, 0, 0, 1);
        }
    }
);
#else
static const char* MY_VShaderCode = RSGL_MULTILINE_STR(
	\x23version 100                      \n
    attribute  vec3 vertexPosition;            \n
    attribute vec2 vertexTexCoord;            \n
    attribute vec4 vertexColor;               \n
    varying vec2 fragTexCoord;             \n
    varying vec4 fragColor;                \n
    varying vec4 position;                \n

    uniform mat4 model; \n
    uniform mat4 pv; \n

    uniform vec2 u_mouse;

    void main() {
        fragTexCoord = vertexTexCoord;
        fragColor = vertexColor;
        gl_Position = model * pv  * (vec4(vertexPosition, 1.0) + vec4(u_mouse.x, u_mouse.y, 0, 0));
        position = gl_Position;
    }
);

static const char* MY_FShaderCode = RSGL_MULTILINE_STR(
	\x23version 100                      \n
	precision mediump float;           \n

    varying vec4 FragColor;

    uniform vec2 u_resolution;
    uniform float u_time;

    uniform vec2 pos;

    precision mediump float;
    vec2 pitch  = vec2(50., 50.);

    void main() {
        vec2 uv = gl_FragCoord.xy / u_resolution.xy;
        vec3 color = 0.5 + 0.5 * cos(u_time + uv.xyx + vec3(0, 2, 4));

        if (mod(gl_FragCoord.x, pitch[0]) < 1. ||
            mod(gl_FragCoord.y, pitch[1]) < 1.) {
            gl_FragColor = vec4(color, 1.0);
        } else {
            gl_FragColor = vec4(0, 0, 0, 1);
        }
    }
);
#endif

int main(void) {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* window = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, 500, 500));
	RSGL_renderer_updateSize(renderer, 500, 500);

	RSGL_programBlob blob;
	blob.vertex = MY_VShaderCode;
	blob.vertexLen = sizeof(MY_VShaderCode);
	blob.fragment = MY_FShaderCode;
	blob.fragmentLen = sizeof(MY_FShaderCode);

	RSGL_programInfo program = RSGL_renderer_createProgram(renderer, &blob);

    RGFW_window_showMouse(window, 0);

	size_t u_time = RSGL_renderer_findShaderVariable(renderer, &program, "u_time", 6);
	size_t u_resolution = RSGL_renderer_findShaderVariable(renderer, &program, "u_resolution", 12);
	size_t u_mouse = RSGL_renderer_findShaderVariable(renderer, &program, "u_mouse", 7);
	size_t pos = RSGL_renderer_findShaderVariable(renderer, &program, "pos", 3);

	RSGL_UNUSED(u_time);
	RSGL_UNUSED(u_resolution);
	RSGL_UNUSED(u_mouse);
	RSGL_UNUSED(pos);

    while (RGFW_window_shouldClose(window) == false) {
		RGFW_pollEvents();

        RSGL_renderer_clear(renderer, RSGL_RGB(20, 20, 20));
        RSGL_renderer_setProgram(renderer, NULL);

		RSGL_renderer_setColor(renderer, RSGL_RGB(255, 0, 0));
        RSGL_drawRect(renderer, RSGL_RECT(100, 200, 200, 200));

        RSGL_renderer_render(renderer);

		RSGL_renderer_setProgram(renderer, &program);
        RSGL_drawOval(renderer, RSGL_RECT(0, 0, 60, 60));

		RSGL_renderer_updateShaderVariable(renderer, &program, u_time, (float*)(const float[1]){(float)(time(NULL) / 1000)}, 1);
        RSGL_renderer_updateShaderVariable(renderer, &program, u_resolution, (float*)(const float[2]){(float)window->w, (float)window->h}, 2);

		i32 x, y;
		RGFW_window_getMouse(window, &x, &y);

		RSGL_renderer_updateShaderVariable(renderer, &program, u_mouse, (float*)(const float[3]){(float)x, (float)y, 0}, 2);
        RSGL_renderer_updateShaderVariable(renderer, &program, pos, (float*)(const float[2]){(float)x, (float)y}, 2);

        RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(window);
	}

    RSGL_renderer_deleteProgram(renderer, &program);
    RSGL_renderer_free(renderer);
	RGFW_window_close(window);
}
