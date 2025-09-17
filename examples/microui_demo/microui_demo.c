#define RSGL_RFONT
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#define RGFW_DEBUG
#define GL_SILENCE_DEPRECATION
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#include "microui.h"
#include "atlas.inl"

#define RFONT_IMPLEMENTATION
#include "RFont.h"

static  char logbuf[64000];
static   int logbuf_updated = 0;
static float bg[3] = { 90, 95, 100 };


static void write_log(const char *text) {
  if (logbuf[0]) { strcat(logbuf, "\n"); }
  strcat(logbuf, text);
  logbuf_updated = 1;
}


static void test_window(mu_Context *ctx) {
  /* do window */
  if (mu_begin_window(ctx, "Demo Window", mu_rect(40, 40, 300, 450))) {
	mu_Container *win = mu_get_current_container(ctx);
    win->rect.w = mu_max(win->rect.w, 240);
    win->rect.h = mu_max(win->rect.h, 300);

    /* window info */
    if (mu_header(ctx, "Window Info")) {
      mu_Container *win = mu_get_current_container(ctx);
      char buf[64];
      mu_layout_row(ctx, 2, (int[]) { 54, -1 }, 0);
      mu_label(ctx,"Position:");
      sprintf(buf, "%d, %d", win->rect.x, win->rect.y); mu_label(ctx, buf);
      mu_label(ctx, "Size:");
      sprintf(buf, "%d, %d", win->rect.w, win->rect.h); mu_label(ctx, buf);
    }

    /* labels + buttons */
    if (mu_header_ex(ctx, "Test Buttons", MU_OPT_EXPANDED)) {
      mu_layout_row(ctx, 3, (int[]) { 86, -110, -1 }, 0);
      mu_label(ctx, "Test buttons 1:");
      if (mu_button(ctx, "Button 1")) { write_log("Pressed button 1"); }
      if (mu_button(ctx, "Button 2")) { write_log("Pressed button 2"); }
      mu_label(ctx, "Test buttons 2:");
      if (mu_button(ctx, "Button 3")) { write_log("Pressed button 3"); }
      if (mu_button(ctx, "Popup")) { mu_open_popup(ctx, "Test Popup"); }
      if (mu_begin_popup(ctx, "Test Popup")) {
        mu_button(ctx, "Hello");
        mu_button(ctx, "World");
        mu_end_popup(ctx);
      }
    }

    /* tree */
    if (mu_header_ex(ctx, "Tree and Text", MU_OPT_EXPANDED)) {
      mu_layout_row(ctx, 2, (int[]) { 140, -1 }, 0);
      mu_layout_begin_column(ctx);
      if (mu_begin_treenode(ctx, "Test 1")) {
        if (mu_begin_treenode(ctx, "Test 1a")) {
          mu_label(ctx, "Hello");
          mu_label(ctx, "world");
          mu_end_treenode(ctx);
        }
        if (mu_begin_treenode(ctx, "Test 1b")) {
          if (mu_button(ctx, "Button 1")) { write_log("Pressed button 1"); }
          if (mu_button(ctx, "Button 2")) { write_log("Pressed button 2"); }
          mu_end_treenode(ctx);
        }
        mu_end_treenode(ctx);
      }
      if (mu_begin_treenode(ctx, "Test 2")) {
        mu_layout_row(ctx, 2, (int[]) { 54, 54 }, 0);
        if (mu_button(ctx, "Button 3")) { write_log("Pressed button 3"); }
        if (mu_button(ctx, "Button 4")) { write_log("Pressed button 4"); }
        if (mu_button(ctx, "Button 5")) { write_log("Pressed button 5"); }
        if (mu_button(ctx, "Button 6")) { write_log("Pressed button 6"); }
        mu_end_treenode(ctx);
      }
      if (mu_begin_treenode(ctx, "Test 3")) {
        static int checks[3] = { 1, 0, 1 };
        mu_checkbox(ctx, "Checkbox 1", &checks[0]);
        mu_checkbox(ctx, "Checkbox 2", &checks[1]);
        mu_checkbox(ctx, "Checkbox 3", &checks[2]);
        mu_end_treenode(ctx);
      }
      mu_layout_end_column(ctx);

      mu_layout_begin_column(ctx);
      mu_layout_row(ctx, 1, (int[]) { -1 }, 0);
      mu_text(ctx, "Lorem ipsum dolor sit amet, consectetur adipiscing "
        "elit. Maecenas lacinia, sem eu lacinia molestie, mi risus faucibus "
        "ipsum, eu varius magna felis a nulla.");
      mu_layout_end_column(ctx);
    }

    /* background color sliders */
    if (mu_header_ex(ctx, "Background Color", MU_OPT_EXPANDED)) {
      mu_layout_row(ctx, 2, (int[]) { -78, -1 }, 74);
      /* sliders */
      mu_layout_begin_column(ctx);
      mu_layout_row(ctx, 2, (int[]) { 46, -1 }, 0);
      mu_label(ctx, "Red:");   mu_slider(ctx, &bg[0], 0, 255);
      mu_label(ctx, "Green:"); mu_slider(ctx, &bg[1], 0, 255);
      mu_label(ctx, "Blue:");  mu_slider(ctx, &bg[2], 0, 255);
      mu_layout_end_column(ctx);
      /* color preview */
      mu_Rect r = mu_layout_next(ctx);
      mu_draw_rect(ctx, r, mu_color(bg[0], bg[1], bg[2], 255));
      char buf[32];
      sprintf(buf, "#%02X%02X%02X", (int) bg[0], (int) bg[1], (int) bg[2]);
      mu_draw_control_text(ctx, buf, r, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);
    }

    mu_end_window(ctx);
  }
}


static void log_window(mu_Context *ctx) {
  if (mu_begin_window(ctx, "Log Window", mu_rect(350, 40, 300, 200))) {
    /* output text panel */
    mu_layout_row(ctx, 1, (int[]) { -1 }, -25);
    mu_begin_panel(ctx, "Log Output");
    mu_Container *panel = mu_get_current_container(ctx);
    mu_layout_row(ctx, 1, (int[]) { -1 }, -1);
    mu_text(ctx, logbuf);
    mu_end_panel(ctx);
    if (logbuf_updated) {
      panel->scroll.y = panel->content_size.y;
      logbuf_updated = 0;
    }

    /* input textbox + submit button */
    static char buf[128];
    int submitted = 0;
    mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
    if (mu_textbox(ctx, buf, sizeof(buf)) & MU_RES_SUBMIT) {
      mu_set_focus(ctx, ctx->last_id);
      submitted = 1;
    }
    if (mu_button(ctx, "Submit")) { submitted = 1; }
    if (submitted) {
      write_log(buf);
      buf[0] = '\0';
    }

    mu_end_window(ctx);
  }
}


static int uint8_slider(mu_Context *ctx, unsigned char *value, int low, int high) {
  static float tmp;
  mu_push_id(ctx, &value, sizeof(value));
  tmp = *value;
  int res = mu_slider_ex(ctx, &tmp, low, high, 0, "%.0f", MU_OPT_ALIGNCENTER);
  *value = tmp;
  mu_pop_id(ctx);
  return res;
}


static void style_window(mu_Context *ctx) {
  static struct { const char *label; int idx; } colors[] = {
    { "text:",         MU_COLOR_TEXT        },
    { "border:",       MU_COLOR_BORDER      },
    { "windowbg:",     MU_COLOR_WINDOWBG    },
    { "titlebg:",      MU_COLOR_TITLEBG     },
    { "titletext:",    MU_COLOR_TITLETEXT   },
    { "panelbg:",      MU_COLOR_PANELBG     },
    { "button:",       MU_COLOR_BUTTON      },
    { "buttonhover:",  MU_COLOR_BUTTONHOVER },
    { "buttonfocus:",  MU_COLOR_BUTTONFOCUS },
    { "base:",         MU_COLOR_BASE        },
    { "basehover:",    MU_COLOR_BASEHOVER   },
    { "basefocus:",    MU_COLOR_BASEFOCUS   },
    { "scrollbase:",   MU_COLOR_SCROLLBASE  },
    { "scrollthumb:",  MU_COLOR_SCROLLTHUMB },
    { NULL }
  };

  if (mu_begin_window(ctx, "Style Editor", mu_rect(350, 250, 300, 240))) {
    int sw = mu_get_current_container(ctx)->body.w * 0.14;
    mu_layout_row(ctx, 6, (int[]) { 80, sw, sw, sw, sw, -1 }, 0);
    for (int i = 0; colors[i].label; i++) {
      mu_label(ctx, colors[i].label);
      uint8_slider(ctx, &ctx->style->colors[i].r, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].g, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].b, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].a, 0, 255);
      mu_draw_rect(ctx, mu_layout_next(ctx), ctx->style->colors[i]);
    }
    mu_end_window(ctx);
  }
}


static void process_frame(RSGL_renderer* renderer, RFont_renderer* renderer_rfont, mu_Context *ctx, RSGL_texture atlasTexture, i32 width) {
	mu_begin(ctx);
	style_window(ctx);
	log_window(ctx);
	test_window(ctx);
	mu_end(ctx);

	mu_Command *cmd = NULL;
	while (mu_next_command(ctx, &cmd)) {
		switch (cmd->type) {
			case MU_COMMAND_TEXT: {
				RSGL_renderer_setColor(renderer, RSGL_RGBA(cmd->text.color.r, cmd->text.color.g, cmd->text.color.b, cmd->text.color.a));
				RFont_draw_text(renderer_rfont, cmd->text.font, cmd->text.str, cmd->text.pos.x, cmd->text.pos.y, 18);
				break;
			}
			case MU_COMMAND_RECT: {
				mu_Rect src = atlas[ATLAS_WHITE];

				RSGL_renderer_setTextureSource(renderer, atlasTexture, RSGL_RECT(src.x / ATLAS_WIDTH, src.y / ATLAS_HEIGHT, src.w / ATLAS_WIDTH, src.h / ATLAS_HEIGHT));
				RSGL_renderer_setColor(renderer, RSGL_RGBA(cmd->rect.color.r, cmd->rect.color.g, cmd->rect.color.b, cmd->rect.color.a));
				RSGL_drawRect(renderer, RSGL_RECT(cmd->rect.rect.x, cmd->rect.rect.y, cmd->rect.rect.w, cmd->rect.rect.h));
				break;
			}
			case MU_COMMAND_ICON: {
				mu_Rect src = atlas[cmd->icon.id];
				int x = cmd->rect.rect.x + (cmd->rect.rect.w - src.w) / 2;
				int y = cmd->rect.rect.y + (cmd->rect.rect.h - src.h) / 2;

				RSGL_renderer_setTextureSource(renderer, atlasTexture, RSGL_RECT(src.x / ATLAS_WIDTH, src.y / ATLAS_HEIGHT, src.w / ATLAS_WIDTH, src.h / ATLAS_HEIGHT));
				RSGL_renderer_setColor(renderer, RSGL_RGBA(cmd->icon.color.r, cmd->icon.color.g, cmd->icon.color.b, cmd->icon.color.a));
				RSGL_drawRect(renderer, RSGL_RECT(x, y, src.w, src.h));
				break;
			}
			case MU_COMMAND_CLIP:
				RSGL_renderer_render(renderer);
				RSGL_renderer_scissorStart(renderer, RSGL_RECT(cmd->clip.rect.x, cmd->clip.rect.y, cmd->clip.rect.w, cmd->clip.rect.h), width);
				break;
		}
	}
}

static const char button_map[256] = {
  [ RGFW_mouseLeft   & 0xff ] =  MU_MOUSE_LEFT,
  [ RGFW_mouseRight  & 0xff ] =  MU_MOUSE_RIGHT,
  [ RGFW_mouseMiddle & 0xff ] =  MU_MOUSE_MIDDLE,
};

static const char key_map[256] = {
  [ RGFW_shiftL       & 0xff ] = MU_KEY_SHIFT,
  [ RGFW_shiftR       & 0xff ] = MU_KEY_SHIFT,
  [ RGFW_controlL        & 0xff ] = MU_KEY_CTRL,
  [ RGFW_controlR        & 0xff ] = MU_KEY_CTRL,
  [ RGFW_altL         & 0xff ] = MU_KEY_ALT,
  [ RGFW_altR         & 0xff ] = MU_KEY_ALT,
  [ RGFW_return       & 0xff ] = MU_KEY_RETURN,
  [ RGFW_backSpace    & 0xff ] = MU_KEY_BACKSPACE,
};

RFont_renderer* stackRenderer;

static int text_width(mu_Font font, const char *text, int len) {
	RGFW_UNUSED(font);
	if (len == -1) { len = 0; }

	i32 w, h;
	RFont_text_area_len(stackRenderer, font, text, len, 18, 0, 0.0f, &w, &h);
	return w;
}

static int text_height(mu_Font font) {
	RGFW_UNUSED(font);
	return 18;
}


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

	RSGL_textureBlob blob;
	RSGL_MEMSET(&blob, 0, sizeof(blob));
	blob.data = atlas_texture;
	blob.width = ATLAS_WIDTH;
	blob.height = ATLAS_WIDTH;
	blob.dataType = RSGL_textureDataInt;
	blob.dataFormat = RSGL_formatGrayscaleAlpha;
	blob.textureFormat = blob.dataFormat;
	RSGL_texture atlasTexture = RSGL_renderer_createTexture(renderer, &blob);

	RGFW_window_setExitKey(window, RGFW_escape);

	RFont_renderer* renderer_rfont = RFont_RSGL_renderer_init(renderer);
    RFont_font* font = RFont_font_init(renderer_rfont, "COMICSANS.ttf", 60, 500, 500);

	stackRenderer = renderer_rfont;

	/* init microui */
	mu_Context *ctx = (mu_Context*)malloc(sizeof(mu_Context));
	mu_init(ctx);
	ctx->text_width = text_width;
	ctx->text_height = text_height;
	ctx->style->font = font;

	/* main loop */
	while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
		/* handle RGFW events */
		RGFW_event event;
		while (RGFW_window_checkEvent(window, &event)) {
			if (event.type == RGFW_quit) break;

			switch (event.type) {
				case RGFW_quit: break;
				case RGFW_mousePosChanged: mu_input_mousemove(ctx, event.mouse.x,  event.mouse.y); break;

				case RGFW_mouseScroll:
					mu_input_scroll(ctx, event.scroll.x, event.scroll.y);
					break;
				case RGFW_mouseButtonPressed: {
					i32 x, y;
					RGFW_window_getMouse(window, &x, &y);

					int b = button_map[event.button.value & 0xff];
					if (b) mu_input_mousedown(ctx, x, y, b);
					break;
				}
				case RGFW_mouseButtonReleased: {
					i32 x, y;
					RGFW_window_getMouse(window, &x, &y);
					int b = button_map[event.button.value & 0xff];
					if (b) { mu_input_mouseup(ctx, x, y, b);   }
					break;
				}

				case RGFW_keyPressed: {
					char str[2] = {(char)event.key.sym, '\0'};
					mu_input_text(ctx, str);
				}
				case RGFW_keyReleased: {
					int c = key_map[event.key.value & 0xff];
					if (c && event.type == RGFW_keyPressed) { mu_input_keydown(ctx, c); }
					if (c && event.type == RGFW_keyReleased) { mu_input_keyup(ctx, c);   }
					break;
				}

				case RGFW_windowResized:
					width = window->w;
					height = window->h;
					RSGL_renderer_updateSize(renderer, width, height);
					RSGL_renderer_viewport(renderer, RSGL_RECT(0, 0, (i32)((float)width * pixelRatio), (i32)((float)height * pixelRatio)));
					break;
			}
		}

		RSGL_renderer_clear(renderer, RSGL_RGB(bg[0], bg[1], bg[2]));

		/* process frame */
		process_frame(renderer, renderer_rfont, ctx, atlasTexture, width);

		RSGL_renderer_render(renderer);
		RGFW_window_swapBuffers_OpenGL(window);
	}

	RSGL_renderer_deleteTexture(renderer, atlasTexture);
    RFont_font_free(renderer_rfont, font);
	RFont_RSGL_renderer_free(renderer_rfont);

	RSGL_renderer_free(renderer);
	RGFW_window_close(window);

	return 0;
}


