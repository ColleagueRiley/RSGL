#ifndef RSGL
#include "../../include/linux/rsgl.hpp" 
#endif
#include "deps/nanosvgrast.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.hpp"

RSGL::Font::Font(std::string File, int Size){ size=Size; file=File; if(!(font = dtx_open_font(file.c_str(), size))) fprintf(stderr, "failed to open font\n"); }

std::vector<RSGL::Font> fonts;

void RSGL::drawText(std::string text,RSGL::circle c,RSGL::Font font,RSGL::color col,RSGL::window win){
    float i = win.r.width/2*1.0f; //Convert RSGL::win.r int coordinates to OpenGL float coordinates
    float  x = (c.x/i)-1.0f;
    i = win.r.length/2*1.0f;
    float  y = (-(c.y)/i)+1.0f;

    RSGL::Font Font;
	if (font.size != c.radius && font.file.size()){ bool loaded=false;
        for (auto& fi : fonts) if (fi.file == font.file && fi.size == c.radius){ Font=fi; loaded=true; break; }
        if (!loaded){ fonts.insert(fonts.end(),RSGL::Font(font.file,c.radius)); Font=fonts.back(); }
    }
	dtx_use_font(Font.font,c.radius);

	glPushMatrix();
    c.x-=5;
    c.y = win.r.length-(c.y+20);
	glOrtho(0, win.r.width, 0, win.r.length, 0, 2);
	glTranslatef(c.x,c.y , 0);
	glColor4f(col.r/255.0, col.g/255.0, col.b/255.0,col.a/255.0);
	dtx_string(text.c_str());
	glPopMatrix();
}


void DrawDottedLine(RSGL::rect r,RSGL::color c,RSGL::drawable win,bool t=false) {
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
    glPushMatrix();
    if (r.rotationAngle){
    glRotatef(r.rotationAngle, 0, 0, 1);
    glTranslatef(dx,dy, 0);
    } glOrtho(0, win.r.width, 0, win.r.length, 0, 2);
    glBegin(GL_POINTS);
    while (1) {
        if (count < 10) {
            float  x2 = x0;
            float  y2 = y0;
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
    glPopMatrix();
    glFlush();
    }
}

void RSGL::drawRect(RSGL::rect r,color c, bool fill,bool dotted, RSGL::drawable win){
    if (win.GPU == 1){
        if (dotted){DrawDottedLine(r,c,win,true);}
        else{
            GLenum m=GL_POLYGON; if (!fill) m=GL_LINE_STRIP;
        
            glPushMatrix();
            if (r.rotationAngle){
                glRotatef(r.rotationAngle, 0, 0, 1);
                glTranslatef(r.x,r.y, 0);
            } glOrtho(0, win.r.width, win.r.length, 0, 0, 2);

            glBegin(m);
            glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
            glVertex2f(r.x,  r.y);
            glVertex2f(r.x+r.width, r.y);
            glVertex2f(r.x+r.width, r.y+r.length);
            glVertex2f(r.x, r.y+r.length);
            if (!fill) glVertex2f(r.x,  r.y);
            glEnd();
            glPopMatrix();
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
    glPushMatrix(); 
    glOrtho(0, win.r.width, win.r.length,0, 0, 2); 
    if (solid) glBegin(GL_TRIANGLES); 
    else{ glBegin(GL_LINE_LOOP);}
        glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
        glVertex3f(t.points.at(2).x, t.points.at(2).y, 0);
        glVertex3f(t.points.at(0).x, t.points.at(0).y, 0);
        glVertex3f(t.points.at(1).x, t.points.at(1).y, 0);
    glEnd();
    glPopMatrix();
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
        c.radius /= 2; c.x+=c.radius; c.y+=c.radius;

        float theta = 3.1415926 * 2 / float(50);
        float tangetial_factor = tanf(theta);//calculate the tangential factor 

        float radial_factor = cosf(theta);//calculate the radial factor 

        glPushMatrix();
        glLineWidth(2);
        glOrtho(0, win.r.width, win.r.length, 0, 0, 2);

        glBegin(GL_LINE_LOOP);
        glColor4f(col.r/255.0,col.g/255.0,col.b/255.0,col.a/255.0);
        for (int i=0; i < c.radius; i++){
            float x = c.radius-i;
            float y = 0;

            for (int ii = 0; ii < 50; ii++)
            {
                glVertex2f(x + c.x, y + c.y);//output vertex 
                float tx = -y;
                float ty = x;
                
                x += tx * tangetial_factor;
                y += ty * tangetial_factor;
                
                x *= radial_factor;
                y *= radial_factor;
            } if (!fill) i=c.radius;
        }
        glEnd();
        glPopMatrix();
    }
  return 1;
}
void RSGL::drawCirclea(RSGL::circle c, RSGL::color col, circleArgs args){RSGL::drawCircle(c,col,args.fill,args.win);}

int RSGL::drawPoint(RSGL::point p, color c, RSGL::drawable win){  RSGL::drawRect({p.x,p.y,1,1,},c,true,false,win); return 1; }

std::vector<RSGL::image> tex;
int RSGL::drawImage(RSGL::image r, RSGL::window win){
    int xx=-1;
    if (!tex.empty()) {
        for (int i=0; i<tex.size(); i++) { 
            if (tex[i].file == r.file) {xx=i;break;} 
        }
    }

    if (xx==-1) {
        int w,h,n;

        unsigned char *data = stbi_load(r.file.c_str(), &w, &h, &n, 0);
        glGenTextures(1, &r.tex);
        glBindTexture(GL_TEXTURE_2D, r.tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        if (!r.r.width && !r.r.length) r.r = {r.r.x,r.r.y,w,h};
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
        if (n==3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h,0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if (n==4)  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
        tex.insert(tex.end(), r);
        xx=tex.size()-1;
    }
    
    if (!r.r.width && !r.r.length) r.r = {r.r.x,r.r.y,tex[xx].r.width,tex[xx].r.length};

    glPushMatrix();
    if (r.r.rotationAngle){
        glRotatef(r.r.rotationAngle, 0, 0, 1);
        glTranslatef(r.r.x,r.r.y, 0);
    }
    glOrtho(0, win.r.width, win.r.length, 0, 0, 2);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex[xx].tex);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(r.r.x,  r.r.y);
        glTexCoord2i(1, 0); glVertex2f(r.r.x + r.r.width, r.r.y);
        glTexCoord2i(1, 1);  glVertex2f(r.r.x + r.r.width, r.r.y + r.r.length);
        glTexCoord2i(0, 1); glVertex2f(r.r.x, r.r.y + r.r.length);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

void RSGL::drawImage(std::string fileName, RSGL::rect r,RSGL::window d){ RSGL::image img; img.r=r; img.file=fileName; RSGL::drawImage(img,d); }

int RSGL::drawSVG(RSGL::image r, RSGL::window win){
    int xx=-1;
    if (!tex.empty()) {
        for (int i=0; i<tex.size(); i++) { 
            if (tex[i].file == r.file) {xx=i;break;} 
        }
    }

    if (xx==-1) {
		NSVGimage *image = NULL;
		NSVGrasterizer *rast = NULL;
		unsigned char* img = NULL;
		int w, h;
		const char* filename =  r.file.c_str();
		image = nsvgParseFromFile(filename, "px", 96.0f);
		w = (int)image->width;
		h = (int)image->height;
		rast = nsvgCreateRasterizer();
		img = (unsigned char*)malloc(w*h*4);
		nsvgRasterize(rast, image, 0,0,1, img, w, h, w*4);

        glGenTextures(1, &r.tex);
        glBindTexture(GL_TEXTURE_2D, r.tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		nsvgDeleteRasterizer(rast);
		nsvgDelete(image);

        glBindTexture(GL_TEXTURE_2D, 0);
        tex.insert(tex.end(), r);
        xx=tex.size()-1;
    }
    glPushMatrix();
    if (r.r.rotationAngle){
        glRotatef(r.r.rotationAngle, 0, 0, 1);
        glTranslatef(r.r.x,r.r.y, 0);
    }
    glOrtho(0, win.r.width, win.r.length, 0, 0, 2);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex[xx].tex);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(r.r.x,  r.r.y);
        glTexCoord2i(1, 0); glVertex2f(r.r.x + r.r.width, r.r.y);
        glTexCoord2i(1, 1);  glVertex2f(r.r.x + r.r.width, r.r.y + r.r.length);
        glTexCoord2i(0, 1); glVertex2f(r.r.x, r.r.y + r.r.length);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
} 
void RSGL::drawSVG(std::string fileName, RSGL::rect r,RSGL::window d){ RSGL::image img; img.r=r; img.file=fileName; RSGL::drawSVG(img,d); }

void RSGL::drawRect(RSGL::rect r, RSGL::color col, RSGL::stroke s,bool fill, RSGL::window d){
    if (fill == false && s.fill){
         for (int i=0; i < s.size; i++){
            RSGL::drawRect({r.x-i,r.y-i,r.width+i*2,r.length+i*2,r.rotationAngle},s.color,false,false,d);
        } RSGL::drawRect({r.x-s.size ,r.y-s.size,3,3,r.rotationAngle},{220,0,0},true);
    } else RSGL::drawRect({r.x-s.size,r.y-s.size,r.width+s.size*2,r.length+s.size*2,r.rotationAngle},s.color,s.fill,false,d);
    RSGL::drawRect(r,col,fill,false,d);
}

void RSGL::drawPoints(std::vector<RSGL::point> p, RSGL::color c, RSGL::drawable win){
    glPushMatrix();
    glOrtho(0, win.r.width, win.r.length, 0, 0, 2);
    glBegin(GL_POINTS);
        glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
        for (int i=0; i < p.size(); i++){
            glVertex2f(p.at(i).x,p.at(i).y);
        }
    glEnd();
    glPopMatrix();
    glFlush();
}

void RSGL::drawPoints(std::vector<RSGL::point> p,std::vector<RSGL::color> c, RSGL::drawable win){
    glPushMatrix();
    glOrtho(0, win.r.width, win.r.length, 0, 0, 2);
    glBegin(GL_POINTS);
        for (int i=0; i < p.size(); i++){
            glColor4f(c.at(i).r/255.0, c.at(i).g/255.0, c.at(i).b/255.0,c.at(i).a/255.0);
            glVertex2f(p.at(i).x,p.at(i).y);
        }
    glEnd();
    glPopMatrix();
    glFlush();
}

void RSGL::drawBoundCircle(RSGL::circle c, RSGL::color col, RSGL::rect boundary,RSGL::rect noHIT,bool fill,RSGL::window win){
    bool noHit= noHIT.x+noHIT.y+noHIT.width+noHIT.length != -1;

    c.radius /= 2; c.x+=c.radius; c.y+=c.radius;

    float theta = 3.1415926 * 2 / float(50);
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    glPushMatrix();
    glLineWidth(2);
    glOrtho(0, win.r.width, win.r.length, 0, 0, 2);

    glBegin(GL_LINE_LOOP);
    glColor4f(col.r/255.0,col.g/255.0,col.b/255.0,col.a/255.0);
    for (int i=0; i < c.radius; i++){
        float x = c.radius-i;
        float y = 0;

        for (int ii = 0; ii < 50; ii++)
        {
            int newX= x + c.x, newY=y + c.y;
            if ( ( boundary.x == -1 ||  newX >= boundary.x) &&  
            (boundary.y == -1 || newX <= boundary.y ) &&
            ( boundary.width == -1 || ( newY >= boundary.width)) && 
            (boundary.length == -1 || newY <= boundary.length ) &&
            (!noHit || noHit && !RSGL::RectCollidePoint(noHIT,{newX,newY}))) glVertex2f(newX, newY);//output vertex 
            
            float tx = -y;
            float ty = x;
            
            x += tx * tangetial_factor;
            y += ty * tangetial_factor;
            
            x *= radial_factor;
            y *= radial_factor;
        } if (!fill) i=c.radius;
    }
    glEnd();
    glLineWidth(1);
    glPopMatrix();
    glFlush(); 
}

void RSGL::drawRoundRect(RSGL::rect r,RSGL::color c,bool fill, RSGL::window win){
    int radius=2;
	if (fill)  RSGL::drawRect({r.x, r.y,r.width-1,r.length-1},c,true,false,win);

    RSGL::drawLine({r.x + radius, r.y}, {r.x + r.width - (radius * 2), r.y},c,win);
    RSGL::drawBoundCircle({r.x + r.width - (radius * 2)-2, r.y, radius+2 * 2},c, {r.x + r.width - (radius * 2),-1,-1,r.y+3},{-1,-1,-1,-1},false,win);
    RSGL::drawLine({r.x + r.width, r.y + radius}, {r.x + r.width, r.y + r.length - (radius * 2)},c,win);
    RSGL::drawBoundCircle({r.x + r.width - (radius * 2)-2 , r.y + r.length - (radius * 2)-2, radius+2 * 2},c,{r.x + r.width - (radius * 2) ,-1,r.y + r.length - (radius * 2),-1},{-1,-1,-1,-1},false,win);

	RSGL::drawLine({r.x + r.width - (radius * 2), r.y + r.length}, {r.x + radius, r.y + r.length},c,win);
    RSGL::drawBoundCircle({r.x, r.y + r.length - (radius * 2), radius * 2},c,{r.x,r.x+2, -1,-1},{-1,-1,-1,-1},false,win);

    RSGL::drawLine({r.x,r.y+r.length - (radius*2)}, {r.x,r.y+radius}, c,win);
    RSGL::drawBoundCircle({r.x-1,r.y,radius*2},c,{r.x,r.x+2,-1,r.y+2},{-1,-1,-1,-1},false,win);
}
