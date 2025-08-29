#include <assert.h>
#include "microui.h"
#include "atlas.inl"

#define BUFFER_SIZE 16384

static int width  = 800;
static int height = 600;

void r_init(RGFW_window* win) {
  RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress, RSGL_GL_renderer());

  /* init gl */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
}

void r_set_clip_rect(mu_Rect rect) {
  RSGL_draw();
  glScissor(rect.x, height - (rect.y + rect.h), rect.w, rect.h);
}

void r_push_quad(mu_Rect dst, mu_Rect src, mu_Color color) {
  float u0 = src.x / ATLAS_WIDTH;
  float v0 = src.y / ATLAS_HEIGHT;
  float u1 = (src.x + src.w) / ATLAS_WIDTH;
  float v1 = (src.y + src.h) / ATLAS_HEIGHT;

  float texPoints[12];
  texPoints[0] = u0;   texPoints[1] = v1;
  texPoints[2] = u0;   texPoints[3] = v0;
  texPoints[4] = u1;   texPoints[5] = v1;

  texPoints[6] = u1;   texPoints[7] = v0;
  texPoints[8] = u1;   texPoints[9] = v1;
  texPoints[10] = u0;  texPoints[11] = v0;

  RSGL_color c;
  c.r = color.r;
  c.g = color.g;
  c.b = color.b;
  c.a = color.a;

  RSGL_point3D center = (RSGL_point3D){ dst.x + (dst.w * 0.5f), dst.y + (dst.h * 0.5f), 0.0f};
  RSGL_mat4 matrix = RSGL_initDrawMatrix(center);

  float points[] = {
      RSGL_GET_MATRIX_POINT(dst.x, dst.y, 0.0f),
      RSGL_GET_MATRIX_POINT(dst.x, dst.y + dst.h, 0.0f),
      RSGL_GET_MATRIX_POINT(dst.x + dst.w, dst.y, 0.0f),

      RSGL_GET_MATRIX_POINT(dst.x + dst.w, dst.y + dst.h, 0.0f),
      RSGL_GET_MATRIX_POINT(dst.x + dst.w, dst.y, 0.0f),
      RSGL_GET_MATRIX_POINT(dst.x, dst.y + dst.h, 0.0f),
  };

  RSGL_basicDraw(RSGL_TRIANGLES, (float*)points, (float*)texPoints, c, 6);
}

void r_draw_rect(mu_Rect rect, mu_Color color, u32 textureId) {
  RSGL_setTexture(textureId);
  RSGL_drawRect(RSGL_RECT(rect.x, rect.y, rect.w, rect.h), RSGL_RGB(color.r, color.g, color.b));
}

void r_draw_text(u32 textureId, const char *text, mu_Vec2 pos, mu_Color color) {
  RSGL_setTexture(textureId);

  mu_Rect dst = { pos.x, pos.y, 0, 0 };
  for (const char *p = text; *p; p++) {
    if ((*p & 0xc0) == 0x80) { continue; }
    int chr = mu_min((unsigned char) *p, 127);
    mu_Rect src = atlas[ATLAS_FONT + chr];
    dst.w = src.w;
    dst.h = src.h;
    r_push_quad(dst, src, color);
    dst.x += dst.w;
  }
}

void r_draw_icon(u32 textureId, int iconId, mu_Rect rect, mu_Color color) {
  iconId = MU_ICON_CLOSE;
  mu_Rect src = atlas[iconId];
  int x = rect.x + (rect.w - src.w) / 2;
  int y = rect.y + (rect.h - src.h) / 2;

  RSGL_setTexture(textureId);
  r_push_quad(mu_rect(x, y, src.w, src.h), src, color);
}

int r_get_text_width(const char *text, int len) {
  return RSGL_textArea(text, 18, 0).w;
}

int r_get_text_height() {
  return 18;
}

void r_clear(mu_Color color) {
  RSGL_draw();
  RSGL_clear(RSGL_RGB(color.r, color.g, color.b));
}
