#ifndef RSGL
#include "../../include/include/linux/rsgl.hpp" 
#endif
#define STB_TRUETYPE_IMPLEMENTATION 
#include "deps/stb_truetype.h" // for loading fonts

char ttf_buffer[1<<25];
char ttf_buffer2[1<<25];

void drawTextRAW(std::string text, RSGL::circle r, const char* Font, RSGL::color col, RSGL::drawable d){
  int high=0;
   if (d.GPU == 1){ glBegin(GL_POINTS); glColor4f(col.r/255.0, col.g/255.0, col.b/255.0,col.a/255.0);}
   for (int dr=0; dr<2; dr++){
        int L2=0; 
        for (int L=0; L < text.size(); L++){
            stbtt_fontinfo font;
            unsigned char *bitmap;
            int w,h,i,j,c = text.at(L), s = r.radius; L2++;
            FILE* f = fopen(Font, "rb");

            if (ttf_buffer != NULL && f != NULL) fread(ttf_buffer, 1, 1<<25, f);

            stbtt_InitFont(&font, (unsigned char*)ttf_buffer, stbtt_GetFontOffsetForIndex((unsigned char*)ttf_buffer,0));
            bitmap = stbtt_GetCodepointBitmap(&font, 0,stbtt_ScaleForPixelHeight(&font, s), c, &w, &h, 0,0);
            if (h > high && !dr) high=h; 
            else if (dr){
                int b=0;
                if (h < high && text.size() > 2) b=high-h;
                if (text.at(L) == ' ') r.x+=10;
                for (j=0; j < h; ++j) {
                    for (i=0; i < w; ++i)
                        if (" .:ioVM@"[bitmap[j*w+i]>>5] != ' '){ 
                            RSGL::point p1 = {r.x+i,r.y+j+b};
                            float i = d.r.width/2*1.0f;
                            float  x = (p1.x/i)-1.0f;
                            i = d.r.length/2*1.0f;
                            float  y = (-(p1.y)/i)+1.0f;
                            if (!d.GPU) RSGL::drawPoint(p1,col);
                            else if (d.GPU == 1) glVertex2f(x,y);
                        }
                }  //r.x+=w + bias.at(text.at(L));
                b=0; int w2=w;
                if (L+1 < text.size()) bitmap = stbtt_GetCodepointBitmap(&font, 0,stbtt_ScaleForPixelHeight(&font, s), text.at(L+1), &w2, &h, 0,0);
                if (w >= 20 || w2 >= 20 ) b++; 
                if (w >= 32 || w2 >= 32) b++;  
                r.x+=w + b; 
            } 
        }
   } if (d.GPU == 1){glEnd(); glFlush();} *ttf_buffer=*ttf_buffer2;
}


struct Letter{
    std::string letter;
    int size; int num;
    std::string text;
    const char* font;
};

std::vector<Letter> ls;
std::vector<RSGL::point> points;

void RSGL::drawText(std::string text, RSGL::circle r, const char* Font, RSGL::color col,RSGL::drawable win){
    int x; int x2=0; int y;
    std::string textStr = text;
    for (int I3=0; I3 < textStr.size(); I3++){
        text = textStr.at(I3);
        bool done=false; int L3=0;
        for (L3; L3 < ls.size(); L3++){
            if (text == ls.at(L3).letter && r.radius == ls.at(L3).size && Font == ls.at(L3).font && ls.at(L3).num == I3 && ls.at(L3).text == textStr){ done=true; break;} 
        }
        if (!done){
            if (textStr.size() > I3+1) text += textStr.at(I3+1);
            /* load font file */
            long size;
            unsigned char* fontBuffer;

            FILE* fontFile = fopen(Font, "rb");
            fseek(fontFile, 0, SEEK_END);
            size = ftell(fontFile); /* how long is the file ? */
            fseek(fontFile, 0, SEEK_SET); /* reset */
            fontBuffer = (unsigned char*)malloc(size);

            fread(fontBuffer, size, 1, fontFile);

            /* prepare font */
            stbtt_fontinfo info;
            if (!stbtt_InitFont(&info, fontBuffer, 0)) printf("failed\n");


            int b_w = r.radius*text.size(); /* bitmap width */
            int b_h = r.radius; /* bitmap height */
            int l_h = r.radius; /* line height */

            /* create a bitmap for the phrase */
            unsigned char* bitmap = (unsigned char*)calloc(b_w * b_h , sizeof(unsigned char));
            /* calculate font scaling */
            float scale = stbtt_ScaleForPixelHeight(&info, l_h);

            char* word = (char*)text.data();
            
            x = 0;

            int ascent, descent, lineGap;

            stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

            ascent = roundf(ascent * scale);

            for (int i = 0; i < 1; i++) {
                /* how wide is this character */
                int ax;
                int lsb;
                stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);
                /* (Note that each Codepoint call has an alternative Glyph version which caches the work required to lookup the character word[i].) */

                /* get bounding box for character (may be offset to account for chars that dip above or below the line */
                int c_x1, c_y1, c_x2, c_y2;
                stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
                /* compute y (different characters have different heights */
                y = ascent + c_y1;

                /* advance x */
                x += roundf(ax * scale);

                /* add kerning */
                int kern;
                kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
                x += roundf(kern * scale);
            } 
            std::string ftext; ftext += text.at(0);
            drawTextRAW(ftext,{r.x + x2,r.y+y,r.radius},Font,col,win);
            ls.insert(ls.end(),{ftext,r.radius,I3,textStr,Font});
            points.insert(points.end(),{x2,y});
            fclose(fontFile);
            x2+=x;
        }
        else{
            std::string ftext; ftext += text.at(0);
            drawTextRAW(ftext,{r.x + points.at(L3).x ,r.y+ points.at(L3).y ,r.radius},Font,col,win);
        }
    }
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
void RSGL::drawRecta(RSGL::rect r, RSGL::color c, RSGL::rectArgs args){RSGL::drawRect(r,c,args.fill,args.dotted,args.win);}

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



int RSGL::drawTriangle(RSGL::triangle t, RSGL::color c, bool solid/*=true*/, RSGL::drawable win) {
    if (!win.GPU){}
    
    else if (win.GPU == 1){
        float i = win.r.x/2*1.0f;
        float  x  = (t.x/i)-1.0f;
        float  x2 = ((t.x+t.width)/i)-1.0f;
        i = win.r.y/2*1.0f;
        float  y = (-(t.y)/i)+1.0f;
        float  y2 =(-(t.y+t.length)/i)+1.0f;
        
        if (solid) { glBegin(GL_POLYGON); }
        else { glBegin(GL_LINE_LOOP);}
            glColor4f(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0);
            glVertex2f(x+abs(x2)+0.1f,  y);
            glColor4f(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0);
            glVertex2f(x, y2);
            glColor4f(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0);
            glVertex2f(x2, y2);
        glEnd();
    }
    return 1;
}
int RSGL::drawTrianglea(RSGL::triangle t, RSGL::color c, RSGL::TriArgs args){RSGL::drawTriangle(t,c,args.solid,args.win);}


int RSGL::drawCircle(RSGL::circle c, color col,bool fill,RSGL::drawable win){
    if (!win.GPU){
        XSetForeground(win.display,XDefaultGC(win.display,XDefaultScreen(win.display)),RSGLRGBTOHEX(col.r,col.g,col.b));
        if (fill){XFillArc(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)), c.x-(30/2), c.y-(30/2), c.radius, c.radius, 0, 360*64);}
        else XDrawArc(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)), c.x-(30/2), c.y-(30/2), c.radius, c.radius, 0, 360*64);
        return 1;
    }
    else if (win.GPU == 1){
        c.x+=c.radius/2;
        c.y+=c.radius/2;
        float i = win.r.x/2*1.0f;
        float  x2 = (c.x/i)-1.0f;
        i = win.r.y/2*1.0f;
        float  y2 = (-(c.y)/i)+1.0f;
        c.radius-=10;
        i = win.r.x/2*1.0f;
        //c.radius -= 200 + (win.r.width*0.1);
        float  r = ((-(c.radius)/i)-1.0f);
        if (!fill){
            int n_cx = c.x;
            int n_cy = c.y;
            int radius = c.radius;
            
            double error = (double)-radius;
            double x = (double)radius - 0.5;
            double y = (double)0.5;
            double cx = n_cx - 0.5;
            double cy = n_cy - 0.5;

            while (x >= y){
                RSGL::drawPoint( {(int)(cx + x), (int)(cy + y)},col);
                RSGL::drawPoint( {(int)(cx + y), (int)(cy + x)},col);

                if (x != 0)
                {
                    RSGL::drawPoint( {(int)(cx - x), (int)(cy + y)},col);
                    RSGL::drawPoint( {(int)(cx + y), (int)(cy - x)},col);
                }

                if (y != 0)
                {
                    RSGL::drawPoint( {(int)(cx + x), (int)(cy - y)},col);
                    RSGL::drawPoint( {(int)(cx - y), (int)(cy + x)},col);
                }

                if (x != 0 && y != 0)
                {
                    RSGL::drawPoint({(int)(cx - x), (int)(cy - y)},col);
                    RSGL::drawPoint( {(int)(cx - y), (int)(cy - x)},col);
                }

                error += y;
                ++y;
                error += y;

                if (error >= 0)
                {
                    --x;
                    error -= x;
                    error -= x;
                }
	        }
        }
        if (fill){
            int cx = c.x;
            int cy = c.y;
            int radius = c.radius;	
            static const int BPP = 4;

            for (double dy = 1; dy <= radius; dy += 1.0){

                double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
                int x = cx - dx;
                RSGL::drawLine({(int)(cx - dx), (int)(cy + dy - radius)}, {(int)(cx + dx), (int)(cy + dy - radius)},col);
                RSGL::drawLine({(int)(cx - dx), (int)(cy - dy + radius)}, {(int)(cx + dx), (int)(cy - dy + radius)},col);

            }
        }
  }
  return 1;
}
void RSGL::drawCirclea(RSGL::circle c, RSGL::color col, circleArgs args){RSGL::drawCircle(c,col,args.fill,args.win);}

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

std::vector<RSGL::image> tex;
int RSGL::drawImage(RSGL::image r, RSGL::window win){
    int xx=-1;
    if (!tex.empty()) {
        for (int i=0; i<tex.size(); i++) { 
            if (tex[i].file == r.file) {xx=i;break;} 
        }
    }

    if (xx==-1) {
        png::image< png::rgba_pixel > image(r.file);
        int w=image.get_width();
        int h=image.get_height();
        std::vector<unsigned> img;
        for (png::uint_16 y=0; y < h; y++){  
            for (png::uint_16 x=0; x < w; x++) {
                img.insert(img.end(), image[y][x].alpha*0x1000000 +RSGLRGBTOHEX(image[y][x].blue, image[y][x].green, image[y][x].red));
            }
        }
        glGenTextures(1, &r.tex);
        glBindTexture(GL_TEXTURE_2D, r.tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());
    
        glBindTexture(GL_TEXTURE_2D, 0);
        tex.insert(tex.end(), r);
        xx=tex.size()-1;
        
    }
    float i = win.r.width/2*1.0f; //Convert RSGL::win.r int coordinates to OpenGL float coordinates
    float  x = (r.r.x/i)-1.0f;
    float  x2 =((r.r.x+r.r.width)/i)-1.0f;
    i = win.r.length/2*1.0f;
    float  y = (-(r.r.y)/i)+1.0f;
    float  y2 =(-(r.r.y+r.r.length)/i)+1.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex[xx].tex);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(x,  y);
        glTexCoord2i(1, 0); glVertex2f(x2, y);
        glTexCoord2i(1, 1);  glVertex2f(x2, y2);
        glTexCoord2i(0, 1); glVertex2f(x, y2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

void RSGL::drawImage(std::string fileName, RSGL::rect r,RSGL::window d){ RSGL::drawImage({r,fileName.data()},d); }