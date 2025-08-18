#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_IMPLEMENTATION
#define RSGL_USE_COMPUTE
#include "RSGL.h"
#include "RSGL_gl.h"

/*
 	Example showing off basic compute shader use

	Written by TKPMonke
*/

#ifndef __EMSCRIPTEN__

const static char* ComputeShader = RSGL_MULTILINE_STR(
	\x23version 430 core		\n
	layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;		\n
	layout (rgba8, binding = 0) uniform image2D output_img;		\n
	layout (location = 0) uniform float u_time; 					\n
	layout (location = 1) uniform vec2 u_screen_size;			\n

	const float c_speed = 100;
	void main() {
		vec4 value = vec4(0, 0, 0, 1);
		ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

		value.x = mod(float(texelCoord.x)+u_time * 100, u_screen_size.x)/gl_NumWorkGroups.x;
		value.y = mod(float(texelCoord.y)+u_time * 100, u_screen_size.y)/gl_NumWorkGroups.y;

		imageStore(output_img, texelCoord, value);
	}
);

int main(void) {
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->major = 3;
	hints->minor = 3;
	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_window* window = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer renderer = RSGL_GL_renderer();
	RSGL_renderer_init(&renderer, RSGL_AREA(500, 500), RGFW_getProcAddress_OpenGL);

	RSGL_programInfo program = RSGL_renderer_createComputeProgram(&renderer, ComputeShader);
	u32 texture = RSGL_renderer_createTexture(&renderer, NULL, RSGL_AREA(200, 200), 4);

	RGFW_window_showMouse(window, 0);

	while (RGFW_window_shouldClose(window) == false) {
		RGFW_pollEvents();

		RSGL_renderer_setProgram(&renderer, program.program);
		RSGL_renderer_bindComputeTexture(&renderer, texture, 4);
		RSGL_renderer_setShaderValue(&renderer, program.program, "u_time", (float[1]){time(NULL) / 1000}, 1);
		RSGL_renderer_setShaderValue(&renderer, program.program, "u_screen_size", (float[2]){20, 20}, 2);
		RSGL_renderer_dispatchComputeProgram(&renderer, program, 20, 20, 1);

		RSGL_renderer_clear(&renderer, RSGL_RGB(20, 20, 20));
		RSGL_renderer_setProgram(&renderer, 0);
		RSGL_renderer_setTexture(&renderer, texture);
		RSGL_drawRect(&renderer, RSGL_RECT(100, 200, 200, 200), RSGL_RGB(255, 255, 255));


		RSGL_renderer_render(&renderer);
		RGFW_window_swapBuffers_OpenGL(window);
	}

	RSGL_renderer_deleteProgram(&renderer, program);
	RSGL_renderer_deleteTexture(&renderer, texture);
	RSGL_renderer_free(&renderer);
	RGFW_window_close(window);
}

#else

int main(void) {
	printf("This example does not support emscripten\n");
	return 1;
}

#endif // __EMSCRIPTEN__
