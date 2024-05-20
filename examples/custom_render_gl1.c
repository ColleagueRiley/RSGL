#define RSGL_NO_AUDIO /* RSGL uses miniaudio.h, and I don't want to compile it if I'm not using it */
#define RSGL_IMPLEMENTATION
#define GL_NO_RENDER
#define RSGL_CUSTOM_DRAW
#define RFONT_RENDER_LEGACY
#define GL_LOAD

#include "RSGL.h"

void RSGL_basicDraw(u32 GL_TYPE, float* points, float* texPoints, RSGL_point3DF center, RSGL_color c, size_t len) {  
    if (GL_TYPE > 0x0010)
        GL_TYPE -= 0x0010;
    
    i32 i;
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!center.x && !center.y)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, RSGL_args.texture);
    glLineWidth(RSGL_args.lineWidth);

    glColor4ub(c.r, c.g, c.b, c.a);

    if (RSGL_args.rotate.x || RSGL_args.rotate.y || RSGL_args.rotate.z) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        if (RSGL_args.center.x != -1 && RSGL_args.center.y != -1 &&  RSGL_args.center.z != -1)
            center = RSGL_args.center;

        glTranslatef(center.x, center.y, center.z);
        glRotatef(RSGL_args.rotate.z,  0, 0, 1);
        glRotatef(RSGL_args.rotate.y, 0, 1, 0);
        glRotatef(RSGL_args.rotate.x, 1, 0, 0);
        glTranslatef(-center.x, -center.y, -center.z);
    }

    glBegin(GL_TYPE);
        size_t pIndex = 0;
        size_t tIndex = 0;

        for (i = 0; i < len; i++) {
            if (i && i <= RSGL_args.gradient_len)
                glColor4ub(RSGL_args.gradient[i - 1].r, RSGL_args.gradient[i - 1].g, RSGL_args.gradient[i - 1].b, RSGL_args.gradient[i - 1].a);
            
            glTexCoord2f(texPoints[tIndex], texPoints[tIndex + 1]);
            glVertex3f(points[pIndex + 0], points[pIndex + 1], points[pIndex + 2]);

            pIndex += 3;
            tIndex += 2;
        }
    glEnd();

    if (RSGL_args.rotate.x || RSGL_args.rotate.y || RSGL_args.rotate.z)
        glPopMatrix();

    if (RSGL_argsClear) {
        RSGL_setTexture(0);
        RSGL_clearArgs();
    }
}

int main() {
    RSGL_window* win = RSGL_createWindow("name", RSGL_RECT(0, 0, 500, 500), RSGL_CENTER);


    u32 easy_font = RSGL_loadFont("Super Easy.ttf");
    RSGL_setFont(easy_font);

    /* generic button */
    RSGL_button generic = RSGL_initButton();
    
    /* this can be a rect or polygon */
    RSGL_button_setRect(&generic, RSGL_RECT(50, 50, 100, 50));
    
    RSGL_button_setText(&generic, "generic", 8, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(100, 100, 100));
    RSGL_button_alignText(&generic, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);
    
    RSGL_button_setStyle(&generic, RSGL_STYLE_DARK | RSGL_STYLE_ROUNDED);

    RSGL_button_setWindow(&generic, win);

    u32 comicSans = RSGL_loadFont("COMICSANS.ttf");

    RSGL_textbox* tb = RSGL_initTextbox(0);
    RSGL_textbox_setTextInfo(tb, RSGL_CIRCLE(0, 0, 20), RSGL_RGB(100, 100, 100));
    
    RSGL_textbox_setRect(tb, RSGL_RECT(20, 20, 200, 30));
    RSGL_textbox_setStyle(tb, RSGL_STYLE_DARK);
    RSGL_textbox_alignText(tb, RSGL_ALIGN_CENTER | RSGL_ALIGN_MIDDLE);

    bool running = true;

    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            RSGL_button_update(&generic, win->event);
            RSGL_textbox_update(tb, win->event);
        }

        RSGL_setFont(easy_font);
        RSGL_drawButton(generic);

        RSGL_setFont(comicSans);
        RSGL_textbox_draw(tb);

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
        
        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_textbox_free(tb);

    RSGL_window_close(win);
}