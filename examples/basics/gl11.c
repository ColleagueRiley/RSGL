#define RSGL_RFONT
#define RFONT_IMPLEMENTATION
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl1.h"

#define RGFW_INT_DEFINED
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "examples/deps/RGFW.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <time.h>

/* TODO: add more facts */
const char* facts[] = {
	"The hedgehog in the logo is named 'Lonic';\na joke name combining 'Linux' and 'Sonic'.",
	"Postal 3 sucks",
	"RSGL has been rewritten several times",
	"RSGL was originally created in 2020",
	"RSGLmake was a custom build system that can build RSGL projects",
	"RSGL was originally written in C++, BLEH!"
};

const size_t factCount = (sizeof(facts) / sizeof(char*));

void rollDie(RSGL_renderer* renderer, i32* index, float* value) {
	if (value) *value = -(*value);
	*index = rand() % factCount;
	RSGL_renderer_setColor(renderer, RSGL_RGB(rand() % 255, rand() % 255, rand() % 255));
}

int main() {
	RGFW_window* window = RGFW_createWindow("window", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL1_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);
    RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, 500, 500));
	RSGL_renderer_updateSize(renderer, 500, 500);

    int w, h, c;
    u8* logo = stbi_load("logo.png", &w, &h, &c, 4);
	RSGL_textureBlob blob;
	blob.data = logo;
	blob.width = w;
	blob.height = h;
	blob.dataType = RSGL_textureDataInt;
	blob.dataFormat = (c == 4) ? RSGL_formatRGBA : RSGL_formatRGB;
	blob.textureFormat = blob.dataFormat;
    u32 texture = RSGL_renderer_createTexture(renderer, &blob);
    free(logo);

	srand(time(0));

	RSGL_rect rect = RSGL_RECT(225, 225, 100, 100);
	RSGL_vec2D vec = RSGL_VEC2D(2, 3);
	RSGL_renderer_setColor(renderer, RSGL_RGB(rand() % 255, rand() % 255, rand() % 255));

	i32 framebufferWidth;
	i32 framebufferHeight;

	RFont_renderer renderer_rfont;
	RFont_RSGL_renderer_initPtr(renderer, &renderer_rfont);

    RFont_font* font = RFont_font_init(&renderer_rfont, "COMICSANS.ttf", 60, 500, 500);

	i32 factIndex = 0;
	rollDie(renderer, &factIndex, NULL);

	while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
		RGFW_pollEvents();

		RGFW_window_getSize(window, &framebufferWidth, &framebufferHeight);
		RSGL_renderer_updateSize(renderer, framebufferWidth, framebufferHeight);

        RSGL_renderer_setTexture(renderer, texture);

		RSGL_drawRect(renderer, rect);

		rect.w = (0.1f * ((float)framebufferWidth + (float)framebufferHeight));
		rect.h = rect.w;

		if (RGFW_isKeyPressed(RGFW_space)) {
			rect.x = (framebufferWidth / 2) - rect.h;
			rect.y = (framebufferHeight / 2) - rect.h;
		}

		if (rect.x >= (framebufferWidth - rect.w) || rect.x <= 0)
			rollDie(renderer, &factIndex, &vec.x);
		else if (rect.y >= (framebufferHeight - rect.h) || rect.y <= 0)
			rollDie(renderer, &factIndex, &vec.y);

		rect.x += vec.x;
		rect.y += vec.y;

		RSGL_renderer_clear(renderer, RSGL_RGB(10, 50, 100));

		RSGL_color prev = renderer->state.color;
		RSGL_renderer_setColor(renderer, RSGL_RGB(100, 100, 100));

		RFont_draw_text(&renderer_rfont, font,  facts[factIndex], 0, framebufferHeight - 50, 20);

		RSGL_renderer_setColor(renderer, prev);

		RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(window);
	}

    RFont_font_free(&renderer_rfont, font);

	RSGL_renderer_free(renderer);
	RGFW_window_close(window);
}
