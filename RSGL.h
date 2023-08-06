/*
* Copyright (c) 2021-23 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following r estrictions:
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

/*
    define args
    (MAKE SURE RSGL_IMPLEMENTATION is in at least one header or you use -DRSGL_IMPLEMENTATION)
	#define RSGL_IMPLEMENTATION - makes it so source code is included with header
    
    #define RSGL_NO_WINDOW - no RSGL_window, RSGL_graphics is used instead [this is for using a differnt window manager other than RGFW ]
    #define RSGL_NO_TEXT - do not include text rendering functions
    #define RGFW_NO_WIDGETS - do not include widgets
    #define RSGL_NO_AUDIO - do not include audio functions
    #define RSGL_NO_MINIAUDIO_IMPLEMENTATION - do not have `#define MINIAUDIO_IMPLEMENTATION` in this header (you'll have to link miniaudio some other way to use audio)
    #define RSGL_NO_SAVE_IMAGE - do not save/load images (don't use RSGL_drawImage if you use this), 
                                    RSGL_drawImage saves the file name + texture so it can load it
                                    when you ask for it later. This disables that 
    #define RSGL_INIT_FONTS [number of fonts] - set how much room should be pre-allocated for fonts by fontstash
                                                this avoids performance issues related to realloc
                                                RSGL_INIT_FONTS = 4 by default
    #define RSGL_INIT_IMAGES [number of fonts] - set how much room should be pre-allocated for images by RSGL
                                                this avoids performance issues related to realloc
                                                RSGL_INIT_IMAGES = 20 by default
    #define RSGL_NEW_IMAGES [number of fonts] - set how much room should be reallocated at a time for images by RSGL
                                                this avoids performance issues related to realloc
                                                RSGL_NEW_IMAGES 10 by default

    RGFW (more RGFW documentation in RGFW.h):
    
	#define RGFW_PRINT_ERRORS - (optional) makes it so RGFW prints errors when they're found
	#define RGFW_OSMESA - (optional) use OSmesa as backend (instead of system's opengl api + regular opengl)
	#define RGFW_BUFFER - (optional) just draw directly to (RGFW) window pixel buffer that is drawn to screen
	#define RGFW_EGL - (optional) use EGL for loading an OpenGL context (instead of the system's opengl api)
	#define RGFW_OPENGL_ES - (optional) use EGL to load and use Opengl ES for backend rendering (instead of the system's opengl api)
	#define VULKAN - (optional) use vulkan for the rendering backend (rather than opengl)

	#define RGFW_LINK_EGL (optional) (windows only) if EGL is being used, if EGL functions should be defined dymanically (using GetProcAddress)
	#define RGFW_LINK_OSMESA (optional) (windows only) if EGL is being used, if OS Mesa functions should be defined dymanically  (using GetProcAddress)

	#define RGFW_X11 (optional) (unix only) if X11 should be used. This option is turned on by default by unix systems except for MacOS
	#define RGFW_WGL_LOAD (optional) (windows only) if WGL should be loaded dynamically during runtime
*/
#define RSGL_NO_AUDIO

#ifndef RSGL_INIT_FONTS
#define RSGL_INIT_FONTS 4
#endif
#ifndef RSGL_INIT_IMAGES
#define RSGL_INIT_IMAGES 20
#endif
#ifndef RSGL_NEW_IMAGES
#define RSGL_NEW_IMAGES 10
#endif


#ifndef RSGL_H
#define RSGL_H

typedef struct RSGL_rect {
    int x, y, w, h;
} RSGL_rect;
#define RSGL_RECT(x, y, w, h) (RSGL_rect){x, y, w, h}

#define RSGL_between(x, lower, upper) (((lower) <= (x)) && ((x) <= (upper)))

#ifndef RSGL_NO_WINDOW
#define RGFW_RECT RSGL_rect
#define RGFW_EXTRA_CONTEXT
#include "deps/RGFW.h"
#else
typedef struct {unsigned int type, x, y;} RGFW_Event;
#endif


#include <stdbool.h>
#include <stddef.h>

/* 
*******
RSGL_[shape]
*******
*/

typedef struct RSGL_point {
    int x, y;
} RSGL_point;
#define RSGL_POINT(x, y) (RSGL_point){x, y}

typedef struct RSGL_pointF {
    float x, y;
} RSGL_pointF;

typedef struct RSGL_point3D {
    int x, y, z;
} RSGL_point3D;

#define RSGL_POINT3D(x, y, z) (RSGL_point3D){x, y, z}

typedef struct RSGL_area {
    int w, h;
} RSGL_area;
#define RSGL_AREA(w, h) (RSGL_area){w, h}

typedef struct RSGL_circle {
    int x, y, d;
} RSGL_circle;
#define RSGL_CIRCLE(x, y, d) (RSGL_circle){x, y, d}

typedef struct RSGL_triangle {
    RSGL_point p1, p2, p3;
} RSGL_triangle;
#define RSGL_TRIANGLE(p1, p2, p3) (RSGL_triangle){p1, p2, p3}

#define RSGL_createTriangle(x1, y1, x2, y2, x3, y3) (RSGL_triangle){{x1, y1}, {x2, y2}, {x3, y3}}

typedef struct RSGL_color {
    unsigned char r, g, b, a;
} RSGL_color;

#define RSGL_RGB(r, g, b) (RSGL_color){r, g, b, 255}
#define RSGL_RGBA(r, g, b, a) (RSGL_color){r, g, b, a}

/* 
*******
RSGL_window
*******
*/

#ifndef RSGL_NO_WINDOW
typedef RGFW_window RSGL_window;

inline RSGL_window* RSGL_createWindow(const char* name, RSGL_rect r, unsigned long args);

inline void RSGL_window_setIconImage(RSGL_window* win, const char* image); 
#define RGFW_window_setIcon RSGL_window_setIcon

#define RSGL_window_checkEvent RGFW_window_checkEvent

inline void RSGL_window_makeCurrent(RSGL_window* win);

inline void RSGL_window_clear(RSGL_window* win, RSGL_color color);

inline void RSGL_window_close(RSGL_window* win);
#else /* RSGL_NO_WINDOW */

/* 
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/

inline void RSGL_initGraphics(RSGL_area r, void* loader);
inline void RSGL_graphics_clear(RSGL_color c);
inline void RSGL_graphics_free();

#endif /* RSGL_GRAPHICS_CONTEXT / !RSGL_NO_WINDOW */

/* 
*******
RSGL_draw
*******
*/


/* RSGL_draw args */
inline void RSGL_rotate(RSGL_point3D rotate);
inline void RSGL_setTexture(unsigned int texture);
inline void RSGL_setGradient(RSGL_color gradient[], size_t len);

/* args clear after a draw function by default, this toggles that */
inline void RSGL_defaultClearArgs(); /* toggles if args are cleared by default or not */
inline void RSGL_clearArgs(); /* clears the args */

void RSGL_drawPoint(RSGL_point p, RSGL_color c);
void RSGL_drawTriangle(RSGL_triangle t, RSGL_color c);

void RSGL_drawRect(RSGL_rect r, RSGL_color c);
void RSGL_drawRoundRect(RSGL_rect r, RSGL_point rounding, RSGL_color c);

void RSGL_drawPolygon(RSGL_rect r, unsigned int sides, RSGL_color c);
void RSGL_drawArc(RSGL_rect o, RSGL_point arc, RSGL_color color);
void RSGL_drawCircle(RSGL_circle c, RSGL_color color);
void RSGL_drawOval(RSGL_rect o, RSGL_color c);

void RSGL_drawLine(RSGL_point p1, RSGL_point p2, unsigned int thickness, RSGL_color c);

void RSGL_drawTriangleOutline(RSGL_triangle t, unsigned int thickness, RSGL_color c);

void RSGL_drawRectOutline(RSGL_rect r, unsigned int thickness, RSGL_color c);
void RSGL_drawRoundRectOutline(RSGL_rect r, RSGL_point rounding, unsigned int thickness, RSGL_color c);

void RSGL_drawPolygonOutline(RSGL_rect r, unsigned int sides, unsigned int thickness, RSGL_color c);
void RSGL_drawArcOutline(RSGL_rect o, RSGL_point arc, unsigned int thickness, RSGL_color color);
void RSGL_drawCircleOutline(RSGL_circle c, unsigned int thickness, RSGL_color color);
void RSGL_drawOvalOutline(RSGL_rect o, unsigned int thickness, RSGL_color c);

#ifndef RSGL_NO_TEXT
inline unsigned int RSGL_loadFont(const char* font);
#define RSGL_FONT(str) RSGL_loadFont(str)

void RSGL_setFont(unsigned int font);

void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color);
#define RSGL_drawTextF(text, font, c, color) \
    RSGL_setFont(font);\
    RSGL_drawText(text, c, color);

inline unsigned int RSGL_textWidth(const char* text, unsigned int fontSize, size_t textEnd);
#define RSGL_textWidthF(text, fontSize, textEnd) \
    RSGL_setFont(font);\
    RSGL_textWidthF(text, fontSize, textEnd);
#endif /* RSGL_NO_TEXT */

inline unsigned int RSGL_createTexture(unsigned char* bitmap, RSGL_area memsize, unsigned char channels);
inline unsigned int RSGL_drawImage(const char* image, RSGL_rect r);

#define RSGL_loadImage(image) RSGL_drawImage(image, (RSGL_rect){})

/* 
*******
RSGL_widgets
*******
*/

#ifndef RGFW_NO_WIDGETS

typedef enum {
    RSGL_none = 0,
    RSGL_hovered,
    RSGL_pressed,
} RSGL_buttonStatus;

typedef struct RSGL_button {
    char* text;
    RSGL_rect r;
    RSGL_buttonStatus s;
    bool toggle; /* for toggle buttons */
} RSGL_button;

/* these functions should be run in an event loop */
inline void RSGL_button_update(
    RSGL_button* b, /* button pointer */
    RGFW_Event e /* current event */
);

inline void RSGL_ratio_button_update(
    RSGL_button* bArray, /* array of ratio buttons (if you have an array of general buttons I suggest you do something like `buttons + x`) */
    size_t arrayLen, /* size of the array, or last button to update in the array */
    RGFW_Event e
);

inline void RSGL_slider_update(
    RSGL_button* b, /* button pointer */
    /* limits, 
        if the slider moves [up/down], the width should be 0, 
        if the slight moves [left/right] the height should be 0 
    */
    RSGL_rect limits,
    RGFW_Event e
);

#ifndef RSGL_NO_TEXT
typedef struct RSGL_textbox {
    char* text;

    RSGL_rect r;
    unsigned int textSize;
    RSGL_point cursor;

    /* info about the string */
    size_t index, len, capacity;       
} RSGL_textbox;

inline RSGL_textbox* RSGL_createTextbox(const char* text, unsigned int textSize, RSGL_rect box, RSGL_point cursor);

inline void RSGL_textbox_update(RSGL_textbox* texbox, RGFW_Event e);
inline void RSGL_textbox_draw(RSGL_textbox* textBox, int font, RSGL_color c, RSGL_color cursorColor);

inline void RSGL_textbox_free(RSGL_textbox* tb);
#endif /* RSGL_NO_TEXT */

#endif /* RGFW_NO_WIDGETS */

/* 
*******
RSGL_audio
*******
*/

#ifndef RSGL_NO_AUDIO

struct RSGL_audioData;

typedef struct RSGL_audio {
    char* file;
    bool loop;
    bool ptr; /* if the audio struct is a pointer (so it can be freed latre on) */

    struct RSGL_audioData* data;
} RSGL_audio;

void RSGL_audio_playFile(RSGL_audio* a, const char* file);
inline void RSGL_audio_play(RSGL_audio a);
inline void RSGL_audio_pause(RSGL_audio a);
inline void RSGL_audio_stop(RSGL_audio a);
inline void RSGL_audio_free(RSGL_audio a);

/* write audio info */
inline void RSGL_audio_setVolume(RSGL_audio a, unsigned int);
inline void RSGL_audio_seek(RSGL_audio a, unsigned int position);

/* get audio info */
inline unsigned int RSGL_audio_len(RSGL_audio a);
inline unsigned int RSGL_audio_volume(RSGL_audio a);
inline unsigned int RSGL_audio_position(RSGL_audio a);
inline bool RSGL_audio_isPlaying(RSGL_audio a);

#endif /* RSGL_NO_AUDIO */

/* 
*******
extra
*******
*/

/* wait functions */
inline bool RSGL_wait(unsigned int miliseconds);
inline bool RSGL_wait_frames(unsigned int frames);
inline char RSGL_keyCodeToKeyChar(unsigned int keycode);

/* ** collision functions ** */
inline bool RSGL_circleCollidePoint(RSGL_circle c, RSGL_point p);
inline bool RSGL_circleCollideRect(RSGL_circle c, RSGL_rect r);
inline bool RSGL_circleCollide(RSGL_circle cir1, RSGL_circle cir2);
inline bool RSGL_rectCollidePoint(RSGL_rect r, RSGL_point p);
inline bool RSGL_rectCollide(RSGL_rect r, RSGL_rect r2);
inline bool RSGL_pointCollide(RSGL_point p, RSGL_point p2);

#endif /* ndef RSGL_H */

/*
(Notes on how to manage Silicon (macos) included)

Example to get you started :

linux : gcc main.c -lX11 -lXcursor -lGL
windows : gcc main.c -lopengl32 -lshell32 -lgdi32
macos:
	<Silicon> can be replaced to where you have the Silicon headers stored
	<libSilicon.a> can be replaced to wherever you have libSilicon.a
	clang main.c -I<Silicon> <libSilicon.a> -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo

	NOTE(EimaMei): If you want the MacOS experience to be fully single header, then I'd be best to install Silicon (after compiling)
	by going to the `Silicon` folder and running `make install`. After this you can easily include Silicon via `#include <Silicon/silicon.h>'
	and link it by doing `-lSilicon`

#define RSGL_NO_AUDIO
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main() {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, 0);


    for (;;) {
        RSGL_window_checkEvent(win); // NOTE: checking events outside of a while loop may cause input lag 

        if (win->event.type == RGFW_quit || RGFW_isPressedI(win, RGFW_Escape))
            break;

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 255, 255))
        RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_window_close(win);
}

	compiling :

	if you wish to compile the library all you have to do is create a new file with this in it

	RSGL.c
	#define RSGL_IMPLEMENTATION
	#include "RSGL.h"

	then you can use gcc (or whatever compile you wish to use) to compile the library into object file

	ex. gcc -c RSGL.c -fPIC

	after you compile the library into an object file, you can also turn the object file into an static or shared library

	(commands ar and gcc can be replaced with whatever equivalent your system uses)
	static : ar rcs RSGL.a RSGL.o
	shared :
		windows:
			gcc -shared RSGL.o  -lshell32 -lgdi32 -o RSGL.dll
		linux:
			gcc -shared RSGL.o -lX11 -lXcursor -o RSGL.so
		macos:
			<Silicon/include> can be replaced to where you have the Silicon headers stored
			<libSilicon.a> can be replaced to wherever you have libSilicon.a
			gcc -shared RSGL.o -framework Foundation <libSilicon.a> -framework AppKit -framework CoreVideo -I<Silicon/include>

	installing/building silicon (macos)

	Silicon does not need to be installde per se.
	I personally recommended that you use the Silicon included using RGFW

	to build this version of Silicon simplly run

	cd Silicon && make

	you can then use Silicon/include and libSilicon.a for building RGFW projects

	ex.
	gcc main.c -framework Foundation -lSilicon -framework AppKit -framework CoreVideo -ISilicon/include

	I also suggest you compile Silicon (and RGFW if applicable)
	per each time you compile your application so you know that everything is compiled for the same architecture.
*/

#ifdef RSGL_IMPLEMENTATION

#define RGFW_IMPLEMENTATION
#define RLGL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <assert.h>

#include "deps/rlgl.h"

#ifndef RSGL_NO_WINDOW
#include "deps/RGFW.h"
#endif

#ifndef RSGL_NO_TEXT
#include "deps/rglfontstash.h"
#endif /* RSGL_NO_TEXT */

#include "deps/stb_image.h"

#include <time.h>

#ifndef RSGL_NO_AUDIO

#ifndef RSGL_NO_MINIAUDIO_IMPLEMENTATION
#define MINIAUDIO_IMPLEMENTATION
#endif /* RSGL_NO_MINIAUDIO_IMPLEMENTATION */
#include "deps/miniaudio.h"
#endif /* RSGL_NO_AUDIO */

#undef bool
#if !defined(__cplusplus)
#define bool _Bool
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
#define _Bool bool
#endif

/* RSGL_args */
typedef struct RSGL_drawArgs { 
    RSGL_point3D rotate; 
    unsigned int texture; 
    RSGL_color gradient[6000]; 
    unsigned int gradient_len;
    
    RSGL_rect currentRect; /* size of current window */
} RSGL_drawArgs;

RSGL_drawArgs RSGL_args = {{0, 0, 0}, 1, 0, 0};
bool RSGL_argsClear = true;

unsigned int RSGL_windowsOpen = 0;

typedef struct { const char* img; unsigned int tex;} RSGL_image;
RSGL_image* RSGL_images = NULL;
size_t RSGL_images_len = 0;

inline void glPrerequisites(RSGL_rect r, RSGL_color c);

inline bool RSGL_cstr_equal(const char* str, const char* str2);
bool RSGL_cstr_equal(const char* str, const char* str2) {
    char* s;
    char* s2 = (char*)str2;
    
    for (s = (char*)str; *s && *s2; s++) {
        if (*s != *s2)
            return false;

        s2++;
    }

    if (*s == '\0' && *s2 == '\0')
        return true;
    
    return false;
}


inline void RSGL_BASIC_DRAW(unsigned int RL_TYPE, RSGL_pointF points[], RSGL_pointF texPoints[], RSGL_rect rect, RSGL_color c, size_t len);
void RSGL_BASIC_DRAW(unsigned int RL_TYPE, RSGL_pointF points[], RSGL_pointF texPoints[], RSGL_rect rect, RSGL_color c, size_t len) {
    rlSetTexture(RSGL_args.texture);

    int i;

    glPrerequisites(rect, c);
        rlBegin(RL_TYPE);
            for (i = 0; i < len; i++) {
                if (i && i <= RSGL_args.gradient_len)
                    rlColor4ub(RSGL_args.gradient[i - 1].r, RSGL_args.gradient[i - 1].g, RSGL_args.gradient[i - 1].b, RSGL_args.gradient[i - 1].a);
                rlTexCoord2f(texPoints[i].x, texPoints[i].y);
                rlVertex2f(points[i].x, points[i].y);
            }
        rlEnd();
    rlPopMatrix();
    rlSetTexture(0);

    if (RSGL_argsClear) RSGL_clearArgs();
}

/* 
    RSGL_window
*/

#ifndef RSGL_NO_TEXT
FONScontext* RSGL_fonsContext;
#endif /* RSGL_NO_TEXT */

#ifndef RSGL_NO_WINDOW

RSGL_window* RSGL_createWindow(const char* name, RSGL_rect r, unsigned long args) {
    RGFW_window* win = RGFW_createWindow(name, r.x, r.y, r.w, r.h, args);

    if (RSGL_windowsOpen == 0) {
        rlLoadExtensions((void*)RGFW_getProcAddress);
        rlglInit(win->r.w, win->r.h);
        glEnable(GL_DEPTH_TEST);
        #ifndef RSGL_NO_TEXT
        RSGL_fonsContext = fonsCreateInternal(500, 500, 1);
        #endif
    }

    assert(win != NULL);
    RSGL_windowsOpen++;

    return win;
}

void RSGL_window_setIconImage(RGFW_window* win, const char* image) {
    int x, y, c;
    unsigned char* img = stbi_load(image, &x, &y, &c, 0);

    RGFW_window_setIcon(win, img, x, y, c);

    free(img);
}

void RSGL_window_makeCurrent(RSGL_window* win) {
    RGFW_window_makeCurrent(win);

    RSGL_args.currentRect = win->r;
}

void RSGL_window_clear(RSGL_window* win, RSGL_color color) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rlDrawRenderBatchActive();

    RSGL_window_makeCurrent(win);
    RGFW_window_swapBuffers(win);
    
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}

void RSGL_window_close(RSGL_window* win) {
    RSGL_windowsOpen--;

    if (RSGL_windowsOpen == 0) {
        #ifndef RSGL_NO_TEXT
        fonsDeleteInternal(RSGL_fonsContext);
        #endif
        rlglClose();

        int i;
        for (i = 0; i < RSGL_images_len; i++)
            glDeleteTextures(1, &RSGL_images[i].tex);
    }

    RGFW_window_close(win);
}

#else /* !RGFW_NO_WINDOW */

/* 
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/


void RSGL_initGraphics(RSGL_area r, void* loader) {
    rlLoadExtensions(loader);
    rlglInit(r.w, r.h);
    rlEnableDepthTest();

    RSGL_args.currentRect = (RSGL_rect){0, 0, r.w, r.h};

    #ifndef RSGL_NO_TEXT
    RSGL_fonsContext = glfonsCreate(500, 500, 1);
    #endif
}

void RSGL_graphics_clear(RSGL_color color) {
    rlClearScreenBuffers();
    rlDrawRenderBatchActive();
    
    rlClearColor(color.r, color.g, color.b, color.a);
}

void RSGL_graphics_free() {
    #ifndef RSGL_NO_TEXT
    fonsDeleteInternal(RSGL_fonsContext);
    #endif
    rlglClose();

    if (images != NULL) {
        int i;
        for (i = 0; i < RSGL_images_len; i++)
            glDeleteTextures(1, RSGL_images[i]);
        free(images);
    }
}
#endif /* RSGL_GRAPHICS_CONTEXT / !RGFW_NO_WINDOW */

/* 
****
RSGL_draw
****
*/

/* RSGL_args */

void RSGL_rotate(RSGL_point3D rotate){
    RSGL_args.rotate = rotate;
}
void RSGL_setTexture(unsigned int texture) {
    RSGL_args.texture = texture;
}
void RSGL_setGradient(RSGL_color gradient[], size_t len) {
    RSGL_args.gradient_len = len;
    
    int i;
    for (i = 0; i < RSGL_args.gradient_len; i++)
        RSGL_args.gradient[i] = gradient[i];
}
void RSGL_defaultClearArgs() {
    RSGL_argsClear = !RSGL_argsClear;
}
void RSGL_clearArgs() {
    RSGL_args.rotate = (RSGL_point3D){0, 0, 0}; 
    RSGL_args.texture = 1;
    RSGL_args.gradient_len = 0;    
}

void RSGL_drawPoint(RSGL_point p, RSGL_color c) { RSGL_drawRect((RSGL_rect){p.x, p.y, 1, 1}, c); }
void RSGL_drawTriangle(RSGL_triangle t, RSGL_color c) {
    RSGL_pointF points[] = {{(float)t.p1.x, (float)t.p1.y}, {(float)t.p2.x, (float)t.p2.y}, {(float)t.p2.x, (float)t.p2.y}, {(float)t.p3.x, (float)t.p3.y}};
    RSGL_pointF texPoints[] = {  
                {((float)(t.p3.x - t.p1.x)/t.p2.x < 1) ? (float)(t.p3.x - t.p1.x) / t.p2.x : 0, 0.0f}, 
                {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
    };
    
    RSGL_rect r = {t.p2.x, t.p3.y, abs(t.p2.x - t.p1.x), abs(t.p2.y - t.p3.y)};

    RSGL_BASIC_DRAW(RL_QUADS, (RSGL_pointF*)points, (RSGL_pointF*)texPoints, r, c, sizeof(points)/sizeof(RSGL_point));
}

void RSGL_drawRect(RSGL_rect r, RSGL_color c) {
    RSGL_pointF points[] = {{r.x, r.y}, {r.x, r.y + r.h}, {r.x + r.w, r.y + r.h}, {r.x + r.w, r.y}};
    RSGL_pointF texPoints[] = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};

    RSGL_BASIC_DRAW(RL_QUADS, (RSGL_pointF*)points, (RSGL_pointF*)texPoints, r, c, sizeof(points)/sizeof(RSGL_point));
}

void RSGL_drawRoundRect(RSGL_rect r, RSGL_point rounding, RSGL_color c) {
    RSGL_drawRect((RSGL_rect) {r.x + (rounding.x/2), r.y, r.w - rounding.x, r.h}, c);
    RSGL_drawRect((RSGL_rect) {r.x, r.y + (rounding.y/2), r.w,  r.h - rounding.y}, c);

    RSGL_drawArc((RSGL_rect) {r.x, r.y, rounding.x, rounding.y}, (RSGL_point){180, 270}, c);
    RSGL_drawArc((RSGL_rect) {r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y}, (RSGL_point){90, 180}, c);
    RSGL_drawArc((RSGL_rect) {r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y) - 1, rounding.x, rounding.y + 2}, (RSGL_point){0, 90}, c);
    RSGL_drawArc((RSGL_rect) {r.x + 1, r.y  + (r.h - rounding.y) - 1,  rounding.x, rounding.y + 2}, (RSGL_point){270, 360}, c);
}

#define rlColor4ubX(x) if (RSGL_args.gradient_len >= 1) rlColor4ub(RSGL_args.gradient[x].r, RSGL_args.gradient[x].g, RSGL_args.gradient[x].b, RSGL_args.gradient[x].a);

void RSGL_drawPolygonPro(RSGL_rect o, unsigned int sides, RSGL_point arc, RSGL_color c) {
    o = (RSGL_rect){o.x + (o.w / 2), o.y + (o.h / 2), o.w / 2, o.h / 2};
    float centralAngle = 0;

    static RSGL_pointF points[360 * 4];
    static RSGL_pointF texPoints[360 * 4];

    int x = 0, y;
    
    rlSetTexture(RSGL_args.texture);

    glPrerequisites(o, c);
    rlBegin(RSGL_args.texture == 1 ? RL_TRIANGLES : RL_QUADS);
    for (x = 0; (x / 3) < arc.y; x += 3) {

        if ((x / 3) < arc.x) { 
            centralAngle += 360.0f/(float)sides;
            continue;
        }

        float rad = (360 / sides * (x / 3)) * (M_PI / 180.0);

        float tx = (float)cos(rad) * 0.5 + 0.5;
        float ty = (float)sin(rad) * 0.5 + 0.5;

        rlTexCoord2f(0.5f, 0.5f);

        rlVertex2f(o.x, o.y);

        rlTexCoord2f(ty, 0);
    
        rlColor4ubX(0);
        rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);

        if (RSGL_args.texture != 1) {
            rlTexCoord2f(ty, tx);
            rlColor4ubX(0);

            rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
        }

        centralAngle += 360.0f/(float)sides;

        rlTexCoord2f(ty, tx);

        rlColor4ubX(1);
        rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
    }
    rlEnd();
    rlPopMatrix();

    if (RSGL_argsClear) RSGL_clearArgs();
}

void RSGL_drawPolygon(RSGL_rect o, unsigned int sides, RSGL_color c) { RSGL_drawPolygonPro(o, sides, (RSGL_point){0, (int)sides}, c); }

void RSGL_drawArc(RSGL_rect o, RSGL_point arc, RSGL_color color) {  RSGL_drawPolygonPro(o, 360, arc, color); }
void RSGL_drawCircle(RSGL_circle c, RSGL_color color) {  RSGL_drawPolygonPro((RSGL_rect){c.x, c.y, c.d, c.d}, 360, (RSGL_point){0, 360}, color); }
void RSGL_drawOval(RSGL_rect o, RSGL_color c) { RSGL_drawPolygonPro(o, 360, (RSGL_point){0, 360}, c); }

/* 
outlines
*/

void RSGL_drawLine(RSGL_point p1, RSGL_point p2, unsigned int thickness, RSGL_color c) {
    rlLineWidth(thickness);
    RSGL_pointF points[] = {{p1.x, p1.y}, {p2.x, p2.y}};
    RSGL_pointF texPoints[] = {{0, 0}, {0, 0}};

    RSGL_rect r = {p1.x, p1.y, (p2.x - p1.x), (p2.y - p1.y)};

    RSGL_BASIC_DRAW(RL_LINES, (RSGL_pointF*)points, (RSGL_pointF*)texPoints, r, c, sizeof(points)/sizeof(RSGL_point));
    rlLineWidth(1);
}

void RSGL_drawTriangleOutline(RSGL_triangle t, unsigned int thickness, RSGL_color c) {
    rlLineWidth(thickness);
    RSGL_pointF points[] = {{t.p3.x, t.p3.y}, {t.p1.x, t.p1.y}, {t.p1.x, t.p1.y}, {t.p2.x, t.p2.y}, {t.p2.x, t.p2.y}, {t.p3.x, t.p3.y}};
    RSGL_pointF texPoints[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

    RSGL_rect r = {t.p2.x, t.p3.y, abs(t.p2.x - t.p1.x), abs(t.p2.y - t.p3.y)};

    RSGL_BASIC_DRAW(RL_LINES, (RSGL_pointF*)points, (RSGL_pointF*)texPoints, r, c, sizeof(points)/sizeof(RSGL_point));
    rlLineWidth(1);
}
void RSGL_drawRectOutline(RSGL_rect r, unsigned int thickness, RSGL_color c) {
    RSGL_drawLine((RSGL_point){r.x, r.y}, (RSGL_point){r.x + r.w, r.y}, thickness, c);
    RSGL_drawLine((RSGL_point){r.x, r.y}, (RSGL_point){r.x, r.y + r.h}, thickness, c);
    RSGL_drawLine((RSGL_point){r.x, r.y + r.h}, (RSGL_point){r.x + r.w, r.y + r.h}, thickness, c);
    RSGL_drawLine((RSGL_point){r.x + r.w, r.y}, (RSGL_point){r.x + r.w, r.y + r.h}, thickness, c);
}
void RSGL_drawRoundRectOutline(RSGL_rect r, RSGL_point rounding, unsigned int thickness, RSGL_color c) {
    RSGL_drawRect((RSGL_rect) {r.x + (rounding.x/2), r.y, r.w - rounding.x, (int)(thickness + !thickness)}, c);
    RSGL_drawRect((RSGL_rect) {r.x + (rounding.x/2), r.y + r.h, r.w - rounding.x, (int)(thickness + !thickness)}, c);    
    RSGL_drawRect((RSGL_rect) {r.x, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y}, c);
    RSGL_drawRect((RSGL_rect) {r.x + r.w, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y}, c);

    RSGL_drawArcOutline((RSGL_rect) {r.x, r.y, rounding.x, rounding.y}, (RSGL_point){180, 270}, thickness, c);
    RSGL_drawArcOutline((RSGL_rect) {r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y}, (RSGL_point){90, 180}, thickness, c);
    RSGL_drawArcOutline((RSGL_rect) {r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y) - 1, rounding.x, rounding.y + 2}, (RSGL_point){0, 90}, thickness, c);
    RSGL_drawArcOutline((RSGL_rect) {r.x + 1, r.y  + (r.h - rounding.y) - 1,  rounding.x, rounding.y + 2}, (RSGL_point){270, 360}, thickness, c);
}

void RSGL_drawPolygonOutlinePro(RSGL_rect o, unsigned int sides, RSGL_point arc, RSGL_color c) {
    o = (RSGL_rect){o.x + (o.w / 2), o.y + (o.h / 2), o.w / 2, o.h / 2};
    float centralAngle = 0;

    glPrerequisites(o, c);
    rlBegin(RL_LINES);
        int i;
        for (i = 0; i < arc.y; i++) {
            rlColor4ub(c.r, c.g, c.b, c.a);

            if (i < arc.x ) {
                centralAngle += 360.0f/(float)sides;
                continue;
            }

            rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.w);
            centralAngle += 360.0f/(float)sides;
            rlVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
        }
    rlEnd();
    rlPopMatrix();
}

void RSGL_drawPolygonOutline(RSGL_rect o, unsigned int sides, unsigned int thickness, RSGL_color c) {
    rlLineWidth(thickness);
    RSGL_drawPolygonOutlinePro(o, sides, (RSGL_point){0, (int)sides}, c);
}
void RSGL_drawArcOutline(RSGL_rect o, RSGL_point arc, unsigned int thickness, RSGL_color color) {
    rlLineWidth(thickness);
    RSGL_drawPolygonOutlinePro(o, 360, arc, color);
}
void RSGL_drawCircleOutline(RSGL_circle c, unsigned int thickness, RSGL_color color) {
    rlLineWidth(thickness);
    RSGL_drawPolygonOutlinePro((RSGL_rect){c.x, c.y, c.d, c.d}, 360, (RSGL_point){0, 360}, color);
}
void RSGL_drawOvalOutline(RSGL_rect o, unsigned int thickness, RSGL_color c) {
    rlLineWidth(thickness);
    RSGL_drawPolygonOutlinePro(o, 360, (RSGL_point){0, 360}, c);
}

/* textures / images */
unsigned int RSGL_createTexture(unsigned char* bitmap, RSGL_area memsize, unsigned char channels) {
    return rlLoadTexture(bitmap, memsize.w, memsize.h, channels);
}

unsigned int RSGL_drawImage(const char* image, RSGL_rect r) {
    unsigned int texture = 0;

    #ifndef RSGL_NO_SAVE_IMAGE
    static size_t images_comp = 0;

    if (images_comp == 0) {
        RSGL_images = (RSGL_image*)malloc(sizeof(RSGL_image) * RSGL_INIT_IMAGES);
        images_comp = RSGL_INIT_IMAGES;
    }

    if (RSGL_images_len) {
        int i; 
        for (i = 0; i < RSGL_images_len; i++) {
            if (RSGL_cstr_equal(image, RSGL_images[i].img)) {
                texture = RSGL_images[i].tex;
                break;
            }
        }
    }
    #endif /* RSGL_NO_SAVE_IMAGE */

    if (!texture) {
        int x, y, c;
        unsigned char* bitmap = stbi_load(image, &x, &y, &c, 0);

        texture = RSGL_createTexture(bitmap, (RSGL_area){x, y}, c);

        #ifndef RSGL_NO_SAVE_IMAGE
        if (RSGL_images_len + 1 > images_comp) {
            RSGL_images = (RSGL_image*)realloc(RSGL_images, sizeof(RSGL_image) * (RSGL_NEW_IMAGES + images_comp));
            images_comp += RSGL_NEW_IMAGES;
        }

        RSGL_images[RSGL_images_len] = (RSGL_image){image, texture};
        RSGL_images_len++;
        #endif
    }

    if (r.w || r.h) {
        unsigned int tex = RSGL_args.texture;
        RSGL_setTexture(texture);

        RSGL_drawRect(r, RSGL_RGB(255, 255, 255));

        RSGL_args.texture = tex;
    }

    return texture;
}

#ifndef RSGL_NO_TEXT
unsigned int RSGL_loadFont(const char* font) {
    int loaded = fonsGetFontByName(RSGL_fonsContext, font);    

    if (loaded == -1)
        return fonsAddFont(RSGL_fonsContext, font);

    return loaded;
}

unsigned int RSGL_font;
void RSGL_setFont(unsigned int font) {
    RSGL_font = font;
}

void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color) {
    if (text == NULL || text[0] == '\0')
        return;
    int w = fonsTextWidth(RSGL_fonsContext, RSGL_font, c.d, text, strlen(text));

    glPrerequisites((RSGL_rect) {c.x, c.y + (c.d - (c.d/4)), w, c.d}, color);
  
    unsigned int fonsColor = glfonsRGBA(color.r, color.b, color.g, color.a);

    fonsDrawText(RSGL_fonsContext, RSGL_font, c.x, c.y + (c.d - (c.d / 4)), c.d, fonsColor, text);
    
    rlPopMatrix();
}

unsigned int RSGL_textWidth(const char* text, unsigned int fontSize, size_t textEnd) {
    return fonsTextWidth(RSGL_fonsContext, RSGL_font, fontSize, text, textEnd);
}
#endif /* RSGL_NO_TEXT */

void glPrerequisites(RSGL_rect r, RSGL_color c) {
    rlColor4ub(c.r, c.g, c.b, c.a);
    rlMatrixMode(RL_PROJECTION);
    rlLoadIdentity();
    rlPushMatrix();

    rlOrtho(0, RSGL_args.currentRect.w, RSGL_args.currentRect.h, 0, -RSGL_args.currentRect.w, RSGL_args.currentRect.w);
    
    rlTranslatef((r.x + (r.w / 2)), (r.x + (r.h / 2)), 0);
        
    rlRotatef(RSGL_args.rotate.x,  0, 0, 1);
    rlRotatef(RSGL_args.rotate.y, 0, 1, 0);
    rlRotatef(RSGL_args.rotate.z, 1, 0, 0);

    rlTranslatef(-(r.x + (r.w / 2)), -(r.x + (r.h / 2)), 0);

    rlMatrixMode(RL_MODELVIEW);
}

/*
******
RSGL_widgets
******
*/

#if !defined(RGFW_NO_WIDGETS) && defined (RGFW_mouseButtonPressed)
void RSGL_button_update(RSGL_button* b, RGFW_Event e) {
    switch (e.type) {
        case RGFW_mouseButtonPressed:
            if (RSGL_rectCollidePoint(b->r, (RSGL_point){e.x, e.y})) {
                if (b->s != RSGL_pressed)
                    b->toggle = !b->toggle;
                b->s = RSGL_pressed;
            }
            break;
        case RGFW_mouseButtonReleased:
            if (b->s == RSGL_pressed && RSGL_rectCollidePoint(b->r, (RSGL_point){e.x, e.y}))
                b->s = RSGL_hovered;
            else
                b->s = RSGL_none;
            break;
        case RGFW_mousePosChanged:
            if (RSGL_rectCollidePoint(b->r, (RSGL_point){e.x, e.y}))
                b->s = RSGL_hovered;
            else
                b->s = RSGL_none;
            break;
        default: break;
    }
}

void RSGL_ratio_button_update(RSGL_button* bArray, size_t len, RGFW_Event e) { 
    RSGL_button* b;
    int i;

    for (b = bArray; (b - bArray ) < len; b++) {
        RSGL_button_update(b, e);

        if (b->s != RSGL_pressed)
            continue;

        for (i = 0; i < len; i++)
            if (i != (b - bArray)) {
                bArray[i].s = RSGL_none;
                bArray[i].toggle = false;
            }
    }
}

void RSGL_slider_update(RSGL_button* b, RSGL_rect limits, RGFW_Event e) {
    RSGL_button_update(b, e);

    if (e.type == RGFW_mouseButtonReleased && b->toggle)
        b->toggle = false;
    
    if (!b->toggle || e.type != RGFW_mousePosChanged)
        return;


    if (limits.w && RSGL_between(e.x, limits.x, limits.x + limits.w))
        b->r.x = e.x;
    else if (limits.h && RSGL_between(e.y, limits.y, limits.y + limits.h)) 
        b->r.y = e.y;
}

#if !defined(RSGL_NO_TEXT) && defined(RGFW_keyPressed)

RSGL_textbox* RSGL_createTextbox(const char* text, unsigned int textSize, RSGL_rect box, RSGL_point cursor) {
    unsigned char* t;
    for (t = (unsigned char*)text; *t; t++);

    RSGL_textbox* tb = (RSGL_textbox*)malloc(sizeof(RSGL_textbox));
    tb->capacity = tb->len = t - (unsigned char*)text;

    if (tb->len)
        tb->text = strdup(text);
    else
        tb->text = (char*)malloc(sizeof(char));

    tb->textSize = textSize;
    tb->r = box;
    tb->cursor = cursor;

    return tb;
}

void RSGL_textbox_insert(RSGL_textbox* textBox, char ch) {
	size_t previous_len = textBox->len;
	size_t before_index_len = previous_len - textBox->index;

	textBox->len += 1;

	if (textBox->capacity < textBox->len) {
        textBox->text = (char*)realloc(textBox->text, textBox->len + 1);

        textBox->capacity += 1;
	}

	char* cur_str = textBox->text;

	char* ptr = (char*)memcpy(cur_str + textBox->len - before_index_len, cur_str + textBox->index, before_index_len);
	memcpy(cur_str + textBox->index, &ch, 1);
	ptr[before_index_len] = '\0';
}

void RSGL_textbox_erase(RSGL_textbox* textBox) {
	char* cur_str = textBox->text;

	size_t after_index_len = textBox->index + 1;

	char* ptr = (char*)memcpy(cur_str + textBox->index, cur_str + after_index_len, textBox->len - after_index_len);
	ptr[textBox->len - after_index_len] = '\0';

    free(textBox->text);
    textBox->text = strdup(cur_str);

	textBox->len--;
}

void RSGL_textbox_update(RSGL_textbox* textBox, RGFW_Event e) {
    switch (e.type) {
        case RGFW_keyPressed:
            switch (e.keyCode) {
                case RGFW_Left:
                    if (textBox->cursor.x) {
                        textBox->cursor.x--;
                        textBox->index--;
                    }
                    break;
                case RGFW_Right:
                    if (textBox->index < textBox->len) {
                        textBox->cursor.x++;
                        textBox->index++;
                    }
                    break;
                case RGFW_Up:
                    textBox->cursor = RSGL_POINT(0, textBox->cursor.y - 1);
                    break;
                case RGFW_Down:
                    textBox->cursor = RSGL_POINT(0, textBox->cursor.y + 1);
                    break;
                case RGFW_BackSpace:
                    if (textBox->text[textBox->index] == '\n')
                        textBox->cursor = RSGL_POINT(0, textBox->cursor.y - 1);
                    else
                        textBox->cursor.x--;

                    RSGL_textbox_erase(textBox);
                    textBox->index--;
                    break;
                default: {
                    char ch;
                    if (e.keyName[1] == '\0')
                        ch = e.keyName[0];
                    else
                        ch = RSGL_keyCodeToKeyChar(e.keyCode);

                    RSGL_textbox_insert(textBox, ch);

                    if (ch == '\n')
                        textBox->cursor = RSGL_POINT(0, textBox->cursor.y + 1);
                    else
                        textBox->cursor.x++;

                    textBox->index++;
                    break;
                }
            }
            break;
        default: break;
    }
}
void RSGL_textbox_draw(RSGL_textbox* textBox,   int font, RSGL_color c, RSGL_color cursorColor) {
    RSGL_setFont(font);
    RSGL_drawText(textBox->text, (RSGL_circle){textBox->r.x, textBox->r.y, textBox->textSize}, c);

    RSGL_point cursor = {
            textBox->r.x + (textBox->cursor.x ? RSGL_textWidth(textBox->text, textBox->textSize, textBox->cursor.x) : 0), 
            textBox->r.y + (textBox->cursor.y) * textBox->textSize
        };

    RSGL_drawLine(cursor, (RSGL_point){cursor.x, cursor.y + textBox->textSize}, 1, cursorColor);
}
void RSGL_textbox_free(RSGL_textbox* tb) {
    free(tb->text);
    free(tb);
}

#endif /* RSGL_NO_TEXT */

#endif /*  RGFW_NO_WIDGETS */

/*
******
RSGL_Audio
******
*/

#ifndef RSGL_NO_AUDIO


typedef struct RSGL_audioData {
    ma_decoder decoder;
    ma_result result;
    ma_device device;
} RSGL_audioData;


void RSGL_data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

void RSGL_audio_playFile(RSGL_audio* a, const char* file) {
    if (file[1] == '\0')
        return;

    a->data = (RSGL_audioData*)malloc(sizeof(RSGL_audioData));

    a->file = (char*)file;
    a->data->result = ma_decoder_init_file(file, NULL, &a->data->decoder);
    
    if (a->data->result != MA_SUCCESS)
        printf("Could not load file: %s\n", file);

    ma_device_config deviceConfig;
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = a->data->decoder.outputFormat;
    deviceConfig.playback.channels = a->data->decoder.outputChannels;
    deviceConfig.sampleRate        = a->data->decoder.outputSampleRate;
    deviceConfig.dataCallback      = RSGL_data_callback;
    deviceConfig.pUserData         = &a->data->decoder;

    if (ma_device_init(NULL, &deviceConfig, &a->data->device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&a->data->decoder);
    }

    if (ma_device_start(&a->data->device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&a->data->device);
        ma_decoder_uninit(&a->data->decoder);
    }
    
    RSGL_audio_play(*a);
}

void RSGL_audio_play(RSGL_audio a) {
    ma_device_start(&a.data->device);
    if (RSGL_audio_position(a) == RSGL_audio_len(a))
        RSGL_audio_seek(a, 0);
}
void RSGL_audio_pause(RSGL_audio a) {
    ma_device_stop(&a.data->device);
}
void RSGL_audio_stop(RSGL_audio a) {
    pause();
    ma_device_uninit(&a.data->device);
    ma_decoder_uninit(&a.data->decoder);
}

/* write audio info */
void RSGL_audio_setVolume(RSGL_audio a, unsigned int vol) {
    a.data->result = ma_device_set_master_volume(&a.data->device, vol / 100.0f);
}
void RSGL_audio_seek(RSGL_audio a, unsigned int position) {
    a.data->result = ma_decoder_seek_to_pcm_frame(&a.data->decoder, position * 44100);
}

/* get audio info */
unsigned int RSGL_audio_len(RSGL_audio a) {
    long long unsigned int len;
    ma_decoder_get_length_in_pcm_frames(&a.data->decoder, &len);
    return len / 44100;
}

unsigned int RSGL_audio_volume(RSGL_audio a) {
    float volume;
    a.data->result = ma_device_get_master_volume(&a.data->device, &volume);
    return volume * 100;
}
unsigned int RSGL_audio_position(RSGL_audio a) {
    long long unsigned int pos;
    ma_decoder_get_cursor_in_pcm_frames(&a.data->decoder, &pos);
    return pos / 44100;
}
bool RSGL_audio_isPlaying(RSGL_audio a) {
    return ma_device_is_started(&a.data->device);
}

void RSGL_audio_free(RSGL_audio a) {
    ma_device_uninit(&a.data->device);
    ma_decoder_uninit(&a.data->decoder);
}

void RSGL_data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {    
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }
    
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
}

#endif /* RSGL_NO_AUDIO */

/* 
******
RSGL_Other
******
*/

/* timers */
bool RSGL_wait(unsigned int miliseconds) {
    static int start_time = -1, miliLimit = 0;

    if (start_time == -1)
        start_time = time(0);

    if (miliseconds > miliLimit)
        miliLimit = miliseconds;

    int passed = time(0) - start_time;

    if (passed > miliLimit)
        start_time = time(0);

    return !(passed % miliseconds) && passed;
}

bool RSGL_wait_frames(unsigned int frames) {
    static int i = 0, frameLimit = 0;
    
    if (frames > frameLimit)
        frameLimit = frames;

    if (i > frameLimit)
        i = 0;

    i++;

    return !(i % frames);
}

char RSGL_keyCodeToKeyChar(unsigned int keycode) {
    switch (keycode) {  
        case RGFW_Backtick: return '`';
        case RGFW_Minus: return '-';
        case RGFW_Equals: return '=';
        case RGFW_Space: return ' ';
        case RGFW_Period: return '.';
        case RGFW_Comma: return ',';
        case RGFW_Slash: return '/';
        case RGFW_Bracket: return '{';
        case RGFW_CloseBracket: return '}';
        case RGFW_Semicolon: return ';';
        case RGFW_Return: return '\n';
        case RGFW_Quote: return '\"';
        case RGFW_BackSlash: return '\\';
        case RGFW_KP_Slash: return '/';
        case RGFW_Multiply: return '*';
        case RGFW_KP_Minus: return '-';
        case RGFW_KP_1: return '1';
        case RGFW_KP_2: return '2';
        case RGFW_KP_3: return '3';
        case RGFW_KP_4: return '4';
        case RGFW_KP_5: return '5';
        case RGFW_KP_6: return '6';
        case RGFW_KP_7: return '7';
        case RGFW_KP_8: return '8';
        case RGFW_KP_9: return '9';
        case RGFW_KP_0: return '0';
        case RGFW_KP_Period: return '.';
        case RGFW_KP_Return: return '\n';
        default: return '\0';
    }
}

/* collision detection */
bool RSGL_circleCollidePoint(RSGL_circle c, RSGL_point p) { return RSGL_circleCollideRect(c, (RSGL_rect) {p.x, p.y, 1, 1}); }
bool RSGL_circleCollideRect(RSGL_circle c, RSGL_rect r) {
    // test cords
    float testX = c.x; 
    float testY = c.y;

    // fill cords based on x/ys of the shapes
    if (c.x < r.x)
      testX = r.x;  

    else if (c.x > r.x+r.w) 
      testX = r.x-r.w;

    if (c.y < r.y)  
      testY = r.y;  

    else if (c.y > r.y+r.h)
      testY = r.y+r.h; 
    
    // check
    return ( sqrt( ( (c.x - testX) * (c.x - testX) ) + ( (c.y - testY) * (c.y - testY) ) )  <= (c.d/2) );
}
bool RSGL_circleCollide(RSGL_circle cir, RSGL_circle cir2) {
    float distanceBetweenCircles = (float) sqrt(
        (cir2.x - cir.x) * (cir2.x - cir.x) + 
        (cir2.y - cir.y) * (cir2.y - cir.y)
    );

    return !(distanceBetweenCircles > (cir.d/2) + (cir2.d/2)); // check if there is a collide
}
bool RSGL_rectCollidePoint(RSGL_rect r, RSGL_point p){ return (p.x >= r.x &&  p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h); }
bool RSGL_rectCollide(RSGL_rect r, RSGL_rect r2){ return (r.x + r.w >= r2.x && r.x <= r2.x + r2.w && r.y + r.h >= r2.y && r.y <= r2.y + r2.h); }
bool RSGL_pointCollide(RSGL_point p, RSGL_point p2){ return (p.x == p2.x && p.y == p2.y); }
#endif /* RSGL_IMPLEMENTATION */