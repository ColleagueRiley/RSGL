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

#ifndef RSGL_H
#include "../RSGL.hpp"
#endif
#ifndef RLGL_H
#include "deps/rlgl.h"
#endif

#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "deps/stb_image.h"
#endif

#include "deps/fontstash.h"
#include "deps/rglfontstash.h"

#define glTexCoord2fXX(x, y) rlTexCoord2f(x, y);
#define glColor4iFF(args, index) \
    if (args.gradient.size > index) \
        rlColor4ub(args.gradient.color##index.r, args.gradient.color##index.g, args.gradient.color##index.b, args.gradient.color##index.a);

namespace RSGL {
    template<typename type>
    struct vector {
        type* src;
        size_t size;

        void push(type data) {
            size++;
            src = (type*)realloc(src, sizeof(type) * size);
            src[size - 1] = data;
        }

        type& operator[](size_t i) { return src[i]; }

        vector() { 
            src = (type*)malloc(0); 
            size = 0;
        }
        ~vector() { free(src); }
    };

    extern RSGL::area REALcurrentWindowSize;

    void glPrerequisites(RSGL::rect r, RSGL::color c, RSGL::drawArgs arg = drawArgs(), RSGL::point customTranslate = (RSGL::point) {-1, -1}, bool startFromDown = false);

    void drawSoftOval(RSGL::rect oval, RSGL::color c, RSGL::drawArgs args = drawArgs());

    vector<RSGL::image> tex; // cache for loading images
    vector<RSGL::font> fonts; // vector of cached loaded fonts

    RSGL::font::font(const char* File, int Size) { 
        size = Size; 
        file = (char*)File; 

        if (ctx == (FONScontext*)0)
            ctx = glfonsCreate(500, 500, 1);

        fonsFont = fonsAddFont(ctx, "sans", File);
        
        if(fonsFont == FONS_INVALID) 
            printf("failed to open font\n"); 
    }

    font::~font() { 
        if (ctx == NULL)
            glfonsDelete(ctx); 
        
        ctx = NULL;
    }

    int textWidth(const char* text, RSGL::font font, int size) {
        bool loaded = false; 

        for (int i = 0; i < fonts.size; i++)
            if (!strcmp(fonts[i].file, font.file)) { 
                font = fonts[i];
                loaded = true; 
                break; 
            }

        // check if the font is in fonts
        if (!loaded) { // if it's not in fonts, load the font into fonts
            font = RSGL::font(font.file, size); 
            fonts.push(font); 
        } 

        fonsSetSize(font.ctx, size);
        fonsSetFont(font.ctx, font.fonsFont);
        return fonsTextBounds(font.ctx, 0, 0, text, NULL, NULL);
    }

    void loadFont(RSGL::font font, int size) {
        bool loaded = false; 

        for (int i = 0; i < fonts.size; i++) { 
            if (!strcmp(fonts[i].file, font.file)) { 
                font = fonts[i];
                loaded = true; 
                break; 
            }
        } // check if the font is in fonts

        if (!loaded) { // if it's not in fonts, load the font into fonts
            font = RSGL::font(font.file, size);
            fonts.push(font); 
        } 
    }

    void drawText(const char* text, RSGL::circle c, color color, RSGL::font font, RSGL::drawArgs args/* = {}*/) {    
        bool loaded = false; 

        for (int i = 0; i < fonts.size; i++)
            if (!strcmp(fonts[i].file, font.file)) { 
                font = fonts[i];
                loaded = true; 
                break; 
            }

        // check if the font is in fonts
        if (!loaded) { // if it's not in fonts, load the font into fonts
            font = RSGL::font(font.file, c.d); 
            fonts.push(font); 
        } 

        if (!strlen(text))
            return;

        int w = fonsTextBounds(font.ctx, c.x, c.y, text, NULL, NULL);

        fonsClearState(font.ctx);

        fonsSetSize(font.ctx, c.d);
        fonsSetFont(font.ctx, font.fonsFont);

        glPrerequisites((RSGL::rect) {c.x, c.y + (c.d - (c.d/4)), w, c.d}, color, args);
        
        fonsSetSpacing(font.ctx, args.spacing);
        fonsSetBlur(font.ctx, args.blur);

        fonsSetColor(font.ctx, glfonsRGBA(color.r, color.b, color.g, color.a));

        size_t textsSize = 1;
        for (int i = 0; i < strlen(text); i++)
            if (text[i] == '\n')
                textsSize++;

        char* texts[textsSize];

        for (int i = 0; i < textsSize; i++) {
            texts[i] = (char*)malloc(strlen(text) * sizeof(char*));

            memset(texts[i], 0, strlen(text) * sizeof(char));
        }

        if (textsSize == 1) 
            texts[0] = (char*)text;

        else {
            int x = 0, y = 0;

            for (int i = 0; i < strlen(text); i++) {
                if (text[i] == '\n') {
                    x++;
                    y = 0;
                }
                else {
                    texts[x][y] = text[i];
                    y++;
                }
            }
        }
        
        int nly = 0;
       
        for (int i = 0; i < textsSize; i++) {
            fonsDrawText(font.ctx, c.x, c.y + (c.d - (c.d / 4)) + nly, texts[i], NULL);
            
            if (textsSize > 1)
                free(texts[i]);
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
            glEnable(GL_TEXTURE_2D);    

            if (args.texture != -1 || args.glTexture != -1) {
                c.r = c.g = c.b = c.a = 255; // for c++ 98 

                rlSetTexture(args.texture != -1 ?   RSGL::tex[args.texture].tex   :   args.glTexture);
            } else 
                rlSetTexture(1);

            glPrerequisites(r, c, args);   
                rlBegin((args.fill) ? RL_QUADS :  RL_LINES); // start drawing the rect
                    if (args.fill) {
                        RSGL::glPrerequisites(r, c);
                        rlBegin(RL_QUADS);
                            glColor4iFF(args, 0); 
                            glTexCoord2fXX(0, 0);
                            rlVertex2f(r.x, r.y);

                            glColor4iFF(args, 2); 
                            glTexCoord2fXX(0, 1);
                            rlVertex2f(r.x, r.y + r.h);

                            glColor4iFF(args, 3); 
                            glTexCoord2fXX(1, 1);
                            rlVertex2f(r.x + r.w, r.y + r.h);
                                
                            glColor4iFF(args, 1); 
                            glTexCoord2fXX(1, 0);
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

        glPrerequisites((rect) {p1.x, p1.y, abs(p2.x - p1.x), abs(p2.y - p1.y)}, c, args);   
            rlBegin(RL_LINES); // start drawing the line
                rlColor4ub(c.r, c.g, c.b, c.a); // set the color of the line

                rlVertex2f(p1.x, p1.y); 
                rlVertex2f(p2.x, p2.y); // ending point
            rlEnd();
        rlPopMatrix(); // start of opengl matrix
    }

    void drawTriangle(RSGL::triangle t, RSGL::color c, drawArgs args/*={}*/) {
        REALcurrentWindowSize = args.windowSize;
            
        RSGL::point cen = (point) {
            (t[0].x+ t[1].x + t[2].x) / 3,
            (t[0].y+ t[1].y + t[2].y) / 3
        };

        int xx = args.texture; 

        glEnable(GL_TEXTURE_2D);
        rlSetTexture(1);
            
        if (xx != -1) {
            rlSetTexture(RSGL::tex[xx].tex);
            c.r = c.g = c.b = c.a = 255; // for c++ 98 
        } 

        glPrerequisites((rect) {t[1].x, t[2].y, abs(t[1].x - t[0].x), abs(t[1].y - t[2].y)}, c, args, cen);
            unsigned int m = (args.fill) ? RL_QUADS :  GL_LINES; // switch the draw type if it's not filled

            rlBegin(m); // start drawing
                if (args.fill) {
                    glColor4iFF(args, 1);
                    if ((float)(t[2].x - t[0].x)/t[1].x < 1)
                        glTexCoord2fXX((float)(t[2].x - t[0].x) / t[1].x, 0);
                    rlVertex2f(t[0].x, t[0].y);
                    
                    glColor4iFF(args, 1);
                    glTexCoord2fXX(0, 0); 
                    rlVertex2f(t[1].x, t[1].y);

                    glColor4iFF(args, 0);
                    glTexCoord2fXX(0, 1);  
                    rlVertex2f(t[1].x, t[1].y);
                    
                    glColor4iFF(args, 0);
                    glTexCoord2fXX(1, 1); 
                    rlVertex2f(t[2].x, t[2].y);
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
        RSGL::drawRect((rect) {p.x, p.y, 1, 1}, c, args); // just draw a 1x1 rect  
    }

    void drawCircle(RSGL::circle c, RSGL::color color, RSGL::drawArgs args) { RSGL::drawOval((rect) {c.x, c.y, c.d, c.d}, color, args); }

    void drawOval(RSGL::oval o, RSGL::color c, RSGL::drawArgs args) { RSGL::drawPolygon(o, c, 360, args); }

    void drawPolygon(RSGL::oval o, RSGL::color color, int sides, RSGL::drawArgs args) {
        REALcurrentWindowSize = args.windowSize;

        int i;
        int xx = args.texture;

   
        if (xx != -1) {
            color.r = color.g = color.b = color.a = 255; // for c++ 98 

            glBindTexture(GL_TEXTURE_2D, RSGL::tex[xx].tex);
        }

        o.w /= 2;
        o.h /=2;

        o.x += o.w;
        o.y += o.h;
        float centralAngle = args.rotationAngle;

        if (args.arc.y == -1)
            args.arc.y = sides;

        double t = 0;
        glPrerequisites(o, color, args);
        rlBegin(args.fill ? ((xx != -1) ? RL_QUADS :  RL_TRIANGLES) : RL_LINES);
            for (int i = 0; i < args.arc.y; i++) {
                rlColor4ub(color.r, color.g, color.b, color.a);

                if (i < args.arc.x ) 
                    centralAngle += 360.0f/(float)sides;

                else if (args.fill) {
                    float rad = (360 / sides * i) * (M_PI / 180.0);

                    float tx = (float)cos(rad) * 0.5 + 0.5;
                    float ty = (float)sin(rad) * 0.5 + 0.5;

                    glTexCoord2fXX(0.5f, 0.5f);
                    glColor4iFF(args, 0);
                    rlVertex2f(o.x, o.y);

                    glTexCoord2fXX(ty, 0);
                    glColor4iFF(args, 2);
                    rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);

                    if (xx != -1) {
                        glTexCoord2fXX(ty, tx);
                        glColor4iFF(args, 2);
                        rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
                    }

                    centralAngle += 360.0f/(float)sides;

                    glTexCoord2fXX(ty, tx);
                    glColor4iFF(args, 3);
                    rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
                } else {
                    glColor4iFF(args, 0);
                    rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.w);
                    centralAngle += 360.0f/(float)sides;
                    glColor4iFF(args, 1);
                    rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
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
        if (tex.size && args.preLoad) { 
            for (int i=0; i < tex.size; i++)
                if ((channels * memsize.w * memsize.h) == (RSGL::tex[i].channels * RSGL::tex[i].size.w * RSGL::tex[i].size.h) && 
                    !memcmp(data, RSGL::tex[i].data, (RSGL::tex[i].channels * RSGL::tex[i].size.w * RSGL::tex[i].size.h))
                ) {
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

            tex.push(RSGL::image());

            xx = tex.size - 1;

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
       
        for (int i = 0; i < tex.size; i++) /* check if the file has already been loaded */
            if (file == tex[i].file) 
                xx = i;

        if (xx == -1) {
            int w, h, c; /* else load the file */
            
            unsigned char* data = stbi_load((char*)file, &w, &h, &c, 0);

            args.preLoad = false;
            xx = drawBitmap(data, (rect) {0, 0, 0, 0}, c, (area) {w, h}, args);

            free(data);

            tex[xx].file = (char*)file;
            tex[xx].data = NULL;
        }
        
        args.texture = xx;

        RSGL::color c;
        c.r = c.g = c.b = c.a = 255; // for c++ 98 
        
        RSGL::drawRect(r, c, args);

        return xx;
    }

    void drawRoundRect(RSGL::rect r, RSGL::color c, RSGL::drawArgs d) {
        d.rounded = false;

        if (d.fill) {
            RSGL::drawRect((RSGL::rect) {r.x + (d.roundPoint.x/2), r.y, r.w - d.roundPoint.x, r.h}, c, d);
            RSGL::drawRect((RSGL::rect) {r.x, r.y + (d.roundPoint.y/2), r.w,  r.h - d.roundPoint.y}, c, d);
        } else {
            RSGL::drawRect((RSGL::rect) {r.x + (d.roundPoint.x/2), r.y, r.w - d.roundPoint.x, 1}, c, d);
            RSGL::drawRect((RSGL::rect) {r.x + (d.roundPoint.x/2), r.y + r.h, r.w - d.roundPoint.x, 1}, c, d);
            
            RSGL::drawRect((RSGL::rect) {r.x, r.y + (d.roundPoint.y/2), 1,  r.h - d.roundPoint.y}, c, d);
            RSGL::drawRect((RSGL::rect) {r.x + r.w, r.y + (d.roundPoint.y/2), 1,  r.h - d.roundPoint.y}, c, d);
        }

        d.arc = {180, 270};
        RSGL::drawOval((RSGL::rect) {r.x, r.y, d.roundPoint.x, d.roundPoint.y}, c, d);
        d.arc = {90, 180};
        RSGL::drawOval((RSGL::rect) {r.x + (r.w - d.roundPoint.x), r.y, d.roundPoint.x, d.roundPoint.y}, c, d);
        d.arc = {0, 90};
        RSGL::drawOval((RSGL::rect) {r.x + (r.w - d.roundPoint.x), r.y  + (r.h - d.roundPoint.y), d.roundPoint.x, d.roundPoint.y}, c, d);
        d.arc = {270, 360};
        RSGL::drawOval((RSGL::rect) {r.x, r.y  + (r.h - d.roundPoint.y),  d.roundPoint.x, d.roundPoint.y}, c, d);
    }

    int loadImage(const char* file) { 
        return RSGL::drawImage((char*)file, (rect) {0, 0, 0, 0}); 
    }

    int loadBitmap(unsigned char* data, int channels, RSGL::area memsize) {
         return RSGL::drawBitmap(data, (rect) {0, 0, 0, 0}, channels, memsize);
    }

    void freeBitmap(int tex) {
        RSGL::tex[tex].~bitmap();

        for (int i = tex; i < RSGL::tex.size; i++)
            RSGL::tex[i] = RSGL::tex[ i + 1];
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
        for (int i=0; i < tex.size; i++)
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

        rlTranslatef((r.x + (r.w / 2)), (r.x + (r.h / 2)), 0);
            
        rlRotatef(args.rotationAngle,  0, 0, 1);
        rlRotatef(args.flipHorizontal, 0, 1, 0);
        rlRotatef(args.flipVertical, 1, 0, 0);

        rlTranslatef(-(r.x + (r.w / 2)), -(r.x + (r.h / 2)), 0);

        rlMatrixMode(RL_MODELVIEW);

        glLineWidth(args.lineWidth);
    }

    bitmap::~bitmap() { 
        if (data != NULL)
            free(data);
        if (tex != -1)
            glDeleteTextures(1, &tex);

        data = NULL;
        tex = -1;
    }
}