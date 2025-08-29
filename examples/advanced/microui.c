#undef _WIN32_WINNT
#include <stdio.h>

// #define RGFW_DEBUG
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

// #define RSGL_DEBUG
#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#include "microui.h" /* microui header */
#include "microui_renderer.h" /* render commands to RSGL */
#include "microui_helpers.h" /* not to be used directly */
#include "microui_windows.h" /* window gui setup */

int main() {
  RGFW_window* window = RGFW_createWindow("", RGFW_RECT(0, 0, width, height), RGFW_windowCenter | RGFW_windowScaleToMonitor);
  r_init(window);

  u32 atlas_texture_id = RSGL_renderCreateTexture(atlas_texture, RSGL_AREA(ATLAS_WIDTH, ATLAS_HEIGHT), 1);

  /* init microui */
  mu_Context *ctx = rh_init();

  /* main loop */
  RGFW_event *event;
  while (!RGFW_window_shouldClose(window)) {
    while ((event = RGFW_window_checkEvent(window)) != NULL) {
      if (event->type == RGFW_quit) break;

      switch (event->type) {
        case RGFW_quit: break;
        case RGFW_mousePosChanged: mu_input_mousemove(ctx, event->point.x,  event->point.y); break;
        case RGFW_mouseButtonPressed: {
          mu_input_scroll(ctx, 0, window->event.scroll * -30);
          int b = button_map[event->button & 0xff];
          if (b) { mu_input_mousedown(ctx, event->point.x,  event->point.y , b); }
        } break;
        case RGFW_mouseButtonReleased: {
          int b = button_map[event->button & 0xff];
          if (b) { mu_input_mouseup(ctx, event->point.x,  event->point.y, b); }
        } break;
        case RGFW_keyPressed: {
          char str[2] = {(char)event->key, '\0'};
          mu_input_text(ctx, str);
          int c = key_map[event->key & 0xff];
          if (c) { mu_input_keydown(ctx, c); }
        } break;
        case RGFW_keyReleased: {
          int c = key_map[event->key & 0xff];
          if (c) { mu_input_keyup(ctx, c); }
        } break;
        case RGFW_windowResized:
          width = window->r.w;
          height = window->r.h;
        } break;
    }

    /* process microui frame */
    process_frame(ctx);

    /* render */
    r_clear(mu_color(bg[0], bg[1], bg[2], 255));
    mu_Command *cmd = NULL;
    while (mu_next_command(ctx, &cmd)) {
      switch (cmd->type) {
        case MU_COMMAND_TEXT: r_draw_text(atlas_texture_id, cmd->text.str, cmd->text.pos, cmd->text.color); break;
        case MU_COMMAND_RECT: r_draw_rect(cmd->rect.rect, cmd->rect.color, 0); break;
        case MU_COMMAND_ICON: r_draw_icon(atlas_texture_id, cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
        case MU_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
      }
    }

    /* swap buffers */
    RSGL_draw();
    RGFW_window_swapBuffers(window);
  }

  /* free memory */
  RSGL_renderDeleteTexture(atlas_texture_id);
  RSGL_free();

  RGFW_window_close(window);

  return 0;
}
