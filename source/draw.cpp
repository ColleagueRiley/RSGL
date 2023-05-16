/*
* Copyright (c) 2021-23 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/

#define GL_GLEXT_PROTOTYPES
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define FONTSTASH_IMPLEMENTATION
#define GLFONTSTASH_IMPLEMENTATION
#define RSGLDEP

#ifndef RSGL_H
#include "../RSGL.hpp"
#endif
#ifndef RLGL_H
#include "deps/rlgl.h"
#endif
#include "deps/stb_image.h"

#include "deps/fontstash.h"
#include "deps/glfontstash.h"

#define glTexCoord2iXX(x, y, xx) if (xx != -1) rlTexCoord2f(x, y);
#define glTexCoord2fXX(x, y, xx) if (xx != -1) rlTexCoord2f(x, y);
#define glColor4iFF(args, index) \
    if (args.gradient.size() > index) \
        rlColor4ub(args.gradient[index].r, args.gradient[index].g, args.gradient[index].b, args.gradient[index].a);

namespace RSGL {
    FONScontext* FONSfs = NULL;

    extern RSGL::area REALcurrentWindowSize;

    void glPrerequisites(RSGL::rect r, RSGL::color c, RSGL::drawArgs arg = drawArgs(), RSGL::point customTranslate = (RSGL::point){-1, -1}, bool startFromDown = false);

    void drawSoftOval(RSGL::rect oval, RSGL::color c, RSGL::drawArgs args = drawArgs());

    vector<RSGL::image> tex; // cache for loading images
    vector<RSGL::font> fonts; // vector of cached loaded fonts

    RSGL::font::font(const char* File, int Size){ 
        size = Size; 
        file = (char*)File; 

        if (FONSfs == NULL)
            FONSfs = glfonsCreate(500, 500, 1);

        fonsFont = fonsAddFont(FONSfs, "sans", File);
        
        if(fonsFont == FONS_INVALID) 
            printf("failed to open font\n"); 
    }

    int textWidth(const char* text, RSGL::font font, int size){
        fonsSetSize(FONSfs, size);
        fonsSetFont(FONSfs, font.fonsFont);
        return fonsTextBounds(FONSfs, 0, 0, text, NULL, NULL);
    }

    void loadFont(RSGL::font font, int size){
        bool loaded = false; 


        for (int i = 0; i < fonts.size(); i++){ 
            if (fonts[i].file == font.file){ 
                font = fonts[i];
                loaded = true; 
                break; 
            }
        } // check if the font is in fonts

        if (!loaded){ // if it's not in fonts, load the font into fonts
            font = RSGL::font(font.file, size);
            fonts.push_back(font); 
        } 
    }

    void drawText(const char* text, RSGL::circle c, color color, RSGL::font font, RSGL::drawArgs args/* = {}*/){    
        RSGL::font Font; // current font

        bool loaded = false; 


        for (int i = 0; i < fonts.size(); i++){ 
            if (fonts[i].file == font.file){ 
                Font = fonts[i];
                loaded = true; 
                break; 
            }
        } // check if the font is in fonts
    
        if (!loaded){ // if it's not in fonts, load the font into fonts
            Font = RSGL::font(font.file, c.d);
            fonts.push_back(Font); 
        } 

        int w = fonsTextBounds(FONSfs, c.x, c.y, text, NULL, NULL);

        fonsClearState(FONSfs);

        fonsSetSize(FONSfs, c.d);
        fonsSetFont(FONSfs, Font.fonsFont);

        glPrerequisites({c.x, c.y + (c.d - (c.d/4)), w, c.d}, color, args);
  
        fonsSetColor(FONSfs, glfonsRGBA(color.r, color.b, color.g, color.a));

        RSGL::vector<char*> texts = {" "};
        texts.erase(0);

        char* col = (char*)malloc(0);
        size_t strSize = 0;
        
        for (int i = 0; i < strlen(text); i++) {        
            strSize++;
            col = (char*)realloc(col, strSize);
            col[strSize - 1] = text[i];
            
            if (i == strlen(text) - 1) {
                //printf("%s\n", col);
                texts.push_back(col);
                //printf("%s\n", texts[texts.size() - 1]);

                //free(col);
                //col = (char*)realloc(col, 0);
            }
        }

        //free(col);

        int nly = 0;
        
        for (int i = 0; i < texts.size(); i++) {
            fonsDrawText(FONSfs, c.x, c.y + (c.d - (c.d/4)) + nly, texts[i], NULL);
            nly += c.d;
        } 

        rlPopMatrix();
    }

    void drawRoundRect(RSGL::rect r, RSGL::color c, RSGL::drawArgs d);

    void drawRect(RSGL::rect r,color c, drawArgs args/*={}*/) {
        REALcurrentWindowSize = args.windowSize;

        // rounded rect
        if (args.rounded)
            RSGL::drawRoundRect(r, c, args);

        // draw rect with opengl
        else {
            unsigned int m = (args.fill) ? RL_QUADS :  RL_LINES; // switch the draw type if it's not filled

            
            if (args.texture != -1 || args.glTexture != -1) {
                c.r = c.g = c.b = c.a = 255; // for c++ 98 

                glEnable(GL_TEXTURE_2D);    

                rlSetTexture(args.texture != -1 ?   RSGL::tex[args.texture].tex   :   args.glTexture);
            }

            glPrerequisites(r, c, args);   
                rlBegin(m); // start drawing the rect
                    if (args.fill) {
                        RSGL::glPrerequisites(r, c);
                        rlBegin(RL_QUADS);
                            glColor4iFF(args, 0); 
                            glTexCoord2iXX(0, 0, args.texture);
                            rlVertex2f(r.x, r.y);

                            glColor4iFF(args, 2); 
                            glTexCoord2iXX(0, 1, args.texture);
                            rlVertex2f(r.x, r.y + r.h);

                            glColor4iFF(args, 3); 
                            glTexCoord2iXX(1, 1, args.texture);
                            rlVertex2f(r.x + r.w, r.y + r.h);
                                
                            glColor4iFF(args, 1); 
                            glTexCoord2iXX(1, 0, args.texture);
                            rlVertex2f(r.x + r.w, r.y);
                        rlEnd();
                        rlPopMatrix();
                    } else {
                        glColor4iFF(args, 0);
                        rlVertex2f(r.x, r.y);
                        rlVertex2f(r.x + r.w, r.y);

                        glColor4iFF(args, 3);
                        rlVertex2f(r.x + r.w, r.y);
                        rlVertex2f(r.x + r.w, r.y + r.h);

                        glColor4iFF(args, 1);
                        rlVertex2f(r.x + r.w, r.y + r.h);
                        rlVertex2f(r.x, r.y + r.h);
                        
                        glColor4iFF(args, 2);
                        rlVertex2f(r.x, r.y + r.h);
                        rlVertex2f(r.x, r.y);
                    }
                rlEnd();

            rlPopMatrix(); // end of matrix

            if (args.texture != -1) {
                glDisable(GL_TEXTURE_2D);

                rlSetTexture(0);
            }
        }
    }

    void drawLine(RSGL::point p1, RSGL::point p2, RSGL::color c, RSGL::drawArgs args) {
        REALcurrentWindowSize = args.windowSize;

        glPrerequisites((rect){p1.x, p1.y, abs(p1.x - p2.x), abs(p1.y - p2.y)}, c, args);   

            rlBegin(RL_QUADS); // start drawing the line
                rlColor4ub(c.r, c.g, c.b, c.a); // set the color of the line

                rlVertex2f(p1.x, p1.y);
                rlVertex2f(p2.x, p2.y); // ending point
            rlEnd();
        rlPopMatrix(); // start of opengl matrix
    }

    void drawTriangle(RSGL::triangle t, RSGL::color c, drawArgs args/*={}*/) {
        REALcurrentWindowSize = args.windowSize;
            
        RSGL::point cen = (point){
            (t[0].x+ t[1].x + t[2].x) / 3,
            (t[0].y+ t[1].y + t[2].y) / 3
        };

        int xx = args.texture; 

        if (xx != -1) {
            glEnable(GL_TEXTURE_2D);
            rlSetTexture(RSGL::tex[xx].tex);
            c.r = c.g = c.b = c.a = 255; // for c++ 98 
        }

        glPrerequisites((rect){t[1].x, t[2].y, abs(t[1].x - t[0].x), abs(t[1].y - t[2].y)}, c, args, cen);
            unsigned int m = (args.fill) ? RL_QUADS :  GL_LINES; // switch the draw type if it's not filled

            rlBegin(m); // start drawing
                if (args.fill) {
                    glColor4iFF(args, 0);
                    glTexCoord2fXX(1, 1, xx); 
                    rlVertex2f(t[2].x, t[2].y);
                    
                    glColor4iFF(args, 1);
                    glTexCoord2iXX(0, 0, xx); 
                    rlVertex2f(t[0].x, t[0].y);

                    glColor4iFF(args, 1);

                    if ((float)(t[2].x - t[0].x)/t[1].x < 1)
                        glTexCoord2iXX((float)(t[2].x - t[0].x)/t[1].x, 0, xx); 
                    rlVertex2f(t[0].x, t[0].y);
                    
                    glColor4iFF(args, 0);
                    glTexCoord2iXX(0, 1, xx); 
                    rlVertex2f(t[1].x, t[1].y);
                }
                else {
                    glColor4iFF(args, 0);
                    rlVertex2f(t[2].x, t[2].y);
                    rlVertex2f(t[0].x, t[0].y);

                    glColor4iFF(args, 1);
                    rlVertex2f(t[0].x, t[0].y);
                    rlVertex2f(t[1].x, t[1].y);

                    glColor4iFF(args, 2);
                    rlVertex2f(t[1].x, t[1].y);
                    rlVertex2f(t[2].x, t[2].y);
                }
            rlEnd(); // end drawing
        
        rlPopMatrix(); // end of opengl matrix

        if (xx != -1) {
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void drawPoint(RSGL::point p, color c, RSGL::drawArgs args) { 
        RSGL::drawRect((rect){p.x, p.y, 1, 1}, c, args); // just draw a 1x1 rect  
    }

    void drawCircle(RSGL::circle c, RSGL::color color, RSGL::drawArgs args) {
        RSGL::drawOval((rect){c.x, c.y, c.d, c.d}, color, args);
    }

    void drawOval(RSGL::oval o, RSGL::color c, RSGL::drawArgs args) {
        if (args.vectorGraphics)
            return RSGL::drawSoftOval(o, c, args);
        else 
            return RSGL::drawPolygon(o, c, 360, args);
    }

    void drawPolygon(RSGL::oval o, RSGL::color color, int sides, RSGL::drawArgs args) {
        REALcurrentWindowSize = args.windowSize;

        int i;
        int xx = args.texture;

        if (xx != -1) {
            color.r = color.g = color.b = color.a = 255; // for c++ 98 

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, RSGL::tex[xx].tex);
        }

        o.w /= 2;
        o.h /=2;

        o.x += o.w;
        o.y += o.h;
        float centralAngle = args.rotationAngle;
        
        glPrerequisites(o, color, args);
        rlBegin(args.fill ? RL_QUADS : RL_LINES);
            for (int i = 0; i < sides; i++) {
                rlColor4ub(color.r, color.g, color.b, color.a);

                if (args.fill) {
                    rlTexCoord2f(0, 0);
                    rlVertex2f(o.x, o.y);

                    rlTexCoord2f(0, (sinf(DEG2RAD*centralAngle)*o.w)/o.w);
                    rlVertex2f(o.x + sinf(DEG2RAD*centralAngle)*o.w, o.y + cosf(DEG2RAD*centralAngle)*o.h);

                    rlVertex2f(o.x + sinf(DEG2RAD*centralAngle)*o.w, o.y + cosf(DEG2RAD*centralAngle)*o.h);

                    centralAngle += 360.0f/(float)sides;
                    rlVertex2f(o.x + sinf(DEG2RAD*centralAngle)*o.w, o.y + cosf(DEG2RAD*centralAngle)*o.h);
                } else {
                    glColor4iFF(args, 0);
                    rlVertex2f(o.x + sinf(DEG2RAD*centralAngle)*o.w, o.y + cosf(DEG2RAD*centralAngle)*o.w);
                    centralAngle += 360.0f/(float)sides;
                    glColor4iFF(args, 1);
                    rlVertex2f(o.x + sinf(DEG2RAD*centralAngle)*o.w, o.y + cosf(DEG2RAD*centralAngle)*o.h);
                }
            }
        rlEnd();
        rlPopMatrix();

        if (xx != -1) {
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0); 
        }
    }

    int drawBitmap(unsigned char* data, RSGL::rect rect, char channels, RSGL::area memsize, RSGL::drawArgs args, RSGL::color color) {
        REALcurrentWindowSize = args.windowSize;

        int xx = -1; // check indexs of texs this is

        // try to find if the image has been preloaded
        if (tex.size() && args.preLoad) { 
            for (int i=0; i < tex.size(); i++)
                if (sizeof(data) == sizeof(RSGL::tex[i].data) && !memcmp(data, RSGL::tex[i].data, sizeof(data))) {
                    xx = i;
                    break;
                } 
        }

        if (xx == -1) {    
            unsigned int texture;

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);


            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            
            //glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
            glPixelStorei(GL_UNPACK_ROW_LENGTH, memsize.w);

            unsigned int c = channels > 0 ? GL_RGBA : GL_BGRA;
            
            switch (channels) {
                case -3: c = GL_ALPHA; break;
                case -2: c = GL_GREEN; break;
                case -1: c = GL_BLUE; break;
                case 1: c = GL_RED; break;
                case 2: c = GL_RG; break;
                case 3: c = GL_RGB; break;
                case 4: c = GL_RGBA; break;
                default: break;
            }


            glTexImage2D(GL_TEXTURE_2D, 0, c, memsize.w, memsize.h, 0, c, GL_UNSIGNED_BYTE, data);
            

            glBindTexture(GL_TEXTURE_2D, 0);

            tex.push_back(RSGL::image());

            xx = tex.size() - 1;

            tex[xx].data = data;
            tex[xx].tex = texture;

        }

        args.texture = xx;

        RSGL::drawRect(rect, color, args);

        return xx;
    }

    int drawImage(RSGL::image r, RSGL::drawArgs args/*={}*/) { return drawImage(r.file, r.r, args); }

    int drawImage(const char* file, RSGL::rect r, RSGL::drawArgs args/*={}*/) { 
        int xx = -1;
       
        for (int i = 0; i < tex.size(); i++) /* check if the file has already been loaded */
            if (file == tex[i].file) 
                xx = i;

        if (xx == -1) {
            int w, h, c; /* else load the file */
            
            unsigned char* data = stbi_load((char*)file, &w, &h, &c, 0);

            xx = drawBitmap(data, (rect){0, 0, 0, 0}, c, (area){w, h}, args);

            tex[xx].file = (char*)file;
        }
        
        args.texture = xx;

        RSGL::color c;
        c.r = c.g = c.b = c.a = 255; // for c++ 98 
        
        RSGL::drawRect(r, c, args);

        return xx;
    }

    void drawRoundRect(RSGL::rect r, RSGL::color c, RSGL::drawArgs d) {
        d.rounded = false;

        RSGL::drawRect((RSGL::rect){r.x + (d.roundPoint.x/2), r.y, r.w - d.roundPoint.x, r.h}, c, d);
        RSGL::drawRect((RSGL::rect){r.x, r.y + (d.roundPoint.y/2), r.w,  r.h - d.roundPoint.y}, c, d);

        RSGL::drawOval((RSGL::rect){r.x, r.y, d.roundPoint.x, d.roundPoint.y}, c, d);
        RSGL::drawOval((RSGL::rect){r.x + (r.w - d.roundPoint.x), r.y, d.roundPoint.x, d.roundPoint.y}, c, d);
        RSGL::drawOval((RSGL::rect){r.x + (r.w - d.roundPoint.x), r.y  + (r.h - d.roundPoint.y), d.roundPoint.x, d.roundPoint.y}, c, d);
        RSGL::drawOval((RSGL::rect){r.x, r.y  + (r.h - d.roundPoint.y),  d.roundPoint.x, d.roundPoint.y}, c, d);
    }

    void drawSoftOval(RSGL::rect rect, RSGL::color c, RSGL::drawArgs args) {
        REALcurrentWindowSize = args.windowSize;


        int xx = -1; // check indexs of texs this is

        // try to find if the rect has been preloaded
        if (tex.size() && args.texture == -1) { 
            for (int i=0; i < tex.size(); i++) { 
                if (RSGL::tex[i].file == (char*)"RSGLOVAL" && RSGL::tex[i].r.w == rect.w && RSGL::tex[i].r.h == rect.h) {
                    xx = i;
                    break;
                } 
            }
        }

        if (xx==-1 && args.texture == -1) { // load the image if it has not been preloaded
            int w, h, n = 4;
            unsigned char channels = 4;

            char file[165];
            sprintf(file, "<rect\nfill=\"rgb(255, 255, 255)\"\nwidth=\" %i \"\nheight=\" %i \"\nstroke=\"rgb(255, 255, 255)\"\n"
                            "stroke-width=\" %i \"\nfill-opacity=\" %i \"\nstroke-opacity=\"1\"\nrx=\"204\"\nry=\"204\">",
                    rect.w, rect.h, (!args.fill) + args.lineWidth, args.fill
            );
            
            NSVGimage* image = nsvgParse(file, "px", 96); // load the file
            
            // grab width/height
            RSGL::area size = {(int)image->width, (int)image->height};

            NSVGrasterizer* rast = nsvgCreateRasterizer(); // create rasterzing object for "flattening"

            unsigned char* data = (unsigned char*)malloc(image->width*image->height*4); // make data with room for the image

            nsvgRasterize(rast, image, 0,0,1, data, image->width,image->height, image->width*4); // load the flattened image into the data
            xx = RSGL::drawBitmap(data, RSGL::rect(), n, size, args);

            // free data
            nsvgDelete(image);
            nsvgDeleteRasterizer(rast);
            
            RSGL::tex[xx].file = (char*)"RSGLOVAL";
            RSGL::tex[xx].r = rect;
        }

        else {
            if (args.texture == -1)
                args.texture = xx;

            if (!sizeof(args.gradient))
                args.gradient[0] = c;

            args.rounded = false;
            
            RSGL::drawRect(rect, c, args);
        }
    }

    int loadImage(const char* file) { 
        return RSGL::drawImage((char*)file, (rect){0, 0, 0, 0}); 
    }

    int loadBitmap(unsigned char* data, int channels, RSGL::area memsize) {
         return RSGL::drawBitmap(data, (rect){0, 0, 0, 0}, channels, memsize);
    }

    void freeBitmap(int tex) {
        RSGL::tex[tex].~bitmap();

        for (int i = tex; i < RSGL::tex.size(); i++)
            RSGL::tex[i] = RSGL::tex[i+1];
    }

    void freeImage(int tex) { RSGL::freeBitmap(tex); }

    void freeBitmap(unsigned char* data) {
        for (int i=0; i < sizeof(data) / sizeof(unsigned int); i++)
            if (sizeof(RSGL::tex[i]) == sizeof(data) && !memcmp(data, RSGL::tex[i].data, sizeof(data))) {
                RSGL::freeImage(i);
                break;
            } 
    }

    void freeImage(const char* file) { 
        for (int i=0; i < tex.size(); i++)
            if (RSGL::tex[i].file == file) {
                RSGL::freeImage(i);
                break;
            } 
    }

    // Utility functions (only for internal usages)
    void glPrerequisites(RSGL::rect r, RSGL::color c, RSGL::drawArgs args/* = {}*/, RSGL::point customTranslate/* = {-1, -1}*/, bool startFromDown/* = false*/) {
        rlColor4ub(c.r, c.g, c.b, c.a);
        rlMatrixMode(RL_PROJECTION);
        rlLoadIdentity();
        rlPushMatrix();



        if (!startFromDown) rlOrtho(0, args.windowSize.w, args.windowSize.h, 0, -args.windowSize.w, args.windowSize.w);
        else rlOrtho(0, args.windowSize.w, 0, args.windowSize.h, -args.windowSize.w, args.windowSize.w);

        rlTranslatef((r.x + (r.w/2)), (r.x + (r.h/2)), 0);
            
        rlRotatef(args.rotationAngle,  0, 0, 1);
        rlRotatef(args.flipHorizontal, 0, 1, 0);
        rlRotatef(args.flipVertical, 1, 0, 0);

        rlTranslatef(-(r.x + (r.w/2)), -(r.x + (r.h/2)), 0);
        
        rlMatrixMode(RL_MODELVIEW);

        glLineWidth(args.lineWidth);
    }

    bitmap::~bitmap() { freeBitmap(data); }

    void freeBitmap(unsigned int tex) { 
        if (RSGL::tex[tex].data != NULL)
            free(RSGL::tex[tex].data); 

        RSGL::tex[tex].data = NULL;
    }
}