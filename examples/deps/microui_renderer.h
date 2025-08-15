#include <assert.h>
#include "microui.h"
#include "atlas.inl"

#define BUFFER_SIZE 16384

static GLfloat   tex_buf[BUFFER_SIZE * 8];
static GLfloat  vert_buf[BUFFER_SIZE * 8];
static GLubyte color_buf[BUFFER_SIZE * 16];
static GLuint  index_buf[BUFFER_SIZE *  6];

static int width  = 800;
static int height = 600;
static int buf_idx;

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

  // printf("%d\n", atlas_texture[0]);
  // /* init texture */
  GLuint id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ATLAS_WIDTH, ATLAS_HEIGHT, 0,
    GL_ALPHA, GL_UNSIGNED_BYTE, atlas_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  assert(glGetError() == 0);
}

void r_flush() {
  RSGL_draw();

  buf_idx = 0;
}

void r_set_clip_rect(mu_Rect rect) {
  r_flush();
  glScissor(rect.x, height - (rect.y + rect.h), rect.w, rect.h);
}

void push_quad(mu_Rect dst, mu_Rect src, mu_Color color) {
  if (buf_idx == BUFFER_SIZE) { r_flush(); }

  int texvert_idx = buf_idx *  8;
  int   color_idx = buf_idx * 16;
  int element_idx = buf_idx *  4;
  int   index_idx = buf_idx *  6;
  buf_idx++;

  /* update texture buffer */
  float x = src.x / (float) ATLAS_WIDTH;
  float y = src.y / (float) ATLAS_HEIGHT;
  float w = src.w / (float) ATLAS_WIDTH;
  float h = src.h / (float) ATLAS_HEIGHT;
  tex_buf[texvert_idx + 0] = x;
  tex_buf[texvert_idx + 1] = y;
  tex_buf[texvert_idx + 2] = x + w;
  tex_buf[texvert_idx + 3] = y;
  tex_buf[texvert_idx + 4] = x;
  tex_buf[texvert_idx + 5] = y + h;
  tex_buf[texvert_idx + 6] = x + w;
  tex_buf[texvert_idx + 7] = y + h;

  /* update vertex buffer */
  vert_buf[texvert_idx + 0] = dst.x;
  vert_buf[texvert_idx + 1] = dst.y;
  vert_buf[texvert_idx + 2] = dst.x + dst.w;
  vert_buf[texvert_idx + 3] = dst.y;
  vert_buf[texvert_idx + 4] = dst.x;
  vert_buf[texvert_idx + 5] = dst.y + dst.h;
  vert_buf[texvert_idx + 6] = dst.x + dst.w;
  vert_buf[texvert_idx + 7] = dst.y + dst.h;

  /* update color buffer */
  memcpy(color_buf + color_idx +  0, &color, 4);
  memcpy(color_buf + color_idx +  4, &color, 4);
  memcpy(color_buf + color_idx +  8, &color, 4);
  memcpy(color_buf + color_idx + 12, &color, 4);

  /* update index buffer */
  index_buf[index_idx + 0] = element_idx + 0;
  index_buf[index_idx + 1] = element_idx + 1;
  index_buf[index_idx + 2] = element_idx + 2;
  index_buf[index_idx + 3] = element_idx + 2;
  index_buf[index_idx + 4] = element_idx + 3;
  index_buf[index_idx + 5] = element_idx + 1;
}

void r_draw_rect(mu_Rect rect, mu_Color color, u32 textureId) {
  RSGL_setTexture(textureId);
  RSGL_drawRect(RSGL_RECT(rect.x, rect.y, rect.w, rect.h), RSGL_RGB(color.r, color.g, color.b));
}

void r_draw_text(RFont_font* font, const char *text, mu_Vec2 pos, mu_Color color) {
  RSGL_setFont(font);
  RSGL_drawText(text, RSGL_CIRCLE(pos.x, pos.y, 18), RSGL_RGB(color.r, color.g, color.b));
}

void r_draw_icon(u32 textureId, int iconId, mu_Rect rect, mu_Color color) {

}

int r_get_text_width(const char *text, int len) {
  return RSGL_textArea(text, 18, 0).w;
}


int r_get_text_height() {
  return 18;
}

void r_clear(mu_Color color) {
  r_flush();
  RSGL_clear(RSGL_RGB(color.r, color.g, color.b));
}
