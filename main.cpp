
#include "RSGL.hpp"
#include "source/linux/deps/stb_truetype.h"

bool running = true;
RSGL::window window("",{500,500,500,500},{0,0,0,500});
bool a =true; bool pressed=false;

char ttf_buffer[1<<25];
char ttf_buffer2[1<<25];

void RSGL::drawText(std::string text, RSGL::circle r, const char* font, RSGL::color col,RSGL::window win=RSGL::root){
    stbtt_fontinfo Font; bool cre=false; 
    FILE* f = fopen(font, "rb");
    if (ttf_buffer != NULL && f != NULL) fread(ttf_buffer, 1, 1<<25,f);
    int high=0;
    for (int dr=0; dr<2; dr++){
        int L=0;
        for (int l=0; l < text.size(); l++){
            int w,h,i,j,c = text.at(l), s = r.radius,b; L++; 
            stbtt_InitFont(&Font, (const unsigned char*)ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char*)ttf_buffer,0));
            unsigned char * bitmap = stbtt_GetCodepointBitmap(&Font, 0,stbtt_ScaleForPixelHeight(&Font, s), c, &w, &h, 0,0);
            if (h > high && !dr) high=h; 
            else if (dr){
                if (h < high && text.size() > 2) b=high-h;
                else b=0;
                glBegin(GL_POINTS);    glColor4f(col.r/255.0, col.g/255.0, col.b/255.0,col.a/255.0);
                for (j=0; j < h; ++j){
                    for (i=0; i < w; ++i) if ( " .:ioVM@"[bitmap[j*w+i]>>5] != ' '){ 
                        //if (!d.GPU){RSGL::drawPoint({i+r.x+(L*s),j+(r.y+b)},col,d);}
                            RSGL::point p1 = {i+r.x+(L*s),j+(r.y+b)};
                            float i = win.r.width/2*1.0f;
                            float  x = (p1.x/i)-1.0f;
                            i = win.r.length/2*1.0f;
                            float  y = (-(p1.y)/i)+1.0f;
                            glVertex2f(x,y);
                    }
                }
                    glEnd();
            }
        }
    }  
    *ttf_buffer=*ttf_buffer2;
}

int main(int argc, char **argv){
    int x = 190;  int frames=0;
    while (running){
        window.checkEvents();
        if (window.event.type == RSGL::MouseButtonPressed) a=true; 
        if (window.event.type == RSGL::MouseButtonReleased) a=false;
        RSGL::drawText("text",{200,200,20},"/usr/share/fonts/SansPosterBold.ttf",{200,0,0});

        // std::cout << window.debug.fps << std::endl;
        if (window.event.type == RSGL::quit) running=false; 
        window.clear();
    } window.close();
} 