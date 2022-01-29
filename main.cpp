#define  GL_GLEXT_PROTOTYPES 1
#include "RSGL.hpp"
bool running = true;
RSGL::window window("",{500,500,500,500},{200,0,0,500});


void drawRect(RSGL::rect r,RSGL::color c, bool fill=true, RSGL::drawable win=RSGL::root){
    if (win.GPU == 1){
        float i = win.r.width/2*1.0f;
        float  x = (r.x/i)-1.0f;
        float  x2 =((r.x+r.width)/i)-1.0f;
        i = win.r.length/2*1.0f;
        float  y = (-(r.y)/i)+1.0f;
        float  y2 =(-(r.y+r.length)/i)+1.0f;
        GLenum m; if (fill) m=GL_POLYGON; else m=GL_LINE_STRIP;
        glBegin(m);
        glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
        glVertex2f(x,  y);
        glVertex2f(x2, y);
        glVertex2f(x2, y2);
        glVertex2f(x, y2);
        if (!fill) glVertex2f(x,  y);
        glEnd();
        //glFlush();
    }
}
int main(int argc, char **argv){
    int x = 190;  int frames=0;
    while (running){
        window.checkEvents(); window.clear();
        std::cout << window.debug.fps << std::endl;
        if (window.event.type == RSGL::quit) running=false;
        if (window.event.type == RSGL::KeyPressed) x+=5;
        //RSGL::drawCircle({200,200,20},{200,0,0});
        RSGL::drawRect({window.event.x,window.event.y,20,20},{0,0,200,.a=0});
    
    } window.close();
} 