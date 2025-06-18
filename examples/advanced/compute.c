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
    RGFW_window* window = RGFW_createWindow("RSGL Shader example", RGFW_RECT(0, 0, 750, 500), RGFW_windowCenter);
	
	RSGL_init(RSGL_AREA(window->r.w, window->r.h), RGFW_getProcAddress, RSGL_GL_renderer());

	RSGL_programInfo program = RSGL_renderCreateComputeProgram(ComputeShader);
	u32 texture = RSGL_renderCreateTexture(NULL, RSGL_AREA(200, 200), 4);

	RGFW_window_showMouse(window, 0);

	while (RGFW_window_shouldClose(window) == false) {
		while (RGFW_window_checkEvent(window)) {
			if (window->event.type == RGFW_quit) break;
		}

		RSGL_setProgram(program.program);
		RSGL_renderBindComputeTexture(texture, 4);
		RSGL_renderSetShaderValue(program.program, "u_time", (float[1]){RGFW_getTime()}, 1);
		RSGL_renderSetShaderValue(program.program, "u_screen_size", (float[2]){20, 20}, 2);
		RSGL_renderDispatchComputeProgram(program, 20, 20, 1);

		RSGL_clear(RSGL_RGB(20, 20, 20));
		RSGL_setProgram(0);
		RSGL_setTexture(texture);
		RSGL_drawRect(RSGL_RECT(100, 200, 200, 200), RSGL_RGB(255, 255, 255));


		RSGL_draw();
		RGFW_window_swapBuffers(window);
	}

	RSGL_renderDeleteProgram(program);
	RSGL_renderDeleteTexture(texture);
	RSGL_free();
	RGFW_window_close(window);
}

#else

int main(void) {
	printf("This example does not support emscripten\n");
	return 1;
}

#endif // __EMSCRIPTEN__
