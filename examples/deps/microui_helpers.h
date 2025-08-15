#ifndef MICROUI_HELPERS
#define MICROUI_HELPERS

static char logbuf[64000];
static int logbuf_updated = 0;
static float bg[3] = { 90, 95, 100 };

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

static void write_log(const char *text) {
  if (logbuf[0]) { strcat(logbuf, "\n"); }
  strcat(logbuf, text);
  logbuf_updated = 1;
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

static int text_width(mu_Font font, const char *text, int len) {
  RGFW_UNUSED(font);
  if (len == -1) { len = strlen(text); }
  return r_get_text_width(text, len);
}

static int text_height(mu_Font font) {
	RGFW_UNUSED(font);
	return r_get_text_height();
}

mu_Context* rh_init() {
  mu_Context* ctx = (mu_Context*)malloc(sizeof(mu_Context));
  mu_init(ctx);
  ctx->text_width = text_width;
  ctx->text_height = text_height;

  return ctx;
}

#endif
