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

#include <GL/gl.h>

#define glTexCoord2iXX(x, y, xx) if (xx != -1) rlTexCoord2f(x, y);
#define glTexCoord2fXX(x, y, xx) if (xx != -1) rlTexCoord2f(x, y);
#define glColor4iFF(args, index) if (args.gradient.size() > index) rlColor4ub((args.gradient)[index].r, (args.gradient)[index].g, (args.gradient)[index].b, (args.gradient)[index].a);

namespace RSGL {
    extern RSGL::area REALcurrentWindowSize;

    void glPrerequisites(RSGL::rect r, RSGL::color c, RSGL::drawArgs arg = drawArgs(), RSGL::point customTranslate = (RSGL::point){-1, -1}, bool startFromDown = false);

    void drawDottedLine(RSGL::rect r,RSGL::color c, RSGL::drawArgs args, bool t = false);
    void drawRoundRect(RSGL::rect rect, RSGL::color c, RSGL::drawArgs args = drawArgs());
    void drawSoftOval(RSGL::rect oval, RSGL::color c, RSGL::drawArgs args = drawArgs());

    vector<RSGL::image> tex; // cache for loading images
    size_t texSize = 0;
    // vector of cached loaded fonts
    RSGL::font* fonts;
  
    void loadFont(RSGL::font font, int size) {

    }

    void drawText(RSGL::string text, RSGL::circle c, color color, RSGL::font font, RSGL::drawArgs args/* = {}*/) {    
        REALcurrentWindowSize = args.windowSize;

    }

    // draw a doted line function
    void drawDottedLine(RSGL::rect r,RSGL::color c, RSGL::drawArgs args, bool t) {
        if (t) {
            RSGL::drawDottedLine(rect(r.x, r.y, r.x+r.w, r.y), c, args);
            RSGL::drawDottedLine(rect(r.x, r.y, r.x, r.y+r.h), c, args);
            RSGL::drawDottedLine(rect(r.x+ r.w, r.y, r.x+r.w, r.y+r.h), c, args);

            RSGL::drawDottedLine(rect(r.x, r.y+r.h, r.x+r.w ,r.y+r.h), c, args);
        }   
        else{
            int x0 = r.x, y0 = r.y, x1 = r.w, y1 = r.h;
            int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
            int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
            int err = dx + dy, e2;
            int count = 0;

            glPrerequisites(r, c, args);

            float vbo[r.w + r.h/10];
            //glVboON = args.win.vbo;

            rlBegin(RL_LINES); 
            while (x0 != x1 && y0 != y1) {
                if (count < 10) {
                    float x2 = x0;
                    float y2 = y0;
                    rlColor4ub(c.r, c.g, c.b, c.a);
                    rlVertex2f(x2, y2);
                }
                if (x0 == x1 && y0 == y1) break;

                e2 = 2*err;
                if (e2 > dy) { 
                    err += dy; 
                    x0 += sx; 
                }
                else if (e2 < dx) { 
                    err += dx; 
                    y0 += sy; 
                }
                
                count = (count + 1) % 20;
            }
            rlEnd();
            rlPopMatrix();
            glFlush();
        }
    }

    void drawRect(RSGL::rect r,color c, drawArgs args/*={}*/) {
        REALcurrentWindowSize = args.windowSize;

        // rounded rect
        if (args.rounded)
        RSGL::drawRoundRect(r, c, args);

        // dotted rect
        else if (args.dotted)
            RSGL::drawDottedLine(r, c, args, true);

        // draw rect with opengl
        else {
            unsigned int m = (args.fill) ? RL_TRIANGLES :  RL_LINES; // switch the draw type if it's not filled

            if (args.texture != -1) {
                c.r = c.g = c.b = c.a = 255; // for c++ 98 

                glEnable(GL_TEXTURE_2D);

                if (args.ovalTex)
                    glBindTexture(GL_TEXTURE_2D, RSGL::tex[args.texture].ovalTex);
                else
                    glBindTexture(GL_TEXTURE_2D, RSGL::tex[args.texture].tex);
            }

                   
            glPrerequisites(r, c, args);   
        
            rlBegin(m); // start drawing the rect
                    glColor4iFF(args, 0); glTexCoord2iXX(0, 0, args.texture);
                    rlVertex2f(r.x, r.y);
                    glColor4iFF(args, 3); glTexCoord2iXX(1, 1, args.texture);
                    rlVertex2f(r.x + r.w, r.y + r.h);
                    glColor4iFF(args, 1); glTexCoord2iXX(0, 1, args.texture);
                    rlVertex2f(r.x + r.w, r.y);

                    glColor4iFF(args, 0); glTexCoord2iXX(0, 0, args.texture);
                    rlVertex2f(r.x, r.y);
                    glColor4iFF(args, 2); glTexCoord2iXX(0, 1, args.texture);
                    rlVertex2f(r.x, r.y + r.h);
                    glColor4iFF(args, 3); glTexCoord2iXX(1, 1, args.texture);
                    rlVertex2f(r.x + r.w, r.y + r.h);
            rlEnd();

            rlPopMatrix(); // end of matrix
            
            if (args.texture != -1) {
                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0); 
            }
        }
    }

    void drawLine(RSGL::point p1, RSGL::point p2, RSGL::color c, RSGL::drawArgs args) {
        REALcurrentWindowSize = args.windowSize;

        glPrerequisites((rect){p1.x, p1.y, abs(p1.x - p2.x), abs(p1.y - p2.y)}, c, args);   

            rlBegin(RL_LINES); // start drawing the line
                rlColor4ub(c.r, c.g, c.b, c.a); // set the color of the line

                rlVertex2f(p1.x, p1.y);
                rlVertex2f(p2.x, p2.y); // ending point
            rlEnd();
        rlPopMatrix(); // start of opengl matrix
    }

    void drawTriangle(RSGL::triangle t, RSGL::color c, drawArgs args/*={}*/) {
        REALcurrentWindowSize = args.windowSize;
            
        RSGL::point cen = (point){
            (t[0].x + t[1].x + t[2].x) / 3,
            (t[0].y + t[1].y + t[2].y) / 3
        };

        int xx = args.texture; 

        if (xx != -1) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, RSGL::tex[xx].tex);
            c.r = c.g = c.b = c.a = 255; // for c++ 98 
        }

        glPrerequisites(rect(t[1].x, t[2].y, abs(t[1].x - t[0].x), abs(t[1].y - t[2].y)), c, args, cen);
            unsigned int m = (args.fill) ? GL_TRIANGLES :  GL_LINE_LOOP; // switch the draw type if it's not filled

            rlBegin(m); // start drawing
                glColor4iFF(args, 0);
                glTexCoord2fXX(0.5, 0, xx); rlVertex2f(t[2].x, t[2].y);
                glColor4iFF(args, 1);
                glTexCoord2iXX(0, 1, xx); rlVertex2f(t[0].x, t[0].y);
                glColor4iFF(args, 2);
                glTexCoord2iXX(1, 1, xx); rlVertex2f(t[1].x, t[1].y);

                // {{200, 400}, {400, 400}, {300, 200};
            rlEnd(); // end drawing
        
        rlPopMatrix(); // end of opengl matrix

        if (xx != -1) {
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void drawPoint(RSGL::point p, color c, RSGL::drawArgs args) { 
        RSGL::drawRect(rect(p.x, p.y, 1, 1), c, args); // just draw a 1x1 rect  
    }

    void drawCircle(RSGL::circle c, RSGL::color color, RSGL::drawArgs args) {
        RSGL::drawOval(rect(c.x, c.y, c.d, c.d), color, args);
    }

    void drawOval(RSGL::oval o, RSGL::color c, RSGL::drawArgs args) {
        if (args.gradient[0].r)
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
        
        glPrerequisites(o, color, args);
        rlBegin(args.fill ? RL_TRIANGLES : RL_LINES);

        int j = 0;

        for(i = 0; i < 3; i++) {
            float rad = (360 / args.segments * i) * (M_PI / 180.0);

            float xcos = (float)cos(rad);
            float ysin = (float)sin(rad);
            float ztan = (float)tanf(rad);
            
            float tx = xcos * 0.5 + 0.5;
            float ty = ysin * 0.5 + 0.5;
            float tz = ztan * 0.5 + 0.5;
            
            /*glColor4iFF(args, (int)(ty*10))

            glTexCoord2fXX(tx, ty, xx);*/

            rlVertex2f( o.x + xcos, o.y + ysin);

        }

        rlEnd();
        rlPopMatrix();

        if (xx != -1) {
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0); 
        }
    }

    int drawBitmap(unsigned char* data, RSGL::rect rect, unsigned char channels, RSGL::area memsize, RSGL::drawArgs args, RSGL::color color) {
        REALcurrentWindowSize = args.windowSize;

        int xx = -1; // check indexs of texs this is

        // try to find if the image has been preloaded
        if (texSize) { 
            for (int i=0; i < texSize; i++)
                if (sizeof(data) == sizeof(RSGL::tex[i].data) && !memcmp(data, RSGL::tex[i].data, sizeof(data))) {
                    xx = i;
                    break;
                } 
        }

        if (xx == -1) {    
            unsigned int texture = rlLoadTexture(data, memsize.w, memsize.h, (channels == 3) ? RL_PIXELFORMAT_COMPRESSED_DXT1_RGB : RL_PIXELFORMAT_COMPRESSED_DXT1_RGBA, 1);

            texSize++;
            tex.push_back(RSGL::image());

            xx = texture - 1;

            tex[xx].data = data;
            tex[xx].tex = texture;

        }

        args.texture = xx;

        RSGL::drawRect(rect, color, args);

        return xx;
    }

    int drawImage(RSGL::image r, RSGL::drawArgs args/*={}*/) { return drawImage(r.file, r.r, args); }

    int drawImage(RSGL::string file, RSGL::rect r, RSGL::drawArgs args/*={}*/) { 
        int xx = -1;
       
        for (int i = 0; i < texSize; i++) /* check if the file has already been loaded */
            if (file == tex[i].file) 
                xx = i;

        if (xx == -1) {
            int w, h, c; /* else load the file */
            
            unsigned char* data = stbi_load(file.src, &w, &h, &c, 0);

            xx = drawBitmap(data, (rect){0, 0, 0, 0}, c, (area){w, h}, args);
        
            tex[xx].file = file;
        }

        args.texture = xx;

        RSGL::color c;
        c.r = c.g = c.b = c.a = 255; // for c++ 98 
        
        RSGL::drawRect(r, c, args);

        return xx;
    }

    void drawRoundRect(RSGL::rect r, RSGL::color c, RSGL::drawArgs d) {
        d.rounded = false;

        RSGL::drawRect(RSGL::rect(r.x + (d.roundPoint.x/2), r.y, r.w - d.roundPoint.x, r.h), c, d);
        RSGL::drawRect(RSGL::rect(r.x, r.y + (d.roundPoint.y/2), r.w,  r.h - d.roundPoint.y), c, d);

        RSGL::drawOval(RSGL::rect(r.x, r.y, d.roundPoint.x, d.roundPoint.y), c, d);
        RSGL::drawOval(RSGL::rect(r.x + (r.w - d.roundPoint.x), r.y, d.roundPoint.x, d.roundPoint.y), c, d);
        RSGL::drawOval(RSGL::rect(r.x + (r.w - d.roundPoint.x), r.y  + (r.h - d.roundPoint.y), d.roundPoint.x, d.roundPoint.y), c, d);
        RSGL::drawOval(RSGL::rect(r.x, r.y  + (r.h - d.roundPoint.y),  d.roundPoint.x, d.roundPoint.y), c, d);
    }

    void drawSoftOval(RSGL::rect rect, RSGL::color c, RSGL::drawArgs args) {
        REALcurrentWindowSize = args.windowSize;

        RSGL::image r;
        r.r = rect;

        int xx = -1; // check indexs of texs this is

        // try to find if the rect has been preloaded
        if (texSize && args.texture == -1) { 
            for (int i=0; i < texSize; i++) { 
                if (RSGL::tex[i].file == (char*)"RSGLOVAL" && RSGL::tex[i].r.w == rect.w && RSGL::tex[i].r.h == rect.h) {
                    xx = i;
                    break;
                } 
            }
        }

        if (xx==-1 && args.texture == -1) { // load the image if it has not been preloaded
            int w, h, n = 4;
            r.n = 4;
            
            char* file;
            sprintf(file, "<rect\nfill=\"rgb(255, 255, 255)\"\nwidth=\" %i \"\nheight=\" %i \"\nstroke=\"rgb(255, 255, 255)\"\n"
                            "stroke-width=\" %i \"\nfill-opacity=\" %i \"\nstroke-opacity=\"1\"\nrx=\"204\"\nry=\"204\">",
                    rect.w, rect.h, (!args.fill) + args.lineWidth, args.fill
            );

            NSVGimage* image = nsvgParse(file, "px", 96); // load the file
            
            // grab width/height
            r.size = RSGL::rect(0, 0, (int)image->width, (int)image->height);

            NSVGrasterizer* rast = nsvgCreateRasterizer(); // create rasterzing object for "flattening"

            r.data = (unsigned char*)malloc(image->width*image->height*4); // make data with room for the image

        
            nsvgRasterize(rast, image, 0,0,1, r.data, image->width,image->height, image->width*4); // load the flattened image into the data
            RSGL::drawBitmap(r.data, r.r, n, r.size, args);

            // free data
            nsvgDelete(image);
            nsvgDeleteRasterizer(rast);
            
            RSGL::tex[texSize - 1].file = (char*)"RSGLOVAL";
            RSGL::tex[texSize - 1].r = rect;
            xx = texSize - 1;
        }

        else{
            if (args.texture == -1)
                args.texture = xx;
            else
                args.ovalTex = true;

            if (!sizeof(args.gradient))
                args.gradient[0] = c;

            args.rounded = false;
            
            RSGL::drawRect(rect, c, args);
        }
    }

    int loadImage(RSGL::string file) { 
        return RSGL::drawImage(file, rect(0, 0, 0, 0)); 
    }

    int loadBitmap(unsigned char* data, int channels, RSGL::area memsize) {
         return RSGL::drawBitmap(data, rect(0, 0, 0, 0), channels, memsize);
    }

    void freeBitmap(int tex) {
        RSGL::tex[tex].~bitmap();

        for (int i = tex; i < RSGL::texSize; i++)
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

    void freeImage(RSGL::string file) { 
        for (int i=0; i < texSize; i++)
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