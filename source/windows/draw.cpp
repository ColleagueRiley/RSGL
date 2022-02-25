#ifndef RSGL
#include "../../include/include/windows/rsgl.hpp"
#endif

int RSGL::drawRect(RSGL::rect r, RSGL::color c, bool solid/*=true*/, bool dotted/*=false*/, int border_size/*=3*/) {
    if (RSGL::win.enabled_flags & 64) { // GDI Rendering
        if (solid) {
            if (c.a==255) { //we don't wanna use transparency for GDI my god
                HBRUSH brush = CreateSolidBrush(RGB(c.r, c.g, c.b));
                if (solid) {
                    RECT rr;
                    rr.top = r.y;
                    rr.bottom= r.y+r.length;
                    rr.left = r.x;
                    rr.right = r.x+r.width;
                    SelectObject(RSGL::win.biop->Get_DC_Buffer(0),brush);
	            
                    FillRect(RSGL::win.biop->Get_DC_Buffer(0),&rr, brush);
                    DeleteObject(brush);
                }
            }
            else { //If the user wants to use GDI but also with transparency
                HDC hdc = GetDC(RSGL::win.hwnd);
                Gdiplus::Graphics g(hdc);
                Gdiplus::SolidBrush  brush(Gdiplus::Color( c.r, c.g, c.b, c.a ));
                Gdiplus::Rect rectangle( r.x, r.y, r.width, r.length );
	        
                g.FillRectangle(&brush, rectangle);
            }
        }
        else {
            HPEN pen = CreatePen(PS_SOLID, border_size, RGB(c.r, c.g, c.b));
            SelectObject(RSGL::win.biop->Get_DC_Buffer(0),pen);
    
            MoveToEx(RSGL::win.biop->Get_DC_Buffer(0), r.x, r.y, NULL);
            LineTo(RSGL::win.biop->Get_DC_Buffer(0), r.x+r.width, r.y);
            LineTo(RSGL::win.biop->Get_DC_Buffer(0), r.x+r.width, r.y+r.length);
            LineTo(RSGL::win.biop->Get_DC_Buffer(0), r.x, r.y+r.length);
            LineTo(RSGL::win.biop->Get_DC_Buffer(0), r.x, r.y);
    
            DeleteObject(pen);
        }
    }
    else if (RSGL::win.enabled_flags & 512) { // DirectX rendering
        /*VOID* pVoid;

        vertex vertices[] = {
            { 400.0f, 62.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
            { 650.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
            { 150.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
        };
        
        v_buffer->Lock(0, 0, (void**)&pVoid, 0);
        memcpy(pVoid, vertices, sizeof(vertices));
        v_buffer->Unlock();*/
        
    }
    else { // OpenGL Rendering
        float i = RSGL::win.r.width/2*1.0f; //Convert RSGL::win.r int coordinates to OpenGL float coordinates
        float  x = (r.x/i)-1.0f;
        float  x2 =((r.x+r.width)/i)-1.0f;
        i = RSGL::win.r.length/2*1.0f;
        float  y = (-(r.y)/i)+1.0f;
        float  y2 =(-(r.y+r.length)/i)+1.0f;
        
        if (solid) { glBegin(GL_POLYGON); }  else { glBegin(GL_LINE_LOOP); } //For unfilled rectangles
            glColor4f(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0);
            glVertex2f(x,  y);
            glVertex2f(x2, y);
            glVertex2f(x2, y2);
            glVertex2f(x, y2);
        glEnd();
    }
    return 0;
}


int RSGL::drawTriangle(RSGL::triangle t, RSGL::color c, bool solid/*=true*/) {
    if (RSGL::win.enabled_flags & 64) { // GDI Rendering
        HDC hdc = GetDC(RSGL::win.hwnd);
        c = {c.r*255, c.g*255, c.b*255, c.a*255};
        TRIVERTEX vertex[3] = {
            {int((t.x+t.width)/2), t.y, (COLOR16)(c.r), (COLOR16)(c.g), (COLOR16)(c.b), (COLOR16)(c.a)},
            {t.x,         t.y+t.length, (COLOR16)(c.r), (COLOR16)(c.g), (COLOR16)(c.b), (COLOR16)(c.a)},
            {t.x+t.width, t.y+t.length, (COLOR16)(c.r), (COLOR16)(c.g), (COLOR16)(c.b), (COLOR16)(c.a)}
        };
        GRADIENT_TRIANGLE gTriangle = {0,1,2};
        GradientFill(hdc, vertex, 3, &gTriangle, 1, GRADIENT_FILL_TRIANGLE);
        if (!solid) {
            c = {RSGL::win.c.r*255, RSGL::win.c.g*255, RSGL::win.c.b*255, RSGL::win.c.a*255};
            TRIVERTEX vertex2[3] = {
                {int((t.x+t.width)/2), t.y+3,   (COLOR16)(c.r), (COLOR16)(c.g), (COLOR16)(c.b), (COLOR16)(c.a)},
                {t.x+3,         t.y+t.length-3, (COLOR16)(c.r), (COLOR16)(c.g), (COLOR16)(c.b), (COLOR16)(c.a)},
                {t.x+t.width-3, t.y+t.length-3, (COLOR16)(c.r), (COLOR16)(c.g), (COLOR16)(c.b), (COLOR16)(c.a)}
            };
            GradientFill(hdc, vertex2, 3, &gTriangle, 1, GRADIENT_FILL_TRIANGLE);
        }
    }
    else {
        float i = RSGL::win.r.x/2*1.0f;
        float  x  = (t.x/i)-1.0f;
        float  x2 = ((t.x+t.width)/i)-1.0f;
        i = RSGL::win.r.y/2*1.0f;
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

int RSGL::drawCircle(RSGL::circle c, RSGL::color col, bool solid/*=true*/, int border_size/*=3*/) {
    if (RSGL::win.enabled_flags & 64) { // GDI Rendering
        HDC hdc = GetDC(RSGL::win.hwnd);
        Gdiplus::Graphics graphics(hdc);
        if (solid) {
            Gdiplus::SolidBrush clr((Gdiplus::Color(col.r, col.g, col.b))); 
            graphics.FillEllipse(&clr, c.x, c.y, c.x+c.radius, c.y+c.radius);
        }
        else {
            Gdiplus::Pen clr{Gdiplus::Color(col.r, col.g, col.b), (float)border_size*1.0f};
            graphics.DrawEllipse(&clr, c.x, c.y, c.x+c.radius, c.y+c.radius);
        }
    }
    else { // OpenGL Rendering
        float i = RSGL::win.r.x/2*1.0f;
        float  x2 = (c.x/i)-1.0f;
        i = RSGL::win.r.y/2*1.0f;
        float  y2 = (-(c.y)/i)+1.0f;
    
        i = RSGL::win.r.x/2*1.0f;
        float  r = ((-(c.radius-220)/i)-1.0f);
        if (!solid){
            glBegin(GL_LINE_LOOP);
                for(int ii = 0; ii < 60; ii++){
                    glColor4f(col.r/255.0, col.g/255.0, col.b/255.0,col.a/255.0);
                    float theta = 2.0f * 3.1415926f * float(ii) / float(60);
        
                    float x = r * cosf(theta);
                    float y = r * sinf(theta);
        
                    glVertex2f(x + x2, y + y2);
                }
            glEnd();
        }
        if (solid){
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

RSGL::image RSGL::loadImage(const char* filename, RSGL::rect r, bool resize/*=false*/, bool upwards/*=false*/, bool reverse/*=false*/) {
    if (RSGL::win.enabled_flags & 64) { }
    else {
        png::image< png::rgba_pixel > image(filename);
        std::vector<std::vector<RSGL::color>> img;

        if (upwards) {
            for (png::uint_16 y=image.get_height()-1; y > 0; y--){  
                img.insert(img.end(),{{}}); 
                if (reverse) for (png::uint_16 x=image.get_width(); x > 0; x--) img.at(img.size()-1).insert(img.at(img.size()-1).end(),{image[y][x].red,image[y][x].green,image[y][x].blue, image[y][x].alpha});
                else for (png::uint_16 x=0; x < image.get_width(); x++) img.at(img.size()-1).insert(img.at(img.size()-1).end(),{image[y][x].red,image[y][x].green,image[y][x].blue, image[y][x].alpha});
            }
        }
        else {
            for (png::uint_16 y=0; y < image.get_height(); y++){  
                img.insert(img.end(),{{}}); 
                if (reverse) for (png::uint_16 x=image.get_width(); x > 0; x--) img.at(img.size()-1).insert(img.at(img.size()-1).end(),{image[y][x].red,image[y][x].green,image[y][x].blue, image[y][x].alpha});
                else for (png::uint_16 x=0; x < image.get_width(); x++) img.at(img.size()-1).insert(img.at(img.size()-1).end(),{image[y][x].red,image[y][x].green,image[y][x].blue, image[y][x].alpha});
            }
        }

        if ((int)image.get_width() != (int)r.width && (int)image.get_height() != r.length && resize) img = RSGL::resizeImage(img,r,{0,0,(int)image.get_width(),(int)image.get_height()});

        float i = RSGL::win.r.x/2*1.0f; 
        std::vector<float> pixels_x, pixels_y, pixels_x2, pixels_y2;  
        std::vector<RSGL::color> clr;

        for (int y3=0; (size_t)y3 < img.size(); y3++){
            for (int x3=0; (size_t)x3 < img.at(y3).size(); x3++){
                if(img.at(y3).at(x3).a >= 100){
                    float  x = (r.x+x3/i)-1.0f;
                    float  x2 =((r.x+x3+1)/i)-1.0f;
                    i = RSGL::win.r.y/2*1.0f;
                    float  y = (-(r.y+y3)/i)+1.0f;
                    float  y2 =(-(r.y+y3+1)/i)+1.0f;

                    pixels_x.insert(pixels_x.end(), x);
                    pixels_y.insert(pixels_y.end(), y);
                    pixels_x2.insert(pixels_x2.end(), x2);
                    pixels_y2.insert(pixels_y2.end(), y2);
                    clr.insert(clr.end(), img.at(y3).at(x3));
                }
            }
        }

        return {filename, r, {pixels_x, pixels_y, pixels_x2, pixels_y2}, clr};
    } 
    return  {filename, r, {}, {}};
}

int RSGL::drawImage(RSGL::image ig) {
    if (RSGL::win.enabled_flags & 64) {
        HDC hdc = GetDC(RSGL::win.hwnd);
        Gdiplus::Graphics graphics(hdc);
        wchar_t WBuf[MAX_PATH];
        mbstowcs(WBuf,ig.filename,MAX_PATH-1);
        Gdiplus::Image img(WBuf);
        if (ig.r.length == -1 && ig.r.width == -1) {
            graphics.DrawImage(&img, 0, 0);
        }
        else {
            Gdiplus::Rect destRect(ig.r.x, ig.r.y, ig.r.width, ig.r.length);
            graphics.DrawImage(&img, destRect);
        }
    }
    else {
        glBegin(GL_POINT);        
        int size = (ig.pixels[0].size()+ig.pixels[1].size()+ig.pixels[2].size()+ig.pixels[3].size())/4;
        for (int i=0; i < size; i++){
            glColor4f(ig.clr[i].r/255.0, ig.clr[i].g/255.0, ig.clr[i].b/255.0,ig.clr[i].a/255.0);
            glVertex2f(ig.pixels[0][i], ig.pixels[1][i]);
            glVertex2f(ig.pixels[2][i], ig.pixels[1][i]);
            glVertex2f(ig.pixels[2][i], ig.pixels[3][i]);
            glVertex2f(ig.pixels[0][i], ig.pixels[3][i]);
        }
        glEnd();
    }
    return 0;
}

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

char ttf_buffer[1<<25];
char ttf_buffer2[1<<25];

int RSGL::drawText(std::string text, RSGL::circle r, const char* font, RSGL::color col){
    return 0;
}
