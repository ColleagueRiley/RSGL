#define NK_IMPLEMENTATION
#include "nuklear.h"

#define RSGL_RFONT
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#define RGFW_DEBUG
#define GL_SILENCE_DEPRECATION
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RFONT_IMPLEMENTATION
#include "RFont.h"

RFont_renderer* renderer_rfont;

// nuklear
#define MAX_MEMORY (512 * 1024)
enum Difficulty {
    EASY,
    HARD
};
enum Difficulty op = EASY;

static void test_window(struct nk_context* ctx) {
	static float value = 0.5f;
    /* fixed widget pixel width */
    if (
		nk_begin(ctx, "Show", nk_rect(50, 50, 220, 220),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)
	) {
	    nk_layout_row_static(ctx, 30, 80, 1);
	    if (nk_button_label(ctx, "button")) {
	        /* event handling */
	    }

	    /* fixed widget window ratio width */
	    nk_layout_row_dynamic(ctx, 30, 2);
	    if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
	    if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;

	    /* custom widget pixel width */
	    nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
	    {
	        nk_layout_row_push(ctx, 50);
	        nk_label(ctx, "Volume:", NK_TEXT_LEFT);
	        nk_layout_row_push(ctx, 110);
	        nk_slider_float(ctx, 0, &value, 1.0f, 0.1f);
	    }
	    nk_layout_row_end(ctx);
	}
	nk_end(ctx);
}

static void process_frame(RSGL_renderer* renderer, struct nk_context* ctx, i32 width) {
	test_window(ctx);

	const struct nk_command *cmd_raw = 0;
    nk_foreach(cmd_raw, ctx) {
    	switch (cmd_raw->type) {
    		case NK_COMMAND_TEXT: {
        		const struct nk_command_text *cmd = (const struct nk_command_text*)cmd_raw;
        		// params:
        		// x/y/w/h
        		// float height (font size)
        		// int length
        		// char string
        		// nk_color background
        		// nk_color foreground
        		// nk_user_font* font

        		// TODO: finish implementation
        		RSGL_renderer_setColor(renderer, RSGL_RGBA(cmd->foreground.r, cmd->foreground.g, cmd->foreground.b, cmd->foreground.a));
				RFont_draw_text(renderer_rfont, (RFont_font*)cmd->font, cmd->string, cmd->x, cmd->y, cmd->height);
        	} break;
    		case NK_COMMAND_RECT: {
    			const struct nk_command_rect *cmd = (const struct nk_command_rect*)cmd_raw;
				RSGL_renderer_setColor(renderer, RSGL_RGBA(cmd->color.r, cmd->color.g, cmd->color.b, cmd->color.a));
				RSGL_drawRect(renderer, RSGL_RECT(cmd->x, cmd->y, cmd->w, cmd->h));
        	} break;
	        case NK_COMMAND_IMAGE: {
				// const struct nk_command_image *cmd = (const struct nk_command_image*)cmd_raw;
				// params:
				// x/y/w/h
				// nk_image img
				// nk_color col

				// TODO: finish implementation
	        } break;
        	case NK_COMMAND_SCISSOR: {
        		const struct nk_command_scissor *cmd = (const struct nk_command_scissor*)cmd_raw;
				RSGL_renderer_render(renderer);
				RSGL_renderer_scissorStart(renderer, RSGL_RECT(cmd->x, cmd->y, cmd->w, cmd->h), width);
        	} break;
    	}
    }
}

static float text_width(nk_handle handle, float fontSize, const char *text, int len) {
	RFont_font* font = handle.ptr;

	if (len < 0) { len = 0; }

	u32 w = 0;
	u32 h =0;
	RFont_text_area_len(renderer_rfont, (RFont_font*)font, text, len, fontSize, 0, 0.0f, &w, &h);
	return (float)w;
}

char key_map(i32 key) {
	switch (key) {
		case RGFW_shiftL: return NK_KEY_SHIFT;
		case RGFW_shiftR: return NK_KEY_SHIFT;
		case RGFW_controlL: return NK_KEY_CTRL;
		case RGFW_controlR: return NK_KEY_CTRL;
		case RGFW_return: return NK_KEY_ENTER;
		case RGFW_backSpace: return NK_KEY_BACKSPACE;
		default: break;
	}

	return 0;
};

int main(int argc, char **argv) {
	RGFW_UNUSED(argc); RGFW_UNUSED(argv);
	/* init RGFW window */
	RGFW_window* window = RGFW_createWindow("", 0, 0, 800, 600, RGFW_windowCenter |  RGFW_windowOpenGL);

	RGFW_monitor mon = RGFW_window_getMonitor(window);
	i32 width = window->w;
	i32 height = window->h;
	float pixelRatio = mon.pixelRatio;

	RSGL_renderer* renderer = RSGL_renderer_init(RSGL_GL_rendererProc(), (void*)RGFW_getProcAddress_OpenGL);

	RSGL_renderer_updateSize(renderer, width, height);
	RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, (i32)((float)width * pixelRatio), (i32)((float)height * pixelRatio)));

	RGFW_window_setExitKey(window, RGFW_escape);

	RFont_renderer* renderer_rfont = RFont_RSGL_renderer_init(renderer);
    RFont_font* font = RFont_font_init(renderer_rfont, "COMICSANS.ttf", 60, 500, 500);

	/* init nuklear */
	struct nk_context ctx;
	void *nuklear_memory = calloc(1, MAX_MEMORY);

	struct nk_user_font nk_font;
	nk_font.userdata.ptr = font;
	nk_font.height = 18;
	nk_font.width = text_width;

	nk_init_fixed(&ctx, nuklear_memory, MAX_MEMORY, &nk_font);
    float bg[3] = {255.0f, 255.0f, 255.0f};

	/* main loop */
	RGFW_event event;
	while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
		nk_input_begin(&ctx);
		/* handle RGFW events */
		while (RGFW_window_checkEvent(window, &event)) {
			if (event.type == RGFW_quit) break;

			switch (event.type) {
			case RGFW_keyPressed:
				case RGFW_keyReleased:
					int c = key_map(event.key.value);
					if (c && event.type == RGFW_keyPressed) { nk_input_key(&ctx, c, true); }
					if (c && event.type == RGFW_keyReleased) { nk_input_key(&ctx, c, false); }
					break;
				case RGFW_windowResized:
					width = window->w;
					height = window->h;
					RSGL_renderer_updateSize(renderer, width, height);
					RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, (i32)((float)width * pixelRatio), (i32)((float)height * pixelRatio)));
					break;
			}
		}
		nk_input_end(&ctx);

		RSGL_renderer_clear(renderer, RSGL_RGB(bg[0], bg[1], bg[2]));

		process_frame(renderer, &ctx, width);

		RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(window);

		nk_clear(&ctx);
	}

	nk_free(&ctx);
    free(nuklear_memory);

    RFont_font_free(renderer_rfont, font);
	RFont_RSGL_renderer_free(renderer_rfont);

	RSGL_renderer_free(renderer);
	RGFW_window_close(window);

	return 0;
}
