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
    (MAKE SURE RSGL_IMPLEMENTATION is in exactly one header or you use -DRSGL_IMPLEMENTATION)
	#define RSGL_IMPLEMENTATION - makes it so source code is included with header
    
    #define RSGL_NO_WINDOW - no RSGL_window, RSGL_graphics is used instead [this is for using a differnt window manager other than RGFW ]
    #define RSGL_NO_TEXT - do not include text rendering functions
    #define RGFW_NO_WIDGETS - do not include widgets
    #define RSGL_NO_AUDIO - do not include audio functions
    #define RSGL_NO_MINIAUDIO_IMPLEMENTATION - do not have `#define MINIAUDIO_IMPLEMENTATION` in this header (you'll have to link miniaudio some other way to use audio)
    #define RSGL_NO_SAVE_IMAGE - do not save/load images (don't use RSGL_drawImage if you use this), 
                                    RSGL_drawImage saves the file name + texture so it can load it
                                    when you ask for it later. This disables that 
    #define RSGL_INIT_FONTS [number of fonts] - set hFow much room should be pre-allocated for fonts by fontstash
                                                this avoids performance issues related to realloc
                                                RSGL_INIT_FONTS = 4 by default
    #define RSGL_INIT_IMAGES [number of fonts] - set how much room should be pre-allocated for images by RSGL
                                                this avoids performance issues related to realloc
                                                RSGL_INIT_IMAGES = 20 by default
    #define RSGL_NEW_IMAGES [number of fonts] - set how much room should be reallocated at a time for images by RSGL
                                                this avoids performance issues related to realloc
                                                RSGL_NEW_IMAGES 10 by default

    #define RSGL_LEGACY_OPENGL - use legacy opengl functions
        
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

#ifndef RSGL_INIT_FONTS
#define RSGL_INIT_FONTS 4
#endif
#ifndef RSGL_NEW_FONTS
#define RSGL_NEW_FONTS 2
#endif
#ifndef RSGL_INIT_IMAGES
#define RSGL_INIT_IMAGES 20
#endif
#ifndef RSGL_NEW_IMAGES
#define RSGL_NEW_IMAGES 10
#endif

/*! Optional arguments for making a windows */
#define RSGL_TRANSPARENT_WINDOW		(1L<<9) /*!< If the window is transparent*/
#define RSGL_NO_BORDER		(1L<<3) /*!< If the window doesn't have border*/
#define RSGL_NO_RESIZE		(1L<<4) /*!< If the window cannot be resized  by the user*/
#define RSGL_ALLOW_DND     (1L<<5) /*!< if the window supports drag and drop*/
#define RSGL_HIDE_MOUSE (1L<<6) /* if the window should hide the mouse or not (can be toggled later on)*/
#define RSGL_OPENGL (1L<<7) /* use normal opengl (if another version is also selected) */
#define RSGL_FULLSCREEN (1L<<8) /* if the window should be fullscreen by default or not */
#define RSGL_CENTER (1L<<10)
#define RSGL_OPENGL_SOFTWARE (1L<<11) /* use OpenGL software rendering */
/*! event codes */
#define RSGL_keyPressed 2 /*!< a key has been pressed*/
#define RSGL_keyReleased 3 /*!< a key has been released*/
#define RSGL_mouseButtonPressed 4 /*!< a mouse button has been pressed (left,middle,right)*/
#define RSGL_mouseButtonReleased 5 /*!< a mouse button has been released (left,middle,right)*/
#define RSGL_mousePosChanged 6 /*!< the position of the mouse has been changed*/
#define RSGL_jsButtonPressed 7 /*!< a joystick button was pressed */
#define RSGL_jsButtonReleased 8 /*!< a joystick button was released */
#define RSGL_jsAxisMove 9 /*!< an axis of a joystick was moved*/
#define RSGL_windowAttribsChange 10 /*!< the window was moved or resized (by the user) */
#define RSGL_quit 33 /*!< the user clicked the quit button*/ 
#define RSGL_dnd 34 /*!< a file has been dropped into the window*/
#define RSGL_dnd_init 35 /*!< the start of a dnd event, when the place where the file drop is known */

/*! mouse button codes */
#define RSGL_mouseLeft  1 /*!< left mouse button is pressed*/
#define RSGL_mouseMiddle  2 /*!< mouse-wheel-button is pressed*/
#define RSGL_mouseRight  3 /*!< right mouse button is pressed*/
#define RSGL_mouseScrollUp  4 /*!< mouse wheel is scrolling up*/
#define RSGL_mouseScrollDown  5 /*!< mouse wheel is scrolling down*/

#define RSGL_JS_A RGFW_JS_A /* or PS X button */
#define RSGL_JS_B RGFW_JS_B /* or PS circle button */
#define RSGL_JS_Y RGFW_JS_Y /* or PS triangle button */
#define RSGL_JS_X RGFW_JS_X /* or PS square button */
#define RSGL_JS_START RGFW_JS_START /* start button */
#define RSGL_JS_SELECT RGFW_JS_SELECT /* select button */
#define RSGL_JS_HOME RGFW_JS_HOME /* home button */
#define RSGL_JS_UP RGFW_JS_UP /* dpad up */
#define RSGL_JS_DOWN RGFW_JS_DOWN /* dpad down*/
#define RSGL_JS_LEFT RGFW_JS_LEFT /* dpad left */
#define RSGL_JS_RIGHT RGFW_JS_RIGHT /* dpad right */
#define RSGL_JS_L1 RGFW_JS_L1 /* left bump */
#define RSGL_JS_L2 RGFW_JS_L2 /* left trigger*/
#define RSGL_JS_R1 RGFW_JS_R1 /* right bumper */
#define RSGL_JS_R2 RGFW_JS_R2 /* right trigger */

/* 
RSGL GL defs
*/

#define RSGL_BLEND				0x0BE2
#define RSGL_BLEND_SRC				0x0BE1
#define RSGL_BLEND_DST				0x0BE0
#define RSGL_ZERO					0
#define RSGL_ONE					1
#define RSGL_SRC_COLOR				0x0300
#define RSGL_ONE_MINUS_SRC_COLOR			0x0301
#define RSGL_SRC_ALPHA				0x0302
#define RSGL_ONE_MINUS_SRC_ALPHA			0x0303
#define RSGL_DST_ALPHA				0x0304
#define RSGL_ONE_MINUS_DST_ALPHA			0x0305
#define RSGL_DST_COLOR				0x0306
#define RSGL_ONE_MINUS_DST_COLOR			0x0307
#define RSGL_SRC_ALPHA_SATURATE			0x0308

#define RSGL_TEXTURE_SWIZZLE_R              0x8E42
#define RSGL_TEXTURE_SWIZZLE_G              0x8E43
#define RSGL_TEXTURE_SWIZZLE_B              0x8E44
#define RSGL_TEXTURE_SWIZZLE_A              0x8E45
#define RSGL_TEXTURE_SWIZZLE_RGBA           0x8E46

/* this can be used in RSGL_basicDraw */
#define RSGL_LINES                                0x0001      /* GL_LINES */
#define RSGL_TRIANGLES                            0x0004      /* GL_TRIANGLES */
#define RSGL_QUADS                                0x0007      /* GL_QUADS */

/* these ensure GL_DEPTH_TEST is disabled when they're being rendered */
#define RSGL_LINES_2D                                0x0011      /* GL_LINES */
#define RSGL_TRIANGLES_2D                            0x0014      /* GL_TRIANGLES */
#define RSGL_QUADS_2D                                0x0017      /* GL_QUADS */

/* 
keys = 
RGFW_{key} 

keys will not be reincluded into RSGL
*/

#ifndef RSGL_H
#define RSGL_H

#ifndef RSGLDEF
#ifdef __APPLE__
#define RSGLDEF extern inline
#else
#define RSGLDEF inline
#endif
#endif

#define RGFWDEF RSGLDEF

#if !defined(u8)
    #include <stdint.h>

    typedef uint8_t     u8;
	typedef int8_t      i8;
	typedef uint16_t   u16;
	typedef int16_t    i16;
	typedef uint32_t   u32;
	typedef int32_t    i32;
	typedef uint64_t   u64;
	typedef int64_t    i64;
#endif

typedef struct RSGL_rect {
    i32 x, y, w, h;
} RSGL_rect;
#define RSGL_RECT(x, y, w, h) (RSGL_rect){x, y, w, h}

typedef struct RSGL_rectF { float x, y, w, h; } RSGL_rectF;
#define RSGL_RECTF(x, y, w, h) (RSGL_rectF){x, y, w, h}

#define RSGL_between(x, lower, upper) (((lower) <= (x)) && ((x) <= (upper)))

#ifndef RSGL_NO_WINDOW
#define RGFW_RECT RSGL_rect
#define RGFW_EXTRA_CONTEXT
#define GL_SILENCE_DEPRECATION
#include "deps/RGFW.h"
#else
typedef struct {u32 type, x, y;} RGFW_Event;
#endif


#include <stdbool.h>
#include <stddef.h>

/* 
*******
RSGL_[shape]
*******
*/

typedef struct RSGL_point {
    i32 x, y;
} RSGL_point;
#define RSGL_POINT(x, y) (RSGL_point){x, y}

typedef struct RSGL_pointF { float x, y; } RSGL_pointF;
#define RSGL_POINTF(x, y) (RSGL_pointF){x, y}

typedef struct RSGL_point3D {
    i32 x, y, z;
} RSGL_point3D;

typedef struct RSGL_point3DF { float x, y, z; } RSGL_point3DF;

#define RSGL_POINT3D(x, y, z) (RSGL_point3D){x, y, z}
#define RSGL_POINT3DF(x, y, z) (RSGL_point3DF){x, y, z}


typedef struct RSGL_area {
    i32 w, h;
} RSGL_area;
#define RSGL_AREA(w, h) (RSGL_area){w, h}

typedef struct RSGL_areaF { float w, h;} RSGL_areaF;
#define RSGL_AREAF(w, h) (RSGL_areaF){w, h}

typedef struct RSGL_circle {
    i32 x, y, d;
} RSGL_circle;
#define RSGL_CIRCLE(x, y, d) (RSGL_circle){x, y, d}

typedef struct RSGL_circleF { float x, y, d; } RSGL_circleF;
#define RSGL_CIRCLEF(x, y, d) (RSGL_circleF){x, y, d}

typedef struct RSGL_triangle {
    RSGL_point p1, p2, p3;
} RSGL_triangle;
#define RSGL_TRIANGLE(p1, p2, p3) (RSGL_triangle){p1, p2, p3}

typedef struct RSGL_triangleF { RSGL_pointF p1, p2, p3; } RSGL_triangleF;
#define RSGL_TRIANGLEF(p1, p2, p3) (RSGL_triangleF){p1, p2, p3}

#define RSGL_createTriangle(x1, y1, x2, y2, x3, y3) (RSGL_triangle){{x1, y1}, {x2, y2}, {x3, y3}}
#define RSGL_createTriangleF(x1, y1, x2, y2, x3, y3) (RSGL_triangleF){{x1, y1}, {x2, y2}, {x3, y3}}

typedef struct RSGL_cube {
    i32 x, y, z, w, h, l;
} RSGL_cube;
#define RSGL_CUBE(x, y, z, w, h, l) (RSGL_cube){x, y, z, w, h, l}

typedef struct RSGL_cubeF { float x, y, z, w, h, l; } RSGL_cubeF;
#define RSGL_CUBEF(x, y, z, w, h, l) (RSGL_cubeF){x, y, z, w, h, l}

/* 
the color stucture is in 
ABGR by default for performance reasons 
(converting color to hex for example)
*/
typedef struct RSGL_color {
    u8 a, b, g, r;
} RSGL_color;

#define RSGL_RGBA(r, g, b, a) ((RSGL_color){(a), (b), (g), (r)})
#define RSGL_RGB(r, g, b) ((RSGL_color){255, (b), (g), (r)})

#define RSGL_COLOR_TO_HEX(color) ((u32)(color) & 0xFFFFFF00)
#define RSGL_RGB_TO_HEX(r, g, b, a) (RSGL_COLOR_TO_HEX(RSGL_RGBA(r, g, b, a)))
#define RSGL_RGBA_TO_HEX(r, g, b) (RSGL_COLOR_TO_HEX(RSGL_RGB(r, g, b, a)))

/*
******
RGFW_FUNCTION_DEFINES
******
*/

#define RSGL_isPressedI RGFW_isPressedI
#define RSGL_isPressedS RGFW_isPressedS
#define RSGL_readClipboard RGFW_readClipboard
#define RSGL_writeClipboard RGFW_writeClipboard
#define RSGL_registerJoystickF RGFW_registerJoystickF
#define RSGL_registerJoystick RGFW_registerJoystick
#define RSGL_isPressedJS RGFW_isPressedJS
#define RSGL_getMaxGLVersion RGFW_getMaxGLVersion
#define RSGL_setGLVersion RGFW_setGLVersion
#define RSGL_window_swapInterval RGFW_window_swapInterval
#define RSGL_getProcAddress RGFW_getProcAddress
#define RSGL_window_checkFPS RGFW_window_checkFPS
#define RSGL_window_setMinSize RGFW_window_setMinSize
#define RSGL_window_setMaxSize RGFW_window_setMaxSize
#define RSGL_window_maximize RGFW_window_maximize
#define RSGL_window_minimize RGFW_window_minimize
#define RSGL_window_restore RGFW_window_restore
#define RSGL_window_shouldClose RGFW_window_shouldClose
#define RSGL_window_isFullscreen RGFW_window_isFullscreen
#define RSGL_window_isHidden RGFW_window_isHidden
#define RSGL_isMinimized RGFW_isMinimized
#define RSGL_isMaximized RGFW_isMaximized
#define RSGL_window_swapBuffers RGFW_window_swapBuffers
#define RSGL_window_hideMouse RGFW_window_hideMouse
#define RSGL_createThread RGFW_createThread

RSGLDEF void RSGL_legacy(i32 legacy);

/* 
*******
RSGL_window
*******
*/

#ifndef RSGL_NO_WINDOW
typedef RGFW_window RSGL_window;

RSGLDEF RSGL_window* RSGL_createWindow(const char* name, RSGL_rect r, u64 args);

RSGLDEF void RSGL_window_setIconImage(RSGL_window* win, const char* image); 
#define RGFW_window_setIcon RSGL_window_setIcon

#define RSGL_window_checkEvent RGFW_window_checkEvent

RSGLDEF void RSGL_window_makeCurrent(RSGL_window* win);

RSGLDEF void RSGL_window_clear(RSGL_window* win, RSGL_color color);

RSGLDEF void RSGL_window_close(RSGL_window* win);

RSGLDEF RSGL_point RSGL_window_getGlobalMousePoint(RSGL_window* win);

#define RSGL_window_screenSize RGFW_window_screenSize
#define RSGL_window_move RGFW_window_move
#define RSGL_window_resize RGFW_window_resize
#define RSGL_window_setName RGFW_window_setName
#define RSGL_window_setMouse RGFW_window_setMouse
#define RSGL_window_moveMouse RGFW_window_moveMouse
#define RSGL_window_setMouseDefault RGFW_window_setMouseDefault
#else /* RSGL_NO_WINDOW */

/* 
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/

RSGLDEF void RSGL_initGraphics(RSGL_area r, void* loader);
RSGLDEF void RSGL_graphics_clear(RSGL_color c);
RSGLDEF void RSGL_graphics_free();

#endif /* RSGL_GRAPHICS_CONTEXT / !RSGL_NO_WINDOW */

/* 
*******
RSGL_draw
*******
*/


/* RSGL_draw args */
RSGLDEF void RSGL_rotate(RSGL_point3D rotate);
RSGLDEF void RSGL_setTexture(u32 texture);
RSGLDEF void RSGL_setGradient(RSGL_color gradient[], size_t len);
RSGLDEF void RSGL_fill(bool fill);

/* args clear after a draw function by default, this toggles that */
RSGLDEF void RSGL_setClearArgs(bool clearArgs); /* toggles if args are cleared by default or not */
RSGLDEF void RSGL_clearArgs(); /* clears the args */

RSGLDEF void RSGL_basicDraw(u32 RGL_TYPE, RSGL_point3DF* points, RSGL_pointF* texPoints, RSGL_rectF rect, RSGL_color c, size_t len);
/* 2D shape drawing */

RSGLDEF void RSGL_drawPoint(RSGL_point p, RSGL_color c);
RSGLDEF void RSGL_drawPointF(RSGL_pointF p, RSGL_color c);

RSGLDEF void RSGL_drawPoint3D(RSGL_point3D p, RSGL_color c);
RSGLDEF void RSGL_drawPoint3DF(RSGL_point3DF p, RSGL_color c);

RSGLDEF void RSGL_drawTriangle(RSGL_triangle t, RSGL_color c);
RSGLDEF void RSGL_drawTriangleF(RSGL_triangleF t, RSGL_color c);

RSGLDEF void RSGL_drawTriangleHyp(RSGL_pointF p, size_t angle, float hypotenuse, RSGL_color color);

RSGLDEF void RSGL_drawRect(RSGL_rect r, RSGL_color c);
RSGLDEF void RSGL_drawRectF(RSGL_rectF r, RSGL_color c);


RSGLDEF void RSGL_drawRoundRect(RSGL_rect r, RSGL_point rounding, RSGL_color c);
RSGLDEF void RSGL_drawRoundRectF(RSGL_rectF r, RSGL_point rounding, RSGL_color c);

RSGLDEF void RSGL_drawPolygon(RSGL_rect r, u32 sides, RSGL_color c);
RSGLDEF void RSGL_drawPolygonF(RSGL_rectF r, u32 sides, RSGL_color c);

RSGLDEF void RSGL_drawArc(RSGL_rect o, RSGL_point arc, RSGL_color color);
RSGLDEF void RSGL_drawArcF(RSGL_rectF o, RSGL_pointF arc, RSGL_color color);

RSGLDEF void RSGL_drawCircle(RSGL_circle c, RSGL_color color);
RSGLDEF void RSGL_drawCircleF(RSGL_circleF c, RSGL_color color);

RSGLDEF void RSGL_drawOval(RSGL_rect o, RSGL_color c);
RSGLDEF void RSGL_drawOvalF(RSGL_rectF o, RSGL_color c);

RSGLDEF void RSGL_drawLine(RSGL_point p1, RSGL_point p2, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawLineF(RSGL_pointF p1, RSGL_pointF p2, u32 thickness, RSGL_color c);

/* 2D outlines */

RSGLDEF void RSGL_drawTriangleOutline(RSGL_triangle t, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawTriangleFOutline(RSGL_triangleF t, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawRectOutline(RSGL_rect r, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawRectFOutline(RSGL_rectF r, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawRoundRectOutline(RSGL_rect r, RSGL_point rounding, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawRoundRectFOutline(RSGL_rectF r, RSGL_point rounding, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawPolygonOutline(RSGL_rect r, u32 sides, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawPolygonFOutline(RSGL_rectF r, u32 sides, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawArcOutline(RSGL_rect o, RSGL_point arc, u32 thickness, RSGL_color color);
RSGLDEF void RSGL_drawArcFOutline(RSGL_rectF o, RSGL_pointF arc, u32 thickness, RSGL_color color);

RSGLDEF void RSGL_drawCircleOutline(RSGL_circle c, u32 thickness, RSGL_color color);
RSGLDEF void RSGL_drawCircleFOutline(RSGL_circleF c, u32 thickness, RSGL_color color);

RSGLDEF void RSGL_drawOvalFOutline(RSGL_rectF o, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawOvalOutline(RSGL_rect o, u32 thickness, RSGL_color c);

/* 3D shaoe drawing */
RSGLDEF void RSGL_drawCube(RSGL_cube r, RSGL_color c);
RSGLDEF void RSGL_drawCubeF(RSGL_cubeF, RSGL_color c);

#ifndef RSGL_NO_TEXT
RSGLDEF const char* RFont_fmt(const char* string, ...);
#define RSGL_strFmt RFont_fmt

RSGLDEF u32 RSGL_loadFont(const char* font);
#define RSGL_FONT(str) RSGL_loadFont(str)

RSGLDEF void RSGL_setFont(u32 font);

typedef struct RFont_font RFont_font;
RSGLDEF void RSGL_setRFont(RFont_font* font);

RSGLDEF void RSGL_drawFPS(RGFW_window* win, RSGL_circle c, RSGL_color color);
RSGLDEF void RSGL_drawText_len(const char* text, size_t len, RSGL_circle c, RSGL_color color);
RSGLDEF void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color);
#define RSGL_drawTextF(text, font, c, color) \
    RSGL_setFont(font);\
    RSGL_drawText(text, c, color);

RSGLDEF u32 RSGL_textWidth(const char* text, u32 fontSize, size_t textEnd);
#define RSGL_textWidthF(text, fontSize, textEnd) \
    RSGL_setFont(font);\
    RSGL_textWidthF(text, fontSize, textEnd);
#endif /* RSGL_NO_TEXT */

RSGLDEF u32 RSGL_createTexture(u8* bitmap, RSGL_area memsize,    u8 channels);
RSGLDEF u32 RSGL_drawImage(const char* image, RSGL_rect r);

#define RSGL_loadImage(image) RSGL_drawImage(image, (RSGL_rect){})

#define RSGL_deleteTexture(texture) glDeleteTextures(1, &texture);
#define RSGL_deleteTextures(texture, num) glDeleteTextures(num, &texture);

RSGLDEF void RSGL_pushPixelValues(i32 alignment, i32 rowLength, i32 skipPixels, i32 skipRows);
RSGLDEF void RSGL_textureSwizzleMask(u32 atlas, u32 param, i32 swizzleRgbaParams[4]);
RSGLDEF void RSGL_atlasAddBitmap(u32 atlas, u8* bitmap, float x, float y, float w, float h);

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
    RSGL_rect r;
    RSGL_buttonStatus s;
    bool toggle; /* for toggle buttons */
} RSGL_button;

/* these functions should be run in an event loop */
RSGLDEF void RSGL_button_update(
    RSGL_button* b, /* button pointer */
    RGFW_Event e /* current event */
);

RSGLDEF void RSGL_ratio_button_update(
    RSGL_button* bArray, /* array of ratio buttons (if you have an array of general buttons I suggest you do something like `buttons + x`) */
    size_t arrayLen, /* size of the array, or last button to update in the array */
    RGFW_Event e
);

RSGLDEF void RSGL_slider_update(
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

    RSGL_button box;
    size_t textSize;
    size_t len, cap;

    i32 x;
    RSGL_point cursor;

    bool autoScroll;
    bool toggle;
} RSGL_textbox;


RSGLDEF RSGL_textbox* RSGL_createTextbox(RSGL_rect box, u32 textSize, bool autoScroll);
RSGLDEF RSGL_textbox* RSGL_createTextbox_ex(const char* text, RSGL_rect box, u32 textSize,  RSGL_point cursor, bool autoScroll);

RSGLDEF void RSGL_textbox_update(RSGL_textbox* texbox, RSGL_window* win);
RSGLDEF void RSGL_textbox_draw(RSGL_textbox* textBox, RSGL_color c, RSGL_color cursorColor);

RSGLDEF void RSGL_textbox_free(RSGL_textbox* tb);
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
    struct RSGL_audioData* data;
    
    char* file;
    bool loop;
    bool ptr; /* if the audio struct is a pointer (so it can be freed latre on) */
} RSGL_audio;

void RSGL_audio_loadFile(RSGL_audio* a, const char* file);
RSGLDEF void RSGL_audio_playFile(RSGL_audio* a, const char* file);
RSGLDEF void RSGL_audio_play(RSGL_audio a);
RSGLDEF void RSGL_audio_pause(RSGL_audio a);
RSGLDEF void RSGL_audio_stop(RSGL_audio a);
RSGLDEF void RSGL_audio_free(RSGL_audio a);

/* write audio info */
RSGLDEF void RSGL_audio_setVolume(RSGL_audio a, u32);
RSGLDEF void RSGL_audio_seek(RSGL_audio a, u32 position);

/* get audio info */
RSGLDEF u32 RSGL_audio_len(RSGL_audio a);
RSGLDEF u32 RSGL_audio_volume(RSGL_audio a);
RSGLDEF u32 RSGL_audio_position(RSGL_audio a);
RSGLDEF bool RSGL_audio_isPlaying(RSGL_audio a);

#endif /* RSGL_NO_AUDIO */

/* 
*******
extra
*******
*/

/* wait functions */
RSGLDEF bool RSGL_wait(u32 miliseconds);
RSGLDEF bool RSGL_wait_frames(u32 frames);
RSGLDEF char RSGL_keyCodeToKeyChar(u32 keycode);

/* ** collision functions ** */
RSGLDEF bool RSGL_circleCollidePoint(RSGL_circle c, RSGL_point p);
RSGLDEF bool RSGL_circleCollideRect(RSGL_circle c, RSGL_rect r);
RSGLDEF bool RSGL_circleCollide(RSGL_circle cir1, RSGL_circle cir2);
RSGLDEF bool RSGL_rectCollidePoint(RSGL_rect r, RSGL_point p);
RSGLDEF bool RSGL_rectCollide(RSGL_rect r, RSGL_rect r2);
RSGLDEF bool RSGL_pointCollide(RSGL_point p, RSGL_point p2);

RSGLDEF bool RSGL_circleCollidePointF(RSGL_circleF c, RSGL_pointF p);
RSGLDEF bool RSGL_circleCollideRectF(RSGL_circleF c, RSGL_rectF r);
RSGLDEF bool RSGL_circleCollideF(RSGL_circleF cir1, RSGL_circleF cir2);
RSGLDEF bool RSGL_rectCollidePointF(RSGL_rectF r, RSGL_pointF p);
RSGLDEF bool RSGL_rectCollideF(RSGL_rectF r, RSGL_rectF r2);
RSGLDEF bool RSGL_pointCollideF(RSGL_pointF p, RSGL_pointF p2);

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

        if (win->event.type == RSGL_quit)
            break;

        RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
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

    Alternatively, you also can find pre-built binaries for Silicon at
    https://github.com/ColleagueRiley/Silicon/tree/binaries

	ex.
	gcc main.c -framework Foundation -lSilicon -framework AppKit -framework CoreVideo -ISilicon/include

	I also suggest you compile Silicon (and RGFW if applicable)
	per each time you compile your application so you know that everything is compiled for the same architecture.
*/

#ifdef RSGL_IMPLEMENTATION

#ifdef RSGL_LEGACY_OPENGL
#define RGL_OPENGL_LEGACY
#define RFONT_RENDER_LEGACY
#endif

#define RGFW_IMPLEMENTATION
#define RGL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <assert.h>

#include "deps/RGL.h"

#ifndef RSGL_NO_WINDOW
#define GL_SILENCE_DEPRECATION
#include "deps/RGFW.h"
#endif

#ifndef RSGL_NO_TEXT
#define RFONT_IMPLEMENTATION
#define RFONT_RENDER_RGL

#include "deps/RFont.h"
#endif /* RSGL_NO_TEXT */

#include "deps/stb_image.h"

#include <time.h>

#ifndef RSGL_NO_AUDIO

#ifndef RSGL_NO_MINIAUDIO_IMPLEMENTATION
#define MINIAUDIO_IMPLEMENTATION
#endif /* RSGL_NO_MINIAUDIO_IMPLEMENTATION */
#include "deps/miniaudio.h"
#endif /* RSGL_NO_AUDIO */

#include <stdbool.h>

/* RSGL_args */
typedef struct RSGL_drawArgs {
    RSGL_color gradient[600]; 
    
    u32 texture;
    u32 gradient_len;

    RSGL_rect currentRect; /* size of current window */
    RSGL_point3D rotate; 

    bool fill;
} RSGL_drawArgs;

RSGL_drawArgs RSGL_args = {{ }, 1, 0, { }, {0, 0, 0}, 1};
bool RSGL_argsClear = false;

#ifndef RSGL_NO_TEXT
typedef struct RSGL_fontData {
    char* name;
    RFont_font* f;
} RSGL_fontData;

typedef struct RSGL_fontsData {
    RFont_font* f;
    
    RSGL_fontData* fonts;
    size_t len;
    size_t cap;
} RSGL_fontsData;

RSGL_fontsData RSGL_font = {NULL, NULL, 0, 0};
#endif

u32 RSGL_windowsOpen = 0;

typedef struct { const char* img; u32 tex;} RSGL_image;
RSGL_image* RSGL_images = NULL;
size_t RSGL_images_len = 0;

RSGLDEF void glPrerequisites(RSGL_rectF r, RSGL_color c);

RSGLDEF bool RSGL_cstr_equal(const char* str, const char* str2);
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


void RSGL_basicDraw(u32 RGL_TYPE, RSGL_point3DF* points, RSGL_pointF* texPoints, RSGL_rectF rect, RSGL_color c, size_t len) {  
    i32 i;

    glPrerequisites(rect, c);
        rglBegin(RGL_TYPE);
            for (i = 0; i < len; i++) {
                if (i && i <= RSGL_args.gradient_len)
                    rglColor4ub(RSGL_args.gradient[i - 1].r, RSGL_args.gradient[i - 1].g, RSGL_args.gradient[i - 1].b, RSGL_args.gradient[i - 1].a);
                
                rglTexCoord2f(texPoints[i].x, texPoints[i].y);
                rglVertex3f(points[i].x, points[i].y, points[i].z);
            }
        rglEnd();
    rglPopMatrix();
    rglPopMatrix();

    if (RSGL_argsClear) {
        RSGL_setTexture(0);
        RSGL_clearArgs();
    }
}

void RSGL_legacy(i32 legacy) {
    rglLegacy(legacy);
    RFont_render_legacy(legacy);
}

/* 
    RSGL_window
*/

#ifndef RSGL_NO_WINDOW

RSGL_window* RSGL_createWindow(const char* name, RSGL_rect r, u64 args) {
    RGFW_window* win = RGFW_createWindow(name, r.x, r.y, r.w, r.h, args);

    if (RSGL_windowsOpen == 0) {
        #ifndef GRAPHICS_API_OPENGL_11
        rglInit((void*)RGFW_getProcAddress);
        rglRenderBatch();      // Update and draw internal render batch
        #endif

        #ifndef RGFW_RECT
        glViewport(0, 0, win->w, win->h);
        #else /* n RGFW_RECT */
        glViewport(0, 0, win->r.w, win->r.h);
        #endif /* RGFW_RECT */
        
        // Init state: Blending mode
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      // Color blending function (how colors are mixed)
        glEnable(GL_BLEND);                                     // Enable color blending (required to work with transparencies)

        glCullFace(GL_BACK);                                    // Cull the back face (default)
        glFrontFace(GL_CCW);                                    // Front face are defined counter clockwise (default)
        glEnable(GL_CULL_FACE);        

        rglMatrixMode(RGL_PROJECTION);    // Switch to projection matrix
        rglPushMatrix();                 // Save previous matrix, which contains the settings for the 2d ortho projection
        rglLoadIdentity();               // Reset current matrix (projection)

        RSGL_args.rotate = (RSGL_point3D){0, 0, 0}; 

        #ifndef RSGL_NO_TEXT
        RFont_init(win->r.w, win->r.h);
        RSGL_font.fonts = (RSGL_fontData*)malloc(sizeof(RSGL_fontData) * RSGL_INIT_FONTS); 
        #endif
    }

    assert(win != NULL);
    RSGL_windowsOpen++;

    return win;
}

void RSGL_window_setIconImage(RGFW_window* win, const char* image) {
    i32 x, y, c;
    u8* img = stbi_load(image, &x, &y, &c, 0);

    RGFW_window_setIcon(win, img, x, y, c);
    
    free(img);
}

void RSGL_window_makeCurrent(RSGL_window* win) {
    RGFW_window_makeCurrent(win);

    RSGL_args.currentRect = win->r;
}

void RSGL_window_clear(RSGL_window* win, RSGL_color color) {
    #ifndef RGFW_RECT
    glViewport(0, 0, win->w, win->h);
    #else /* n RGFW_RECT */
    glViewport(0, 0, win->r.w, win->r.h);
    #endif /* RGFW_RECT */
 
    RSGL_window_makeCurrent(win);
    RGFW_window_swapBuffers(win);

    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    
    rglRenderBatch();
}

void RSGL_window_close(RSGL_window* win) {
    RSGL_windowsOpen--;

    if (RSGL_windowsOpen == 0) {
        u32 i;
        #ifndef RSGL_NO_TEXT
        for (i = 0; i < RSGL_font.len; i++)
            RFont_font_free(RSGL_font.fonts[i].f);

        free(RSGL_font.fonts);
        #endif

        #ifndef GRAPHICS_API_OPENGL_11
        rglClose();
        #endif
        
        for (i = 0; i < RSGL_images_len; i++)
            glDeleteTextures(1, &RSGL_images[i].tex);
    }

    RGFW_window_close(win);
}

RSGL_point RSGL_window_getGlobalMousePoint(RSGL_window* win) {
    int* mouse = RGFW_window_getGlobalMousePoint(win);

    return RSGL_POINT(mouse[0], mouse[1]);
}

#else /* !RGFW_NO_WINDOW */

/* 
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/


void RSGL_initGraphics(RSGL_area r, void* loader) {
    #ifndef GRAPHICS_API_OPENGL_11
    rglInit(r.w, r.h, loader);

    // Init state: Blending mode
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      // Color blending function (how colors are mixed)
    glEnable(GL_BLEND);                                     // Enable color blending (required to work with transparencies)

    glCullFace(GL_BACK);                                    // Cull the back face (default)
    glFrontFace(GL_CCW);                                    // Front face are defined counter clockwise (default)
    glEnable(GL_CULL_FACE);        

    rglMatrixMode(RGL_PROJECTION);    // Switch to projection matrix
    rglPushMatrix();                 // Save previous matrix, which contains the settings for the 2d ortho projection
    rglLoadIdentity();               // Reset current matrix (projection)
    #endif

    RSGL_args.currentRect = (RSGL_rect){0, 0, r.w, r.h};

    #ifndef RSGL_NO_TEXT
    RFont_init(r.w, r.h);
    #endif
}

void RSGL_graphics_clear(RSGL_color color) {
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    rglRenderBatch();
}

void RSGL_graphics_free() {
    u32 i;
    #ifndef RSGL_NO_TEXT
    for (i = 0; i < RSGL_font.len; i++)
        RFont_font_free(RSGL_font.fonts[i].f);

    free(RSGL_font.fonts);
    #endif

    #ifndef GRAPHICS_API_OPENGL_11
    rglClose();
    #endif

    if (RSGL_images != NULL) {
        i32 i;
        for (i = 0; i < RSGL_images_len; i++)
            glDeleteTextures(1, RSGL_images[i].tex);
        free(RSGL_images);
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
void RSGL_setTexture(u32 texture) { 
    RSGL_args.texture = texture;
    rglSetTexture(texture); 
}

void RSGL_setGradient(RSGL_color gradient[], size_t len) {
    RSGL_args.gradient_len = len;
    
    i32 i;
    for (i = 0; i < RSGL_args.gradient_len; i++)
        RSGL_args.gradient[i] = gradient[i];
}
void RSGL_fill(bool fill) {
    RSGL_args.fill = fill;
}
void RSGL_setClearArgs(bool clearArgs) {
    RSGL_argsClear = clearArgs;
}
void RSGL_clearArgs() {
    RSGL_args.rotate = (RSGL_point3D){0, 0, 0}; 
    RSGL_args.gradient_len = 0;    
}


void RSGL_drawPoint(RSGL_point p, RSGL_color c) {
    RSGL_drawPointF((RSGL_pointF){(float)p.x, (float)p.y}, c);
}

void RSGL_drawPoint3D(RSGL_point3D p, RSGL_color c) {
    RSGL_drawPoint3DF((RSGL_point3DF){(float)p.x, (float)p.y, (float)p.z}, c);
}

void RSGL_drawTriangle(RSGL_triangle t, RSGL_color c) {
    RSGL_drawTriangleF(RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y), c);
}

void RSGL_drawRect(RSGL_rect r, RSGL_color c) {
    RSGL_drawRectF((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, c);
}

void RSGL_drawRoundRect(RSGL_rect r, RSGL_point rounding, RSGL_color c) {
    RSGL_drawRoundRectF((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding, c);
}

void RSGL_drawPolygon(RSGL_rect r, u32 sides, RSGL_color c) {
    RSGL_drawPolygonF((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides, c);
}

void RSGL_drawArc(RSGL_rect o, RSGL_point arc, RSGL_color color) {
    RSGL_drawArcF((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y}, color);
}

void RSGL_drawCircle(RSGL_circle c, RSGL_color color) {
    RSGL_drawCircleF((RSGL_circleF){(float)c.x, (float)c.y, (float)c.d}, color);
}

void RSGL_drawOval(RSGL_rect o, RSGL_color c) {
    RSGL_drawOvalF((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, c);
}

void RSGL_drawLine(RSGL_point p1, RSGL_point p2, u32 thickness, RSGL_color c) {
    RSGL_drawLineF((RSGL_pointF){(float)p1.x, (float)p1.y}, (RSGL_pointF){(float)p2.x, (float)p2.y}, thickness, c);
}

void RSGL_drawTriangleOutline(RSGL_triangle t, u32 thickness, RSGL_color c) {
    RSGL_drawTriangleFOutline(RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y), thickness, c);
}

void RSGL_drawRectOutline(RSGL_rect r, u32 thickness, RSGL_color c) {
    RSGL_drawRectFOutline((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, thickness, c);
}

void RSGL_drawRoundRectOutline(RSGL_rect r, RSGL_point rounding, u32 thickness, RSGL_color c) {
    RSGL_drawRoundRectFOutline((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding, thickness, c);
}

void RSGL_drawPolygonOutline(RSGL_rect r, u32 sides, u32 thickness, RSGL_color c) {
    RSGL_drawPolygonFOutline((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides, thickness, c);
}

void RSGL_drawArcOutline(RSGL_rect o, RSGL_point arc, u32 thickness, RSGL_color color) {
    RSGL_drawArcFOutline((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y}, thickness, color);
}

void RSGL_drawCircleOutline(RSGL_circle c, u32 thickness, RSGL_color color) {
    RSGL_drawCircleFOutline((RSGL_circleF){(float)c.x, (float)c.y, (float)c.d}, thickness, color);
}

void RSGL_drawOvalOutline(RSGL_rect o, u32 thickness, RSGL_color c) {
    RSGL_drawOvalFOutline((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, thickness, c);
}

void RSGL_drawCube(RSGL_cube r, RSGL_color c) {
    RSGL_drawCubeF((RSGL_cubeF){(float)r.x, (float)r.y, (float)r.z, (float)r.w, (float)r.h, (float)r.l}, c);
}

void RSGL_drawPointF(RSGL_pointF p, RSGL_color c) {
    RSGL_drawRectF((RSGL_rectF){p.x, p.y, 1.0f, 1.0f}, c);
}

void RSGL_drawPoint3DF(RSGL_point3DF p, RSGL_color c) { RSGL_drawCubeF((RSGL_cubeF){p.x, p.y, p.z, 1, 1, 1}, c); }

void RSGL_drawTriangleF(RSGL_triangleF t, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawTriangleFOutline(t, 1, c);

    RSGL_point3DF points[] = {{(float)t.p1.x, (float)t.p1.y, 0.0f}, {(float)t.p2.x, (float)t.p2.y, 0.0f}, {(float)t.p2.x, (float)t.p2.y}, {(float)t.p3.x, (float)t.p3.y, 0.0f}};
    RSGL_pointF texPoints[] = {  
                {((float)(t.p3.x - t.p1.x)/t.p2.x < 1) ? (float)(t.p3.x - t.p1.x) / t.p2.x : 0, 0.0f}, 
                {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
    };
    
    RSGL_rectF r = {t.p2.x, t.p3.y, fabsf(t.p2.x - t.p1.x), fabsf(t.p2.y - t.p3.y)};

    RSGL_basicDraw(RGL_QUADS_2D, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, r, c, 4);
}

void RSGL_drawTriangleHyp(RSGL_pointF p, size_t angle, float hypotenuse, RSGL_color color) {
    float dir = (hypotenuse > 0);
    hypotenuse = fabsf(hypotenuse);

    float base = hypotenuse * (cos(angle) * RAD2DEG);
    float opp = hypotenuse * (cos(angle) * RAD2DEG); 
    
    RSGL_triangleF t = RSGL_TRIANGLEF(
        p,
        RSGL_POINTF(p.x + base, p.y),
        RSGL_POINTF(p.x + (base * dir), p.y - opp)
    );
    
    RSGL_drawTriangleF(t, color);   
}

void RSGL_drawRectF(RSGL_rectF r, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawRectFOutline(r, 1, c);

    RSGL_pointF texPoints[] = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};
    RSGL_point3DF points[] = {{r.x, r.y, 0.0f}, {r.x, r.y + r.h, 0.0f}, {r.x + r.w, r.y + r.h, 0.0f}, {r.x + r.w, r.y, 0.0f}};

    RSGL_basicDraw(RGL_QUADS_2D, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, r, c, 4);
}

void RSGL_drawRoundRectF(RSGL_rectF r, RSGL_point rounding, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawRoundRectFOutline(r, rounding, 1, c);

    RSGL_drawRect((RSGL_rect) {r.x + (rounding.x/2), r.y, r.w - rounding.x, r.h}, c);
    RSGL_drawRect((RSGL_rect) {r.x, r.y + (rounding.y/2), r.w,  r.h - rounding.y}, c);

    RSGL_drawArc((RSGL_rect) {r.x, r.y, rounding.x, rounding.y}, (RSGL_point){180, 270}, c);
    RSGL_drawArc((RSGL_rect) {r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y}, (RSGL_point){90, 180}, c);
    RSGL_drawArc((RSGL_rect) {r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y) - 1, rounding.x, rounding.y + 2}, (RSGL_point){0, 90}, c);
    RSGL_drawArc((RSGL_rect) {r.x + 1, r.y  + (r.h - rounding.y) - 1,  rounding.x, rounding.y + 2}, (RSGL_point){270, 360}, c);
}

#define rglColor4ubX(x) if (RSGL_args.gradient_len >= 1) rglColor4ub(RSGL_args.gradient[x].r, RSGL_args.gradient[x].g, RSGL_args.gradient[x].b, RSGL_args.gradient[x].a);

void RSGL_drawPolygonFOutlinePro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c);

void RSGL_drawPolygonFPro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawPolygonFOutlinePro(o, sides, arc, c);
    
    o = (RSGL_rectF){o.x + (o.w / 2), o.y + (o.h / 2), o.w / 2, o.h / 2};
    float centralAngle = 0;

    i32 x = 0;

    glPrerequisites(o, c);
    
    rglBegin(RSGL_args.texture != 1 ? RGL_QUADS_2D : RGL_TRIANGLES_2D);
    
    for (x = 0; (x / 3) < arc.y; x += 3) {
        if ((x / 3) < arc.x) { 
            centralAngle += 360.0f/(float)sides;
            continue;
        }

        float rad = (360 / sides * (x / 3)) * (M_PI / 180.0);

        float tx = (float)cos(rad) * 0.5 + 0.5;
        float ty = (float)sin(rad) * 0.5 + 0.5;

        rglTexCoord2f(0.5f, 0.5f);

        rglVertex2f(o.x, o.y);

        rglTexCoord2f(ty, 0);
    
        rglColor4ubX(0);
        rglVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
        
        if (RSGL_args.texture != 1 && RSGL_args.texture) {
            rglTexCoord2f(ty, tx);
            rglColor4ubX(0);

            rglVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
        }

        centralAngle += 360.0f / (float)sides;

        rglTexCoord2f(ty, tx);

        rglColor4ubX(1);
        rglVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
    }
    rglEnd();
    rglPopMatrix();
    rglPopMatrix();

    if (RSGL_argsClear) RSGL_clearArgs();
}

void RSGL_drawPolygonF(RSGL_rectF o, u32 sides, RSGL_color c) { RSGL_drawPolygonFPro(o, sides, (RSGL_pointF){0, (int)sides}, c); }

void RSGL_drawArcF(RSGL_rectF o, RSGL_pointF arc, RSGL_color color) {  
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_drawPolygonFPro(o, verts, arc, color); 
}

void RSGL_drawCircleF(RSGL_circleF c, RSGL_color color) {  
    float verts = ((2 * M_PI * c.d) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_drawPolygonFPro((RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, verts}, color); 
}

void RSGL_drawOvalF(RSGL_rectF o, RSGL_color c) { 
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_drawPolygonFPro(o, verts, (RSGL_pointF){0, verts}, c); 
}

/* 
outlines
*/

void RSGL_drawLineF(RSGL_pointF p1, RSGL_pointF p2, u32 thickness, RSGL_color c) {
    rglLineWidth(thickness);
    RSGL_point3DF points[] = {{p1.x, p1.y, 0.0f}, {p2.x, p2.y, 0.0f}};
    RSGL_pointF texPoints[] = {{0, 0.0f}, {0, 0.0f}};

    RSGL_rectF r = {p1.x, p1.y, (p2.x - p1.x), (p2.y - p1.y)};

    RSGL_basicDraw(RGL_LINES_2D, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, r, c, 2);
}

void RSGL_drawTriangleFOutline(RSGL_triangleF t, u32 thickness, RSGL_color c) {
    rglLineWidth(thickness);
    RSGL_point3DF points[] = {{t.p3.x, t.p3.y, 0.0f}, {t.p1.x, t.p1.y, 0.0f}, {t.p1.x, t.p1.y, 0.0f}, {t.p2.x, t.p2.y, 0.0f}, {t.p2.x, t.p2.y, 0.0f}, {t.p3.x, t.p3.y, 0.0f}};
    RSGL_pointF texPoints[] = {{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}};

    RSGL_rectF r = {t.p2.x, t.p3.y, fabsf(t.p2.x - t.p1.x), fabsf(t.p2.y - t.p3.y)};

    RSGL_basicDraw(RGL_LINES_2D, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, r, c, 6);
}
void RSGL_drawRectFOutline(RSGL_rectF r, u32 thickness, RSGL_color c) {
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x + r.w, r.y}, thickness, c);
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x, r.y + r.h}, thickness, c);
    RSGL_drawLineF((RSGL_pointF){r.x, r.y + r.h}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness, c);
    RSGL_drawLineF((RSGL_pointF){r.x + r.w, r.y}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness, c);
}
void RSGL_drawRoundRectFOutline(RSGL_rectF r, RSGL_point rounding, u32 thickness, RSGL_color c) {
    RSGL_drawRectF((RSGL_rectF) {r.x + (rounding.x/2), r.y, r.w - rounding.x, (int)(thickness + !thickness)}, c);
    RSGL_drawRectF((RSGL_rectF) {r.x + (rounding.x/2), r.y + r.h, r.w - rounding.x, (int)(thickness + !thickness)}, c);    
    RSGL_drawRectF((RSGL_rectF) {r.x, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y}, c);
    RSGL_drawRectF((RSGL_rectF) {r.x + r.w, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y}, c);

    RSGL_drawArcFOutline((RSGL_rectF) {r.x, r.y, rounding.x, rounding.y}, (RSGL_pointF){180, 270}, thickness, c);
    RSGL_drawArcFOutline((RSGL_rectF) {r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y}, (RSGL_pointF){90, 180}, thickness, c);
    RSGL_drawArcFOutline((RSGL_rectF) {r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y) - 1, rounding.x, rounding.y + 2}, (RSGL_pointF){0, 90}, thickness, c);
    RSGL_drawArcFOutline((RSGL_rectF) {r.x + 1, r.y  + (r.h - rounding.y) - 1,  rounding.x, rounding.y + 2}, (RSGL_pointF){270, 360}, thickness, c);
}

void RSGL_drawPolygonFOutlinePro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c) {
    o = (RSGL_rectF){o.x + (o.w / 2), o.y + (o.h / 2), o.w / 2, o.h / 2};
    float centralAngle = 0;
    
    glPrerequisites(o, c);
        i32 i;

        for (i = 0; i < arc.y; i++) {
    rglBegin(RGL_LINES_2D);
            rglColor4ub(c.r, c.g, c.b, c.a);

            if (i < arc.x ) {
                centralAngle += 360.0f/(float)sides;
                continue;
            }

            rglVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.w);
            centralAngle += 360.0f/(float)sides;
            rglVertex2f(o.x + sinf(DEG2RAD * centralAngle) * o.w, o.y + cosf(DEG2RAD * centralAngle) * o.h);
        }
        rglEnd();
    rglPopMatrix();
    rglPopMatrix();
}

void RSGL_drawPolygonFOutline(RSGL_rectF o, u32 sides, u32 thickness, RSGL_color c) {
    rglLineWidth(thickness);
    RSGL_drawPolygonFOutlinePro(o, sides, (RSGL_pointF){0, (int)sides}, c);
}
void RSGL_drawArcFOutline(RSGL_rectF o, RSGL_pointF arc, u32 thickness, RSGL_color color) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    rglLineWidth(thickness);
    RSGL_drawPolygonFOutlinePro(o, verts, arc, color);
}
void RSGL_drawCircleFOutline(RSGL_circleF c, u32 thickness, RSGL_color color) {
    float verts = ((2 * M_PI * c.d) / 10);
    verts = (verts > 360 ? 360 : verts);

    rglLineWidth(thickness);
    RSGL_drawPolygonFOutlinePro((RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, verts}, color);
}
void RSGL_drawOvalFOutline(RSGL_rectF o, u32 thickness, RSGL_color c) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    rglLineWidth(thickness);
    RSGL_drawPolygonFOutlinePro(o, verts, (RSGL_pointF){0, verts}, c);
}

/* 3D shaoe drawing */
void RSGL_drawCubeF(RSGL_cubeF r, RSGL_color color) {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    rglPushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        rglTranslatef(r.x, r.y, r.z);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(1.0f, 1.0f, 1.0f);   // NOTE: Vertices are directly scaled on definition

        rglBegin(RGL_TRIANGLES);
            rglColor4ub(color.r, color.g, color.b, color.a);

            // Front face
            rglVertex3f(x - r.w / 2, y - r.h / 2, z + r.l / 2);  // Bottom Left
            rglVertex3f(x + r.w / 2, y - r.h / 2, z + r.l / 2);  // Bottom Right
            rglVertex3f(x - r.w / 2, y + r.h / 2, z + r.l / 2);  // Top Left

            rglVertex3f(x + r.w / 2, y + r.h / 2, z + r.l / 2);  // Top Right
            rglVertex3f(x - r.w / 2, y + r.h / 2, z + r.l / 2);  // Top Left
            rglVertex3f(x + r.w / 2, y - r.h / 2, z + r.l / 2);  // Bottom Right

            // Back face
            rglVertex3f(x - r.w / 2, y - r.h / 2, z - r.l / 2);  // Bottom Left
            rglVertex3f(x - r.w / 2, y + r.h / 2, z - r.l / 2);  // Top Left
            rglVertex3f(x + r.w / 2, y - r.h / 2, z - r.l / 2);  // Bottom Right

            rglVertex3f(x + r.w / 2, y + r.h / 2, z - r.l / 2);  // Top Right
            rglVertex3f(x + r.w / 2, y - r.h / 2, z - r.l / 2);  // Bottom Right
            rglVertex3f(x - r.w / 2, y + r.h / 2, z - r.l / 2);  // Top Left

            // Top face
            rglVertex3f(x - r.w / 2, y + r.h / 2, z - r.l / 2);  // Top Left
            rglVertex3f(x - r.w / 2, y + r.h / 2, z + r.l / 2);  // Bottom Left
            rglVertex3f(x + r.w / 2, y + r.h / 2, z + r.l / 2);  // Bottom Right

            rglVertex3f(x + r.w / 2, y + r.h / 2, z - r.l / 2);  // Top Right
            rglVertex3f(x - r.w / 2, y + r.h / 2, z - r.l / 2);  // Top Left
            rglVertex3f(x + r.w / 2, y + r.h / 2, z + r.l / 2);  // Bottom Right

            // Bottom face
            rglVertex3f(x - r.w / 2, y - r.h / 2, z - r.l / 2);  // Top Left
            rglVertex3f(x + r.w / 2, y - r.h / 2, z + r.l / 2);  // Bottom Right
            rglVertex3f(x - r.w / 2, y - r.h / 2, z + r.l / 2);  // Bottom Left

            rglVertex3f(x + r.w / 2, y - r.h / 2, z - r.l / 2);  // Top Right
            rglVertex3f(x + r.w / 2, y - r.h / 2, z + r.l / 2);  // Bottom Right
            rglVertex3f(x - r.w / 2, y - r.h / 2, z - r.l / 2);  // Top Left

            // Right face
            rglVertex3f(x + r.w / 2, y - r.h / 2, z - r.l / 2);  // Bottom Right
            rglVertex3f(x + r.w / 2, y + r.h / 2, z - r.l / 2);  // Top Right
            rglVertex3f(x + r.w / 2, y + r.h / 2, z + r.l / 2);  // Top Left

            rglVertex3f(x + r.w / 2, y - r.h / 2, z + r.l / 2);  // Bottom Left
            rglVertex3f(x + r.w / 2, y - r.h / 2, z - r.l / 2);  // Bottom Right
            rglVertex3f(x + r.w / 2, y + r.h / 2, z + r.l / 2);  // Top Left

            // Left face
            rglVertex3f(x - r.w / 2, y - r.h / 2, z - r.l / 2);  // Bottom Right
            rglVertex3f(x - r.w / 2, y + r.h / 2, z + r.l / 2);  // Top Left
            rglVertex3f(x - r.w / 2, y + r.h / 2, z - r.l / 2);  // Top Right

            rglVertex3f(x - r.w / 2, y - r.h / 2, z + r.l / 2);  // Bottom Left
            rglVertex3f(x - r.w / 2, y + r.h / 2, z + r.l / 2);  // Top Left
            rglVertex3f(x - r.w / 2, y - r.h / 2, z - r.l / 2);  // Bottom Right
        rglEnd();
    rglPopMatrix();
    rglPopMatrix();

    /*

    // Front face  (z = 1.0f)
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);I
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
        RSGL_basicDraw(RL_QUADS, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, RSGL_RECT(r.x, r.y, r.w, r.h), c, sizeof(points)/sizeof(RSGL_point));*/
}

/* textures / images */
u32 RSGL_createTexture(u8* bitmap, RSGL_area memsize, u8 channels) {
    return rglCreateTexture(bitmap, memsize.w, memsize.h, channels);
}

void RSGL_pushPixelValues(i32 alignment, i32 rowLength, i32 skipPixels, i32 skipRows) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skipRows);
}

void RSGL_textureSwizzleMask(u32 atlas, u32 param, i32 swizzleRgbaParams[4]) {
   glBindTexture(GL_TEXTURE_2D, atlas);

   glTexParameteriv(GL_TEXTURE_2D, param, swizzleRgbaParams);

   glBindTexture(GL_TEXTURE_2D, 0);
}

void RSGL_atlasAddBitmap(u32 atlas, u8* bitmap, float x, float y, float w, float h) {
    glEnable(GL_TEXTURE_2D);

    i32 alignment, rowLength, skipPixels, skipRows;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowLength);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skipPixels);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skipRows);

    glBindTexture(GL_TEXTURE_2D, atlas);

    RFont_push_pixel_values(1, w, 0, 0);

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_UNSIGNED_BYTE, bitmap);

    RFont_push_pixel_values(alignment, rowLength, skipPixels, skipRows);

    glBindTexture(GL_TEXTURE_2D, 0);
}

u32 RSGL_drawImage(const char* image, RSGL_rect r) {
    u32 texture = 0;

    #ifndef RSGL_NO_SAVE_IMAGE
    static size_t images_comp = 0;

    if (images_comp == 0) {
        RSGL_images = (RSGL_image*)malloc(sizeof(RSGL_image) * RSGL_INIT_IMAGES);
        images_comp = RSGL_INIT_IMAGES;
    }

    if (RSGL_images_len) {
        i32 i; 
        for (i = 0; i < RSGL_images_len; i++) {
            if (RSGL_cstr_equal(image, RSGL_images[i].img)) {
                texture = RSGL_images[i].tex;
                break;
            }
        }
    }
    #endif /* RSGL_NO_SAVE_IMAGE */

    if (!texture) {
        i32 x, y, c;
        u8* bitmap = stbi_load(image, &x, &y, &c, 0);
        
        texture = RSGL_createTexture(bitmap, (RSGL_area){x, y}, c);

        free(bitmap);

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
        u32 tex = RSGL_args.texture;
        RSGL_setTexture(texture);

        RSGL_drawRect(r, RSGL_RGB(255, 255, 255));

        RSGL_args.texture = tex;
    }

    return texture;
}

#ifndef RSGL_NO_TEXT


u32 RSGL_loadFont(const char* font) {
    u32 i;
    for (i = 0; i < RSGL_font.len; i++) 
        if (RSGL_font.fonts[i].name == font)
            return i;
    
    if (RSGL_font.len == RSGL_font.cap) {
        RSGL_font.cap += RSGL_NEW_FONTS;

        RSGL_fontData* nFonts = (RSGL_fontData*)malloc(sizeof(RSGL_fontData) * RSGL_font.cap);
        memcpy(nFonts, RSGL_font.fonts, sizeof(RSGL_fontData) * RSGL_font.len);
        free(RSGL_font.fonts);

        RSGL_font.fonts = nFonts;
    }


    RSGL_font.fonts[RSGL_font.len].name = (char*)font; 
    RSGL_font.fonts[RSGL_font.len].f = RFont_font_init(font);
    RSGL_font.len++;

    return RSGL_font.len - 1;
}

void RSGL_setFont(u32 font) {
    RSGL_font.f = RSGL_font.fonts[font].f;
}

void RSGL_setRFont(RFont_font* font) {
    RSGL_font.f = font;
}

void RSGL_drawFPS(RGFW_window* win, RSGL_circle c, RSGL_color color) {
    RSGL_drawText(RSGL_strFmt("FPS : %i", win->event.fps), c, color);
}

void RSGL_drawText_len(const char* text, size_t len, RSGL_circle c, RSGL_color color) {
    glEnable(GL_BLEND);

    if (text == NULL || text[0] == '\0')
        return;

    RFont_set_color(color.r / 255.0f, color.b / 255.0f, color.g / 255.0f, color.a / 255.0f);
    RFont_draw_text_len(RSGL_font.f, text, len, c.x, c.y, c.d, 0.0f);
}

void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color) {
    RSGL_drawText_len(text, 0, c, color);
}

u32 RSGL_textWidth(const char* text, u32 fontSize, size_t textEnd) {
    return RFont_text_width_len(RSGL_font.f, text, textEnd, fontSize, 0.0);
}
#endif /* RSGL_NO_TEXT */

void glPrerequisites(RSGL_rectF r, RSGL_color c) {
    rglColor4ub(c.r, c.g, c.b, c.a);
    rglMatrixMode(RGL_PROJECTION);
    rglLoadIdentity();
    rglPushMatrix();
    
    rglMatrixMode(RGL_MODELVIEW);
    rglPushMatrix();

    rglOrtho(0, RSGL_args.currentRect.w, RSGL_args.currentRect.h, 0, -RSGL_args.currentRect.w, RSGL_args.currentRect.w);

    rglTranslatef((r.x + (r.w / 2)), (r.x + (r.h / 2)), 0);
    rglRotatef(RSGL_args.rotate.z,  0, 0, 1);
    rglRotatef(RSGL_args.rotate.y, 0, 1, 0);
    rglRotatef(RSGL_args.rotate.x, 1, 0, 0);

    rglTranslatef(-(r.x + (r.w / 2)), -(r.x + (r.h / 2)), 0);
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
    i32 i;

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

RSGL_textbox* RSGL_createTextbox(RSGL_rect box, u32 textSize, bool autoScroll) {
    return RSGL_createTextbox_ex(NULL, box, textSize, RSGL_POINT(0, 0), autoScroll);
}


RSGL_textbox* RSGL_createTextbox_ex(const char* text, RSGL_rect box, u32 textSize,  RSGL_point cursor, bool autoScroll) {
    RSGL_textbox* textBox = (RSGL_textbox*)malloc(sizeof(RSGL_textbox));
    textBox->box.r = box;
    textBox->textSize = 20;
    textBox->toggle = false;

    textBox->text = (char*)malloc(sizeof(char) * 1058 * 8);
    
    textBox->len = 0;
    textBox->cap = 1058 * 8;

    textBox->text[0] = '\0';
    textBox->autoScroll = false;
    
    textBox->cursor = cursor;
    textBox->x = 0;

    if (text != NULL) {
        size_t len = strlen(text);
        strncpy(textBox->text, text, len);

        textBox->len = len;
    }

    return textBox;
}

void RSGL_textbox_update(RSGL_textbox* textBox, RSGL_window* win) {
    RSGL_button_update(&textBox->box, win->event);

    if (win->event.type == RSGL_mouseButtonPressed)
        textBox->toggle = (textBox->box.s == RSGL_pressed);

    if (win->event.type != RSGL_keyPressed)
        return;
    
    if (RSGL_isPressedI(win, RGFW_Left)) {
        if (textBox->cursor.x)
            textBox->cursor.x--;
        
        if (textBox->text[textBox->cursor.x] == '\n')
            textBox->cursor.y--;
    }
    
    if (RSGL_isPressedI(win, RGFW_Right) && textBox->cursor.x < textBox->len) {
        textBox->cursor.x++;

        //if (textBox->text[textBox->cursor.x - 1] == '\n')
        //    textBox->cursor.y++;
    }

    if (RSGL_isPressedI(win, RGFW_Up) && textBox->cursor.y) {
        u32 x;
        for (x = textBox->cursor.x; x >= 0; x--)
            if (textBox->text[x] == '\n') {
                textBox->cursor.x = x;
                textBox->cursor.y--;
                break;
            }
    }
    
    if (RSGL_isPressedI(win, RGFW_Down)) {
        u32 x;
        for (x = textBox->cursor.x; x < textBox->textSize && textBox->text[x]; x++)
            if (textBox->text[x] == '\n') {
                textBox->cursor.x = x;
                textBox->cursor.y++;
                break;
            }
    }

    if (textBox->toggle == false)
        return;
    
    char ch =  RGFW_keystrToChar(win->event.keyName);

    size_t index, lineCount = 1;
    for (index = 0; (ch == '\n') && textBox->len > index; index++)
        if (textBox->text[index] == '\n')
            lineCount++;
    
    if (ch == '\0')
        return;
    
    if (
        (RSGL_textWidth(textBox->text + textBox->x, textBox->textSize, textBox->len) + textBox->box.r.x >= textBox->box.r.w && ch != '\n') ||
        (ch == '\n' && lineCount * textBox->textSize >= textBox->box.r.h)
    ) {
        for (i32 i = 0; i < 2 && (textBox->autoScroll && RSGL_textWidth(textBox->text + textBox->x, textBox->textSize, textBox->len) + textBox->box.r.x >= textBox->box.r.w && ch != '\n'); i++) 
            textBox->x++;
        
        return;
    }

    if (ch == '\n')
        textBox->cursor.y++;

    if (textBox->len != textBox->cursor.x)
        memcpy(textBox->text + textBox->cursor.x + 1, textBox->text + textBox->cursor.x, textBox->len - textBox->cursor.x);
    
    if (textBox->len + 1 > textBox->cap) {
        textBox->cap += 40;
        textBox->text = (char*)realloc(textBox->text, sizeof(char) * textBox->cap);
    }

    textBox->text[textBox->cursor.x] = ch;
    textBox->len++;
    textBox->text[textBox->len] = '\0';
    textBox->cursor.x++;
}

void RSGL_textbox_draw(RSGL_textbox* textBox, RSGL_color c, RSGL_color cursorColor) {
    if (textBox->toggle)
        RSGL_drawRect(RSGL_RECT(textBox->box.r.x + (textBox->cursor.x ? RSGL_textWidth(textBox->text + textBox->box.r.x, textBox->textSize, textBox->cursor.x) : 0), (textBox->box.r.y + 2) + (textBox->cursor.y * 20), 1, 16), cursorColor);

    RSGL_drawText(textBox->text + textBox->x, RSGL_CIRCLE(textBox->box.r.x, textBox->box.r.y - 5, textBox->textSize), c);

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

void RSGL_audio_loadFile(RSGL_audio* a, const char* file) {
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
}

void RSGL_audio_playFile(RSGL_audio* a, const char* file) {
    RSGL_audio_loadFile(a, file);
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
    ma_device_stop(&a.data->device);
    ma_device_uninit(&a.data->device);
    ma_decoder_uninit(&a.data->decoder);
}

/* write audio info */
void RSGL_audio_setVolume(RSGL_audio a, u32 vol) {
    a.data->result = ma_device_set_master_volume(&a.data->device, vol / 100.0f);
}
void RSGL_audio_seek(RSGL_audio a, u32 position) {
    a.data->result = ma_decoder_seek_to_pcm_frame(&a.data->decoder, position * 44100);
}

/* get audio info */
u32 RSGL_audio_len(RSGL_audio a) {
    ma_uint64 len;
    ma_decoder_get_length_in_pcm_frames(&a.data->decoder, &len);
    return len / 44100;
}

u32 RSGL_audio_volume(RSGL_audio a) {
    float volume;
    a.data->result = ma_device_get_master_volume(&a.data->device, &volume);
    return volume * 100;
}
u32 RSGL_audio_position(RSGL_audio a) {
    ma_uint64 pos;
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
bool RSGL_wait(u32 miliseconds) {
    static i32 start_time = -1, miliLimit = 0;

    if (start_time == -1)
        start_time = time(0);

    if (miliseconds > miliLimit)
        miliLimit = miliseconds;

    i32 passed = time(0) - start_time;

    if (passed > miliLimit)
        start_time = time(0);

    return !(passed % miliseconds) && passed;
}

bool RSGL_wait_frames(u32 frames) {
    static i32 i = 0, frameLimit = 0;
    
    if (frames > frameLimit)
        frameLimit = frames;

    if (i > frameLimit)
        i = 0;

    i++;

    return !(i % frames);
}

#ifndef RSGL_NO_WINDOW
char RSGL_keyCodeToKeyChar(u32 keycode) {
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
#endif

/* collision detection */
bool RSGL_circleCollidePoint(RSGL_circle c, RSGL_point p) { return RSGL_circleCollideRect(c, (RSGL_rect) {p.x, p.y, 1, 1}); }
bool RSGL_circleCollidePointF(RSGL_circleF c, RSGL_pointF p) { return RSGL_circleCollideRectF(c, (RSGL_rectF) {p.x, p.y, 1, 1}); }

bool RSGL_circleCollideRectF(RSGL_circleF c, RSGL_rectF r) {
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
bool RSGL_circleCollideRect(RSGL_circle c, RSGL_rect r) { return RSGL_circleCollideRectF(RSGL_CIRCLEF(c.x, c.y, c.d), RSGL_RECTF(r.x, r.y, r.w, r.h)); }

bool RSGL_circleCollideF(RSGL_circleF cir, RSGL_circleF cir2) {
    float distanceBetweenCircles = (float) sqrt(
        (cir2.x - cir.x) * (cir2.x - cir.x) + 
        (cir2.y - cir.y) * (cir2.y - cir.y)
    );

    return !(distanceBetweenCircles > (cir.d/2) + (cir2.d/2)); // check if there is a collide
}

bool RSGL_circleCollide(RSGL_circle cir, RSGL_circle cir2) { return RSGL_circleCollideF(RSGL_CIRCLEF(cir.x, cir.y, cir.d), RSGL_CIRCLEF(cir2.x, cir2.y, cir2.d)); }

bool RSGL_rectCollidePoint(RSGL_rect r, RSGL_point p){ return (p.x >= r.x &&  p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h); }
bool RSGL_rectCollide(RSGL_rect r, RSGL_rect r2){ return (r.x + r.w >= r2.x && r.x <= r2.x + r2.w && r.y + r.h >= r2.y && r.y <= r2.y + r2.h); }
bool RSGL_pointCollide(RSGL_point p, RSGL_point p2){ return (p.x == p2.x && p.y == p2.y); }

bool RSGL_rectCollidePointF(RSGL_rectF r, RSGL_pointF p){ return (p.x >= r.x &&  p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h); }
bool RSGL_rectCollideF(RSGL_rectF r, RSGL_rectF r2){ return (r.x + r.w >= r2.x && r.x <= r2.x + r2.w && r.y + r.h >= r2.y && r.y <= r2.y + r2.h); }
bool RSGL_pointCollideF(RSGL_pointF p, RSGL_pointF p2){ return (p.x == p2.x && p.y == p2.y); }
#endif /* RSGL_IMPLEMENTATION */