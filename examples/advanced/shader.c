#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_IMPLEMENTATION
#include "RSGL.h"

/*
    this example is designed for Opengl 3.3 GLSL shaders
*/

static const char* MY_VShaderCode = RSGL_MULTILINE_STR(
    \x23version 330                     \n
    in vec3 vertexPosition;            \n
    in vec2 vertexTexCoord;            \n
    in vec4 vertexColor;               \n
    out vec2 fragTexCoord;             \n
    out vec4 fragColor;                \n
    out vec4 position;                \n

    uniform vec2 u_mouse;
    uniform vec2 u_resolution;

    void main() {
        fragTexCoord = vertexTexCoord;
        fragColor = vertexColor;
        gl_Position = vec4(vertexPosition, 1.0) + vec4(u_mouse.x, u_mouse.y, 0, 0);
        position = gl_Position;
    }
);

static const char* MY_FShaderCode = RSGL_MULTILINE_STR(
    \x23version 330 core \n

    out vec4 FragColor;

    uniform vec2 u_resolution;
    uniform float u_time;

    uniform vec2 pos;
    uniform vec2 u_mouse;

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

int main(void) {
    RGFW_window* window = RGFW_createWindow("RSGL Shader example", RGFW_RECT(0, 0, 750, 500), RGFW_CENTER);
    
	RSGL_init(RSGL_AREA(window->r.w, window->r.h), RGFW_getProcAddress);

    RSGL_programInfo program = RSGL_renderCreateProgram(MY_VShaderCode, MY_FShaderCode, "vertexPosition", "vertexTexCoord", "vertexColor");

    while (RGFW_window_shouldClose(window) == false) {
        while (RGFW_window_checkEvent(window)) {
			if (window->event.type == RGFW_quit) break;
		}

        RSGL_clear(RSGL_RGB(20, 20, 20));
        RSGL_setProgram(0);
        RSGL_drawRect(RSGL_RECT(100, 200, 200, 200), RSGL_RGB(255, 0, 0));

        RSGL_setProgram(program.program);
        RSGL_drawCircle(RSGL_CIRCLE(300, 200, 60), RSGL_RGB(255, 0, 0));

        RSGL_renderSetShaderValue(program.program, "u_time", (float[1]){RGFW_getTime()}, 1);
        RSGL_renderSetShaderValue(program.program, "u_resolution", (float[2]){(float)window->r.w, (float)window->r.h}, 2);
        RSGL_renderSetShaderValue(program.program, "u_mouse", (float[3]){RSGL_GET_WORLD_POINT((float)window->event.point.x, (float)window->event.point.y, 0)}, 2);
        RSGL_renderSetShaderValue(program.program, "pos", (float[2]){(float)window->event.point.x, (float)window->event.point.y}, 2);

        RSGL_draw();
		RGFW_window_swapBuffers(window);
	}

    RSGL_renderDeleteProgram(program);
    RSGL_free();
	RGFW_window_close(window);
}
