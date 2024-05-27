#define RSGL_IMPLEMENTATION
#define GL_NO_RENDER
#define RSGL_CUSTOM_DRAW
#define RFONT_RENDER_LEGACY
#define GL_LOAD

#include "RSGL.h"

void RSGL_basicDraw(u32 GL_TYPE, float* points, float* texPoints, RSGL_color c, size_t len) {  
    if (GL_TYPE > 0x0010)
        GL_TYPE -= 0x0010;
    
    u32 i;
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (RSGL_args.blend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, RSGL_args.texture);
    glLineWidth(RSGL_args.lineWidth);

    glColor4ub(c.r, c.g, c.b, c.a);

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

    if (RSGL_argsClear) {
        RSGL_setTexture(0);
        RSGL_clearArgs();
    }
}

int main(void) {
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

    RSGL_button toggle = RSGL_initButton();
    
    /* this can be a rect or polygon */
    RSGL_button_setPolygon(&toggle, RSGL_RECT(250, 50, 100, 50), 36);    
    RSGL_button_setStyle(&toggle, RSGL_STYLE_DARK | RSGL_STYLE_TOGGLE | RSGL_STYLE_ROUNDED);

    bool running = true;

    while (running) {
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RSGL_quit) {
                running = false;
                break;
            }

            RSGL_button_update(&generic, win->event);
            RSGL_button_update(&toggle, win->event);
            RSGL_textbox_update(tb, win->event);
        }

        RSGL_setFont(easy_font);
        RSGL_drawButton(generic);

        RSGL_drawButton(toggle);

        RSGL_setFont(comicSans);
        RSGL_textbox_draw(tb);

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
        
        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_textbox_free(tb);

    RSGL_window_close(win);
}