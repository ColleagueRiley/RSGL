#ifndef RSGL
#include "../../include/include/linux/rsgl.hpp" 
#endif
#define STB_TRUETYPE_IMPLEMENTATION 
#include "deps/stb_truetype.h" // for loading fonts

char ttf_buffer[1<<25];
char ttf_buffer2[1<<25];

void RSGL::drawText(std::string text, RSGL::circle r, const char* font, RSGL::color col, RSGL::drawable d){
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
                if (d.GPU==1){ glBegin(GL_POINTS);    glColor4f(col.r/255.0, col.g/255.0, col.b/255.0,col.a/255.0);}
                for (j=0; j < h; ++j){
                    for (i=0; i < w; ++i) if ( " .:ioVM@"[bitmap[j*w+i]>>5] != ' '){ 
                        if (!d.GPU){RSGL::drawPoint({i+r.x+(L*s),j+(r.y+b)},col,d);}
                        else if (d.GPU == 1){ 
                            RSGL::point p1 = {i+r.x+(L*s),j+(r.y+b)};
                            float i = d.r.width/2*1.0f;
                            float  x = (p1.x/i)-1.0f;
                            i = d.r.length/2*1.0f;
                            float  y = (-(p1.y)/i)+1.0f;
                            glVertex2f(x,y);
                        }
                    }
                }
                if (d.GPU==1){
                    glEnd();
                    glFlush();
                }
            }
        }
    }  
    *ttf_buffer=*ttf_buffer2;
}

RSGL::Text::Text(std::string txt, RSGL::circle r, const char* font, RSGL::color col, bool draw, RSGL::drawable d){rect=r; text=txt; c=col; f=font; if (draw) RSGL::drawText(txt,r,font,col,d);}

void RSGL::Text::draw(){ RSGL::drawText(text,rect,f,c,d); }


void DrawDottedLine(RSGL::rect r,RSGL::color c,RSGL::drawable win=RSGL::root,bool t=false) {
    if (t){
        DrawDottedLine({r.x,r.y,r.x+r.width,r.y},c,win);
        DrawDottedLine({r.x,r.y,r.x,r.y+r.length},c,win);
        DrawDottedLine({r.x+r.width,r.y,r.x+r.width,r.y+r.length},c,win);
        
        DrawDottedLine({r.x,r.y+r.length,r.x+r.width,r.y+r.length},c,win);
    }   
    else{
    int x0=r.x, y0=r.y, x1=r.width, y1=r.length;
    int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2;
    int count = 0;
    glBegin(GL_POINTS);
    while (1) {
        if (count < 10) {
            float i = win.r.width/2*1.0f;
            float  x2 = (x0/i)-1.0f;
            i = win.r.length/2*1.0f;
            float  y2 = (-(y0)/i)+1.0f;
            glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
            glVertex2f(x2,  y2);
        }
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
        count = (count + 1) % 20;
    }
    glEnd();
    glFlush();
    }
}

void RSGL::drawRect(RSGL::rect r,color c, bool fill,bool dotted, RSGL::drawable win){
    if (!win.GPU){
        XSetForeground(win.display,XDefaultGC(win.display,XDefaultScreen(win.display)),
        RSGLRGBTOHEX(c.r,c.g,c.b));
        if (fill) XFillRectangle(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)),r.x,r.y,r.width,r.length);
        else{
            RSGL::drawLine({r.x,r.y},{r.x,r.y+r.length},c);
            RSGL::drawLine({r.x+r.width,r.y},{r.x+r.width,r.y+r.length},c);
            RSGL::drawLine({r.x,r.x},{r.x+r.width,r.y},c);
            RSGL::drawLine({r.x,r.y+r.length},{r.x+r.width,r.y+r.length},c);

        }        
    }
    else if (win.GPU == 1){
        float i = win.r.width/2*1.0f;
        float  x = (r.x/i)-1.0f;
        float  x2 =((r.x+r.width)/i)-1.0f;
        i = win.r.length/2*1.0f;
        float  y = (-(r.y)/i)+1.0f;
        float  y2 =(-(r.y+r.length)/i)+1.0f;
        if (dotted){DrawDottedLine(r,c,win,true);}
        else{
        GLenum m=GL_POLYGON; if (!fill) m=GL_LINE_STRIP;
        glBegin(m);
        glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
        glVertex2f(x,  y);
        glVertex2f(x2, y);
        glVertex2f(x2, y2);
        glVertex2f(x, y2);
        if (!fill) glVertex2f(x,  y);
        glEnd();
        glFlush();
        }
    }
}

void RSGL::drawLine(RSGL::point p1, RSGL::point p2, RSGL::color c, RSGL::drawable win){
    if (!win.GPU){
        XSetForeground(win.display,XDefaultGC(win.display,XDefaultScreen(win.display)),
        RSGLRGBTOHEX(c.r,c.g,c.b));
        XDrawLine(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)),p1.x,p1.y,p2.x,p2.y);
    }
    else if (win.GPU == 1){
        float i = win.r.width/2*1.0f;
        float  x = (p1.x/i)-1.0f;
        float  x2 =((p2.x)/i)-1.0f;
        i = win.r.length/2*1.0f;
        float  y = (-(p1.y)/i)+1.0f;
        float  y2 =(-(p2.y)/i)+1.0f;
        glBegin(GL_LINE_STRIP);
        glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
        glVertex2f(x,  y);
        glVertex2f(x2, y2);
        glEnd();
        glFlush();
    }
}

int RSGL::drawCircle(RSGL::circle c, color col,bool fill,RSGL::drawable win){
    if (!win.GPU){
        XSetForeground(win.display,XDefaultGC(win.display,XDefaultScreen(win.display)),RSGLRGBTOHEX(col.r,col.g,col.b));
        if (fill){XFillArc(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)), c.x-(30/2), c.y-(30/2), c.radius, c.radius, 0, 360*64);}
        else XDrawArc(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)), c.x-(30/2), c.y-(30/2), c.radius, c.radius, 0, 360*64);
        return 1;
    }
    else if (win.GPU == 1){
        float i = win.r.width/2*1.0f;
        float  x2 = (c.x/i)-1.0f;
        i = win.r.length/2*1.0f;
        float  y2 = (-(c.y)/i)+1.0f;

        i = win.r.width/2*1.0f;
        float  r = ((-(c.radius-258)/i)-1.0f);
        if (!fill){
        glBegin(GL_LINE_LOOP);
                glColor4f(col.r/255.0, col.g/255.0, col.b/255.0,col.a/255.0);
                for(int ii = 0; ii < 60; ii++){
                    float theta = 2.0f * 3.1415926f * float(ii) / float(60);//get the current angle

                    float x = r * cosf(theta);//calculate the x component
                    float y = r * sinf(theta);//calculate the y component

                    glVertex2f(x + x2, y + y2);//output vertex

                }
        glEnd();
        return 1;
        }
        if (fill){
        glBegin(GL_TRIANGLE_FAN);
             glColor4f(col.r/255.0,col.g/255.0,col.b/255.0,col.a/255.0);
            glVertex2f(x2,y2);

            for (float angle=1.0f;angle<361.0f;angle+=0.2){
                float x3 = x2+sin(angle)*r;
                float y3 = y2+cos(angle)*r;
                glVertex2f(x3,y3);
            }   
        glEnd();
        }
  }
  return 1;
}


int RSGL::drawPoint(RSGL::point p, color c, RSGL::drawable win){  RSGL::drawRect({p.x,p.y,1,1,},c,true,false,win); return 1; }

std::vector<std::vector<RSGL::color>> RSGL::resizeImage(std::vector<std::vector<RSGL::color>> values, RSGL::rect r,RSGL::rect og_r){
    std::vector<std::vector<RSGL::color>> output;
    int xx, yy;
    for (int y=0; y < r.length; y++){ 
        output.insert(output.end(),{{}});
        yy = (og_r.length/r.length)*y;
        for (int x=0; x < r.width; x++){
            xx = (og_r.width/r.width)*x;
            output.at(output.size()-1).insert(output.at(output.size()-1).end(),{values[yy][xx].r,values[yy][xx].g,values[yy][xx].b, values[yy][xx].a});
        }
    }
    
    return output;
}

void RSGL::drawImage(std::string fileName, RSGL::rect r,bool resize,RSGL::drawable d){
    png::image< png::rgba_pixel > image(fileName);
    std::vector<std::vector<RSGL::color>> img;
    for (png::uint_16 y=0; y < image.get_height(); y++){  
        img.insert(img.end(),{{}}); 
        for (png::uint_16 x=0; x < image.get_width(); x++) img.at(img.size()-1).insert(img.at(img.size()-1).end(),{image[y][x].red,image[y][x].green,image[y][x].blue,(float)image[y][x].alpha});
    }
    if (image.get_width() != r.width && image.get_height() != r.length && resize) img = RSGL::resizeImage(img,r,{0,0,(int)image.get_width(),(int)image.get_height()});
    glBegin(GL_POINTS);        
    for (int y3=0; y3 < img.size(); y3++){
        for (int x3=0; x3 < img.at(y3).size(); x3++){
            if(img.at(y3).at(x3).a >= 100){
                RSGL::color c = img.at(y3).at(x3);
                float i = d.r.width/2*1.0f;
                float  x = (r.x+x3/i)-1.0f;
                float  x2 =((r.x+x3+1)/i)-1.0f;
                i = d.r.length/2*1.0f;
                float  y = (-(r.y+y3)/i)+1.0f;
                float  y2 =(-(r.y+y3+1)/i)+1.0f;
                glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
                glVertex2f(x,  y);
                glVertex2f(x2, y);
                glVertex2f(x2, y2);
                glVertex2f(x, y2);
                
            }
        }
    }
    glEnd();
    glFlush();
}

