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
    
    #define RSGL_NO_RGFW - no RSGL_window, RSGL_graphics is used instead [this is for using a differnt window manager other than RGFW ]
    #define RSGL_NO_TEXT - do not include text rendering functions
    #define RSGL_NO_WIDGETS - do not include widgets
    #define RSGL_NO_SAVE_IMAGE - do not save/load images (don't use RSGL_drawImage if you use this), 
                                    RSGL_drawImage saves the file name + texture so it can load it
                                    when you ask for it later. This disables that 
    #define RSGL_INIT_FONTS [number of fonts] - set hFow much room should be pre-allocated for fonts by fontstash
                                                this avoids performance issues related to RSGL_REALLOC
                                                RSGL_INIT_FONTS = 4 by default
    #define RSGL_INIT_IMAGES [number of fonts] - set how much room should be pre-allocated for images by RSGL
                                                this avoids performance issues related to RSGL_REALLOC
                                                RSGL_INIT_IMAGES = 20 by default
    #define RSGL_NEW_IMAGES [number of fonts] - set how much room should be RSGL_REALLOCated at a time for images by RSGL
                                                this avoids performance issues related to RSGL_REALLOC
                                                RSGL_NEW_IMAGES 10 by default

    #define RSGL_MAX_BATCHES [number of batches] - set max number of batches to be allocated
    #define RSGL_MAX_VERTS [number of verts] - set max number of verts to be allocated (global, not per batch)

    #define RSGL_RENDER_LEGACY - use legacy rendering (ex. opengl) functions
    
    #define RSGL_NO_STB_IMAGE - do not include stb_image.h (& don't define image loading funcs)
    #define RSGL_NO_STB_IMAGE_IMP - declare stb funcs but don't define
    #define RSGL_NO_DEPS_FOLDER - Do not use '/deps' for the deps includes, use "./"

    RGFW (more RGFW documentation in RGFW.h):
    
	#define RGFW_PRINT_ERRORS - (optional) makes it so RGFW prints errors when they're found
	#define RGFW_OSMESA - (optional) use OSmesa as backend (instead of system's opengl api + regular opengl)
	#define RGFW_BUFFER - (optional) just draw directly to (RGFW) window pixel buffer that is drawn to screen (the buffer is in the BGRA format)
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
#ifndef RSGL_MAX_BATCHES
#define RSGL_MAX_BATCHES 2028
#endif
#ifndef RSGL_MAX_VERTS
#define RSGL_MAX_VERTS 8192
#endif

#ifndef RSGL_MALLOC
#define RSGL_MALLOC malloc
#define RSGL_REALLOC realloc
#define RSGL_FREE free
#endif

#ifndef RSGL_UNUSED
#define RSGL_UNUSED(x) if (x){}
#endif

/*! Optional arguments for making a windows */
#define RSGL_TRANSPARENT_WINDOW		(1L<<9) /*!< the window is transparent */
#define RSGL_NO_BORDER		(1L<<3) /*!< the window doesn't have border */
#define RSGL_NO_RESIZE		(1L<<4) /*!< the window cannot be resized  by the user */
#define RSGL_ALLOW_DND     (1L<<5) /*!< the window supports drag and drop*/
#define RSGL_HIDE_MOUSE (1L<<6) /*! the window should hide the mouse or not (can be toggled later on) using `RGFW_window_mouseShow*/
#define RSGL_FULLSCREEN (1L<<8) /* the window is fullscreen by default or not */
#define RSGL_CENTER (1L<<10) /*! center the window on the screen */
#define RSGL_OPENGL_SOFTWARE (1L<<11) /*! use OpenGL software rendering */

/*! event codes */
#define RSGL_keyPressed 2 /* a key has been pressed */
#define RSGL_keyReleased 3 /*!< a key has been released*/
/*! key event note
	the code of the key pressed is stored in
	RGFW_Event.keyCode
	!!Keycodes defined at the bottom of the header file!!
	
	while a string version is stored in
	RGFW_Event.KeyString

	RGFW_Event.lockState holds the current lockState
	this means if CapsLock, NumLock are active or not
*/
#define RSGL_mouseButtonPressed 4 /*!< a mouse button has been pressed (left,middle,right)*/
#define RSGL_mouseButtonReleased 5 /*!< a mouse button has been released (left,middle,right)*/
#define RSGL_mousePosChanged 6 /*!< the position of the mouse has been changed*/
/*! mouse event note
	The x and y coords of the mouse are stored as RSGL_point in RGFW_Event.point
	
	RGFW_Event.button holds which mouse button was pressed
*/
#define RSGL_jsButtonPressed 7 /*!< a joystick button was pressed */
#define RSGL_jsButtonReleased 8 /*!< a joystick button was released */
#define RSGL_jsAxisMove 9 /*!< an axis of a joystick was moved*/
/*! joystick event note
	RGFW_Event.joystick holds which joystick was altered, if any
	RGFW_Event.button holds which joystick button was pressed

	RGFW_Event.axis holds the data of all the axis
	RGFW_Event.axisCount says how many axis there are
*/
#define RSGL_windowAttribsChange 10 /*!< the window was moved or resized (by the user) */
/* attribs change event note
	The event data is sent straight to the window structure
	with win->r.x, win->r.y, win->r.w and win->r.h
*/
#define RSGL_quit 33 /*!< the user clicked the quit button*/ 
#define RSGL_dnd 34 /*!< a file has been dropped into the window*/
#define RSGL_dnd_init 35 /*!< the start of a dnd event, when the place where the file drop is known */
/* dnd data note
	The x and y coords of the drop are stored as RSGL_point in RGFW_Event.point

	RGFW_Event.droppedFilesCount holds how many files were dropped
	
	This is also the size of the array which stores all the dropped file string,
	RGFW_Event.droppedFiles
*/

/*! mouse button codes (RGFW_Event.button) */
#define RSGL_mouseLeft  1 /*!< left mouse button is pressed*/
#define RSGL_mouseMiddle  2 /*!< mouse-wheel-button is pressed*/
#define RSGL_mouseRight  3 /*!< right mouse button is pressed*/
#define RSGL_mouseScrollUp  4 /*!< mouse wheel is scrolling up*/
#define RSGL_mouseScrollDown  5 /*!< mouse wheel is scrolling down*/

/* for RGFW_Event.lockstate */
#define RSGL_CAPSLOCK (1L << 1)
#define RSGL_NUMLOCK (1L << 2)

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
RSGL basicDraw types
*/

#ifndef RSGL_QUADS
#define RSGL_POINTS                               0x0000
#define RSGL_LINES                                0x0001      
#define RSGL_LINE_LOOP                            0x0002
#define RSGL_LINE_STRIP                           0x0003
#define RSGL_TRIANGLES                            0x0004      
#define RSGL_TRIANGLE_STRIP                       0x0005
#define RSGL_TRIANGLE_FAN                         0x0006      
#define RSGL_QUADS                                 0x0007

/* these are to ensure GL_DEPTH_TEST is disabled when they're being rendered */
#define RSGL_POINTS_2D                               0x0010
#define RSGL_LINES_2D                                0x0011    
#define RSGL_LINE_LOOP_2D                            0x0012
#define RSGL_LINE_STRIP_2D                           0x0013
#define RSGL_TRIANGLES_2D                            0x0014     
#define RSGL_TRIANGLE_STRIP_2D                       0x0015
#define RSGL_TRIANGLE_FAN_2D                         0x0016

#define RSGL_TRIANGLES_2D_BLEND     0x0114
#endif

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

#define RSGL_between(x, lower, upper) (((lower) <= (x)) && ((x) <= (upper)))
#define RSGL_ENUM(type, name) type name; enum

/* 
*******
RSGL_[shape]
*******
*/

typedef struct RSGL_rect {
    i32 x, y, w, h;
} RSGL_rect;
#define RSGL_RECT(x, y, w, h) (RSGL_rect){x, y, w, h}

typedef struct RSGL_rectF { float x, y, w, h; } RSGL_rectF;
#define RSGL_RECTF(x, y, w, h) (RSGL_rectF){x, y, w, h}

typedef struct RSGL_point {
    i32 x, y;
} RSGL_point;
#define RSGL_POINT(x, y) (RSGL_point){x, y}

typedef struct RSGL_area {
    u32 w, h;
} RSGL_area;
#define RSGL_AREA(w, h) (RSGL_area){w, h}

/*  include RGFW here  */

#ifndef RSGL_NO_RGFW
/* so we're only using one kind of shape data */
#define RGFW_rect RSGL_rect
#define RGFW_vector RSGL_point
#define RGFW_area RSGL_area
#define GL_SILENCE_DEPRECATION
#ifndef RSGL_NO_DEPS_FOLDER
#include "deps/RGFW.h"
#else
#include "RGFW.h"
#endif
#else
typedef struct {
    u32 type;  /* event type */
    RSGL_point point; /* cursor point */
    u8 button; /*!< which mouse button has been clicked (0) left (1) middle (2) right OR which joystick button was pressed*/
    double scroll; /* the raw mouse scroll value */
    u32 keyCode; /* code of key pressed */
    char keyName[16]; /* string of key */
} RGFW_Event;
#endif

#include <stdbool.h>
#include <stddef.h>

/* 
******* 
RSGL_[shape]
*******
*/

typedef struct RSGL_pointF { float x, y; } RSGL_pointF;
#define RSGL_POINTF(x, y) (RSGL_pointF){x, y}

typedef struct RSGL_point3D {
    i32 x, y, z;
} RSGL_point3D;

typedef struct RSGL_point3DF { float x, y, z; } RSGL_point3DF;

#define RSGL_POINT3D(x, y, z) (RSGL_point3D){x, y, z}
#define RSGL_POINT3DF(x, y, z) (RSGL_point3DF){x, y, z}

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

/* toggle the use of legacy OpenGL, on by default unless it fails to load */
RSGLDEF void RSGL_legacy(i32 legacy);

/* 
*******
RSGL_window
*******
*/

#ifndef RSGL_NO_RGFW

#define RSGL_Up RGFW_Up
#define RSGL_Down RGFW_Down
#define RSGL_Left RGFW_Left
#define RSGL_Right RGFW_Right
#define RSGL_Tab RGFW_Tab
#define RSGL_BackSpace RGFW_BackSpace

/* relevent RGFW defines. For documentation, this isn't meant to be used. */
#if defined(RELEVENT_RGFW_STRUCTS) && !defined(RELEVENT_RGFW_STRUCTS)
/* NOTE: some parts of the data can represent different things based on the event (read comments in RGFW_Event struct) */
typedef struct RGFW_Event {
	#ifdef RGFW_WINDOWS
	char keyName[16]; /* key name of event*/
	#else
	char* keyName; /*!< key name of event */
	#endif
	
	/*! drag and drop data */
	/* 260 max paths with a max length of 260 */
	#ifdef RGFW_ALLOC_DROPFILES
    char** droppedFiles;
	#else
	char droppedFiles[RGFW_MAX_DROPS][RGFW_MAX_PATH]; /*!< dropped files*/
	#endif
	u32 droppedFilesCount; /*!< house many files were dropped */

	u32 type; /*!< which event has been sent?*/
	RGFW_vector point; /*!< mouse x, y of event (or drop point) */
    u32 keyCode; /*!< keycode of event 	!!Keycodes defined at the bottom of the header file!! */

	u32 inFocus;  /*if the window is in focus or not*/
	
	u32 fps; /*the current fps of the window [the fps is checked when events are checked]*/
	u32 current_ticks, frames;
	
	u8 lockState;

	u16 joystick; /* which joystick this event applies to (if applicable to any) */

    u8 button; /*!< which mouse button has been clicked (0) left (1) middle (2) right OR which joystick button was pressed*/
  	
	u8 axisesCount; /* number of axises */
	RGFW_vector axis[2]; /* x, y of axises (-100 to 100) */
} RGFW_Event; /*!< Event structure for checking/getting events */

typedef struct RGFW_window {
	RGFW_window_src src;

	#if defined(RGFW_OSMESA) || defined(RGFW_BUFFER) 
	u8* buffer; /* buffer for non-GPU systems (OSMesa, basic software rendering) */
				/* when rendering using RGFW_BUFFER, the buffer is in the BGRA format */
	#endif

	RGFW_Event event; /*!< current event */

	RGFW_rect r; /* the x, y, w and h of the struct */

	u8 fpsCap; /*!< the fps cap of the window should run at (change this var to change the fps cap, 0 = no limit)*/
		/*[the fps is capped when events are checked]*/
} RGFW_window; /*!< Window structure for managing the window */
#endif

typedef RGFW_window RSGL_window;
/* 
    create RSGL window, an RGFW window and init stuff for RSGL 
    u64 args -> custom args (can be found earlier in this file)
*/
RSGLDEF RSGL_window* RSGL_createWindow(const char* name, RSGL_rect rect, u64 args);

/*
******
RGFW_FUNCTION_DEFINES
******
*/

/* 
	this function checks an *individual* event (and updates window structure attributes)
	this means, using this function without a while loop may cause event lag

	ex. 
	
	while (RSGL_window_checkEvent(win) != NULL) [this keeps checking events until it reaches the last one]
*/

RGFW_Event* RSGL_window_checkEvent(RSGL_window* win); /*!< check events (returns a pointer to win->event or NULL if there is no event)*/

/*! window managment functions*/
RSGLDEF void RSGL_window_close(RSGL_window* win); /*!< close the window and free leftover data */

RSGLDEF void RSGL_window_setIconImage(RGFW_window* win, const char* image);
RSGLDEF void RSGL_window_makeCurrent(RSGL_window* win);
RSGLDEF void RSGL_window_clear(RSGL_window* win, RSGL_color color);

RSGLDEF void RSGL_window_setMouseStandard(RSGL_window* win, u32 cursorIcon);

/*!< if window == NULL, it checks if the key is pressed globally. Otherwise, it checks only if the key is pressed while the window in focus.*/
RSGLDEF u8 RSGL_isPressedI(RSGL_window* win, u32 key); /*!< if key is pressed (key code)*/

RSGLDEF u8 RSGL_wasPressedI(RSGL_window* win, u32 key); /*!< if key was pressed (checks prev keymap only) (key code)*/

RSGLDEF u8 RSGL_isHeldI(RSGL_window* win, u32 key); /*!< if key is held (key code)*/
RSGLDEF u8 RSGL_isReleasedI(RSGL_window* win, u32 key); /*!< if key is released (key code)*/
#else /* RSGL_NO_RGFW */

/* 
*********************
RSGL_GRAPHICS_CONTEXT
*********************

this is for standalone RSGL graphics, no windowing / RGFW
AN example of this can be found in examples/glfw.c
*/

/* 
You need to define these if you want to use `RSGL_textbox_update`

#define RSGL_Up 
#define RSGL_Down 
#define RSGL_Left 
#define RSGL_Right 
#define RSGL_Tab 
#define RSGL_BackSpace 
*/

RSGLDEF void RSGL_initGraphics(
                            RSGL_area r, /* graphics context size */
                            void* loader /* opengl prozc address ex. wglProcAddress */
                            ); 
RSGLDEF void RSGL_graphics_updateSize(RSGL_area r);
RSGLDEF void RSGL_graphics_clear(RSGL_color c);
RSGLDEF void RSGL_graphics_free(void);


/* YOU define this version (if you need it) */
/*!< if window == NULL, it checks if the key is pressed globally. Otherwise, it checks only if the key is pressed while the window in focus.*/
RSGLDEF u8 RSGL_isPressedI(void* win, u32 key); /*!< if key is pressed (key code)*/

#ifndef RSGL_MOUSE_ARROW
#define RSGL_MOUSE_ARROW    1
#define RSGL_MOUSE_IBEAM  2
#define RSGL_MOUSE_POINTING_HAND 3
#endif

RSGLDEF void RSGL_window_setMouseStandard(void* win, u32 cursorIcon);

#endif /* RSGL_GRAPHICS_CONTEXT / !RSGL_NO_RGFW */

/* 
*******
RSGL_draw
*******
*/


/* 
    RSGL_draw args 

    RSGL has internal args which control how RSGL draws certain things
    by default these args clear after each RSGL_draw<whatever> call 

    but you can run RSGL_setClearArgs to enable or disable this behavior
    you can also run RSGL_clearArgs to clear the args by hand
*/
/* RSGL_args */
typedef struct RSGL_drawArgs {
    float* gradient; /* does not allocate any memory */
    
    u32 texture;
    u32 gradient_len;

    RSGL_rect currentRect; /* size of current window */
    RSGL_point3D rotate; 

    bool fill;
    RSGL_point3DF center;
    float lineWidth;
    i32 legacy;
    u32 program;
} RSGL_drawArgs;

RSGLDEF void RSGL_rotate(RSGL_point3D rotate); /* apply rotation to drawing */
RSGLDEF void RSGL_setTexture(u32 texture); /* apply texture to drawing */
RSGLDEF void RSGL_setProgram(u32 program); /* use shader program for drawing */
RSGLDEF void RSGL_setGradient(
                                float* gradient, /* array of gradients */
                                size_t len /* length of array */
                            ); /* apply gradient to drawing, based on color list*/
RSGLDEF void RSGL_fill(bool fill); /* toggle filling, if fill is false it runs RSGL_draw<whatever>_outline instead */
RSGLDEF void RSGL_center(RSGL_point3DF center); /* the center of the drawing (or shape), this is used for rotation */

/* args clear after a draw function by default, this toggles that */
RSGLDEF void RSGL_setClearArgs(bool clearArgs); /* toggles if args are cleared by default or not */
RSGLDEF void RSGL_clearArgs(void); /* clears the args */

/* calculate the align a smaller rect with larger rect */
typedef RSGL_ENUM(u8, RSGL_alignment) {
    RSGL_ALIGN_NONE = (1 << 0),
    /* horizontal */
    RSGL_ALIGN_LEFT = (1 << 1),
    RSGL_ALIGN_CENTER = (1 << 2),
    RSGL_ALIGN_RIGHT = (1 << 3),
    
    /* vertical */
    RSGL_ALIGN_UP = (1 << 4),
    RSGL_ALIGN_MIDDLE = (1 << 5),
    RSGL_ALIGN_DOWN = (1 << 6),

    RSGL_ALIGN_HORIZONTAL = RSGL_ALIGN_LEFT | RSGL_ALIGN_CENTER | RSGL_ALIGN_RIGHT,
    RSGL_ALIGN_VERTICAL = RSGL_ALIGN_UP | RSGL_ALIGN_MIDDLE | RSGL_ALIGN_DOWN,
/* ex : alignment = (RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE) */
};

/* align smaller rect onto larger rect based on a given alignment */
RSGLDEF RSGL_rect RSGL_alignRect(RSGL_rect larger, RSGL_rect smaller, u16 alignment);
RSGLDEF RSGL_rectF RSGL_alignRectF(RSGL_rectF larger, RSGL_rectF smaller, u16 alignment);

#define RSGL_GET_WORLD_X(x) (float)(2.0f * (x) / RSGL_args.currentRect.w - 1.0f)
#define RSGL_GET_WORLD_Y(y) (float)(1.0f + -2.0f * (y) / RSGL_args.currentRect.h)
#define RSGL_GET_WORLD_Z(z) (float)(z)

#define RSGL_GET_MATRIX_X(x, y, z) (matrix.m[0] * x + matrix.m[4] * y + matrix.m[8] * z + matrix.m[12])
#define RSGL_GET_MATRIX_Y(x, y, z) (matrix.m[1] * x + matrix.m[5] * y + matrix.m[9] * z + matrix.m[13])
#define RSGL_GET_MATRIX_Z(x, y, z) (matrix.m[2] * x + matrix.m[6] * y + matrix.m[10] * z + matrix.m[14])

#define RSGL_GET_MATRIX_POINT(x, y, z) RSGL_GET_MATRIX_X(x, y, z), RSGL_GET_MATRIX_Y(x, y, z), RSGL_GET_MATRIX_Z(x, y, z)
#define RSGL_GET_WORLD_POINT(x, y, z) RSGL_GET_WORLD_X((x)), RSGL_GET_WORLD_Y((y)), RSGL_GET_WORLD_Z((z))

#define RSGL_GET_FINAL_POINT(x, y, z) RSGL_GET_MATRIX_POINT(RSGL_GET_WORLD_X((x)), RSGL_GET_WORLD_Y((y)), RSGL_GET_WORLD_Z((z)))

typedef struct RSGL_MATRIX {
    float m[16];
} RSGL_MATRIX;

RSGLDEF RSGL_MATRIX RSGL_initDrawMatrix(RSGL_point3DF center);

/* 
RSGL_basicDraw is a function used internally by RSGL, but you can use it yourself
RSGL_basicDraw batches a given set of points based on the data to be rendered
*/
RSGLDEF void RSGL_basicDraw(
                u32 TYPE, /* type of shape, RSGL_QUADS, RSGL_TRIANGLES, RSGL_LINES, RSGL_QUADS_2D */
                float* points, /* array of 3D points */
                float* texPoints, /* array of 2D texture points (must be same length as points)*/
                RSGL_color c, /* the color to draw the shape */
                size_t len /* the length of the points array */
            );

typedef struct RSGL_BATCH {
    size_t start, len; /* when batch starts and it's length */
    u32 type, tex;
    float lineWidth;
} RSGL_BATCH; /* batch data type for rendering */

typedef struct RSGL_RENDER_INFO {
    RSGL_BATCH* batches;

    float* verts;
    float* texCoords;
    float* colors;

    size_t len; /* number of batches*/
    size_t vert_len; /* number of verts */
} RSGL_RENDER_INFO; /* render data */

/* 
    All of these functions are to be defined by the external render backend
*/

/* renders the current batches */
RSGLDEF void RSGL_renderBatch(RSGL_RENDER_INFO* info);
RSGLDEF void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info); /* init render backend */
RSGLDEF void RSGL_renderFree(void); /* free render backend */
RSGLDEF void RSGL_renderClear(float r, float g, float b, float a);
RSGLDEF void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF u32 RSGL_renderCreateTexture(u8* bitmap, RSGL_area memsize,  u8 channels);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_renderUpdateTexture(u32 texture, u8* bitmap, RSGL_area memsize, u8 channels);
/* delete a texture */
RSGLDEF void RSGL_renderDeleteTexture(u32 tex);

/* custom shader program */
typedef struct RSGL_programInfo {
    u32 vShader, fShader, program;
} RSGL_programInfo;

RSGLDEF RSGL_programInfo RSGL_renderCreateProgram(const char* VShaderCode, const char* FShaderCode, char* posName, char* texName, char* colorName);
RSGLDEF void RSGL_renderDeleteProgram(RSGL_programInfo program);
RSGLDEF void RSGL_renderSetShaderValue(u32 program, char* var, float value[], u8 len);

/* these are RFont functions that also must be defined by the renderer

32 RFont_create_atlas(u32 atlasWidth, u32 atlasHeight);
void RFont_bitmap_to_atlas(u32 atlas, u8* bitmap, float x, float y, float w, float h);

*/

/* RSGL translation */
RSGLDEF RSGL_MATRIX RSGL_matrixMultiply(float left[16], float right[16]);
RSGLDEF RSGL_MATRIX RSGL_rotatef(RSGL_MATRIX* matrix, float angle, float x, float y, float z); 
RSGLDEF RSGL_MATRIX RSGL_translatef(RSGL_MATRIX* matrix, float x, float y, float z);
/* 2D shape drawing */
/* in the function names, F means float */

RSGLDEF void RSGL_drawPoint(RSGL_point p, RSGL_color c);
RSGLDEF void RSGL_drawPointF(RSGL_pointF p, RSGL_color c);
RSGLDEF void RSGL_plotLines(RSGL_pointF* lines, size_t points_count, u32 thickness, RSGL_color c);

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

/* thickness means the thickness of the line */

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

/* format a string */
#ifndef RSGL_NO_TEXT
RSGLDEF const char* RFont_fmt(const char* string, ...);
#define RSGL_strFmt RFont_fmt

/* loads a font into RSGL, returns it's index into the RSGL_fonts array, this is used as an id in later functions */
RSGLDEF i32 RSGL_loadFont(const char* font);
/* sets font as the current font in use based on index in RSGL_font, given when it was loaded */
RSGLDEF void RSGL_setFont(i32 font);

typedef struct RFont_font RFont_font;
/* sets source RFont font as the current font, given when it was loaded */
RSGLDEF void RSGL_setRFont(RFont_font* font);

/* draws the current fps on the screen */
#ifndef RSGL_NO_RGFW
RSGLDEF void RSGL_drawFPS(RGFW_window* win, RSGL_circle c, RSGL_color color);
#endif

RSGLDEF void RSGL_drawText_len(const char* text, size_t len, RSGL_circle c, RSGL_color color);
RSGLDEF void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color);
#define RSGL_drawTextF(text, font, c, color) \
    RSGL_setFont(font);\
    RSGL_drawText(text, c, color);

/* align text onto larger rect based on a given alignment */
RSGLDEF RSGL_circle RSGL_alignText(char* str, RSGL_circle c, RSGL_rectF larger, u8 alignment);
/* align text based on a length */
RSGLDEF RSGL_circle RSGL_alignText_len(char* str, size_t str_len, RSGL_circle c, RSGL_rectF larger, u8 alignment);

/* 
    returns the width of a text when rendered with the set font with the size of `fontSize
    stops at `textEnd` or when it reaches '\0'
*/
RSGLDEF RSGL_area RSGL_textArea(const char* text, u32 fontSize, size_t textEnd);
RSGLDEF RSGL_area RSGL_textLineArea(const char* text, u32 fontSize, size_t textEnd, size_t line);
#define RSGL_textAreaF(text, fontSize, textEnd) \
    RSGL_setFont(font);\
    RSGL_textAreaF(text, fontSize, textEnd);
#endif /* RSGL_NO_TEXT */

/* 
    this creates a texture based on a given image, draws it on a rectangle and then returns the loaded texture 
    
    if the rectangle's width and height are 0 it doesn't draw the image
    the texture is loaded into RSGL_image, this means it doesn't need to be freed
    but you can still free it early
*/

typedef struct RSGL_image { u32 tex; RSGL_area srcSize; char file[255]; } RSGL_image;
RSGLDEF RSGL_image RSGL_drawImage(const char* image, RSGL_rect r);

#define RSGL_loadImage(image) ((RSGL_image) RSGL_drawImage(image, (RSGL_rect){0, 0, 0, 0}))

/* 
    these two functions can be used before RSGL_renderCreateTexture in order to create 
    an swizzle'd texutre or atlas
*/

/* 
*******
RSGL_widgets
*******
*/

#ifndef RSGL_NO_WIDGETS

/* style of a widget */
typedef RSGL_ENUM(u32, RSGL_widgetStyle) {
    RSGL_STYLE_NONE = (1L << 0),

    RSGL_STYLE_DARK = (1L << 1), /* dark mode*/
    RSGL_STYLE_LIGHT = (1L << 2), /* light mode*/
    RSGL_STYLE_MODE  = RSGL_STYLE_LIGHT | RSGL_STYLE_DARK,

    RSGL_STYLE_ROUNDED = (1L << 3), /* use rounded rect */
    
    RSGL_STYLE_SLIDER_HORIZONTAL = (1L << 4),
    RSGL_STYLE_SLIDER_VERTICAL = (1L << 5),
    /* rectangle by default */
    RSGL_STYLE_SLIDER_CIRCLE = (1L << 6), 
    RSGL_STYLE_SLIDER = RSGL_STYLE_SLIDER_VERTICAL | RSGL_STYLE_SLIDER_HORIZONTAL, 
    
    RSGL_STYLE_TOGGLE = (1L << 7),
    RSGL_STYLE_RADIO = (1L << 8),
    RSGL_STYLE_CHECKBOX = (1 << 9),
    RSGL_STYLE_COMBOBOX = (1 << 10),
    RSGL_STYLE_CONTAINER = (1 << 11),
    RSGL_STYLE_TEXTBOX = (1 << 12),
    RSGL_STYLE_NO_TAB = (1 << 13),
    RSGL_STYLE_TYPE = RSGL_STYLE_SLIDER | RSGL_STYLE_TOGGLE | RSGL_STYLE_RADIO | RSGL_STYLE_CHECKBOX | RSGL_STYLE_COMBOBOX,

    RSGL_SHAPE_NULL = (1L << 0),
    RSGL_SHAPE_RECT = (1L << 14),
    RSGL_SHAPE_POLYGON = (1L << 15),

    RSGL_SHAPE_POLYGONF = (1L << 16),
    RSGL_SHAPE_RECTF = (1L << 17),

    RSGL_STYLE_RED = (1 << 18),
    RSGL_STYLE_BLUE = (1 << 19),
    RSGL_STYLE_GREEN = (1 << 20),
    RSGL_STYLE_YELLOW = (1 << 21),
    RSGL_STYLE_TEAL = (1 << 22),
    RSGL_STYLE_PURPLE = (1 << 23),
    RSGL_STYLE_COLOR = RSGL_STYLE_RED | RSGL_STYLE_BLUE | RSGL_STYLE_GREEN | RSGL_STYLE_YELLOW | RSGL_STYLE_TEAL | RSGL_STYLE_PURPLE,

    RSGL_STYLE_SHAPE = RSGL_SHAPE_RECT | RSGL_SHAPE_POLYGON | RSGL_SHAPE_POLYGONF | RSGL_SHAPE_RECTF,
};


/* expandable rect, returns true if a change has been made */
bool RSGL_expandableRect_update(RSGL_rect* rect, RGFW_Event e);
bool RSGL_expandableRectF_update(RSGL_rectF* rect, RGFW_Event e);

typedef RSGL_ENUM(u8, RSGL_buttonStatus) {
    RSGL_none = 0,
    RSGL_hovered,
    RSGL_pressed,
};

typedef struct RSGL_button_src {
    /* source data */
    struct {
        char* str;
        size_t text_len;
        size_t text_cap;
        RSGL_circle c;
        RSGL_color color;
        u8 alignment;
    } text;

    char** combo;

    u32 tex;
    RSGL_color color, outlineColor;
    RSGL_point rounding;
    RSGL_drawArgs drawArgs; 
    
    union {    
        size_t array_count; /* used for the combobox and radio buttons array size */
        size_t slider_pos; /* the pos of a slider for x or y */
        u32 cursorIndex; /* current cursor index (for textbox) */
    };

    u32* keys;
    size_t keys_len;

    void* window;

    RSGL_widgetStyle style;   
} RSGL_button_src; /* src data for a button*/

typedef struct RSGL_button {
    RSGL_button_src loaded_states[3]; /* based on RSGL_buttonStatus*/
    RSGL_buttonStatus status;
    
    union {
        size_t radio_select; /* which ratio button the status applies to (the rest are idle)*/
        size_t line_count; /* number of lines (for a textbox) */
    };

    bool toggle; /* for toggle buttons */

    RSGL_rectF rect;
    u32 points; /* for a polygon */
    u32 outline;

    RSGL_button_src src;
} RSGL_button;


typedef struct RSGL_select {
    u32 selectStart;
    u32 selectEnd;
    bool selected;
} RSGL_select;

/* button managing functions */
/* 
    inits the button 
    zeros out the data by default
*/
RSGLDEF RSGL_button RSGL_initButton(void); 

/* creates a new button from another button, does not copy over states */
RSGLDEF RSGL_button RSGL_copyButton(RSGL_button button); 

/* 
    load a default style into the button 
    the rect and any text you want should 
    be loaded before this function is used
*/
RSGLDEF void RSGL_button_setStyle(RSGL_button* button, RSGL_widgetStyle buttonStyle);

RSGLDEF void RSGL_button_setRect(RSGL_button* button, RSGL_rect rect);
RSGLDEF void RSGL_button_setRectF(RSGL_button* button, RSGL_rectF rect);

/* set rounding (for rectangles) */
RSGLDEF void RSGL_button_setRounding(RSGL_button* button, RSGL_point rounding);

RSGLDEF void RSGL_button_setPolygon(RSGL_button* button, RSGL_rect rect, u32 points);
RSGLDEF void RSGL_button_setPolygonF(RSGL_button* button, RSGL_rectF rect, u32 points);

/* add text to the button */
RSGLDEF void RSGL_button_setText(RSGL_button* button, char* text, size_t text_len, RSGL_circle c, RSGL_color color);
/* align the set text to the button */
RSGLDEF void RSGL_button_alignText(RSGL_button* button, u8 alignment);

RSGLDEF void RSGL_button_setTexture(RSGL_button* button, u32 tex);

RSGLDEF void RSGL_button_setColor(RSGL_button* button, RSGL_color color);
RSGLDEF void RSGL_button_setOutline(RSGL_button* button, u32 size, RSGL_color color);

/* set the window for the button (this is used for RGFW_isPressed and changing the cursor icon), NULL by default */
RSGLDEF void RSGL_button_setWindow(RSGL_button* button, void* window);

/* set keybinding for the button, if these keys are pressed the button is marked as pressed */
RSGLDEF void RSGL_button_setKeybind(RSGL_button* button, u32* keys, size_t keys_len);

/* set button combos for a combo box */
RSGLDEF void RSGL_button_setCombo(RSGL_button* button, char** combo, size_t combo_count);

/* 
    by default drawArgs is reset before drawing the button 
    it is then backed up later on
*/

/* draws the button using the current draw data */
RSGLDEF void RSGL_button_setDrawArgs(RSGL_button* button);
/* uses given draw arg data to draw the button */
RSGLDEF void RSGL_button_setDrawArgsData(RSGL_button* button, RSGL_drawArgs args);

/* button states (change button based on current state) */
RSGLDEF void RSGL_button_setOnIdle(RSGL_button* button, RSGL_button_src idle);
RSGLDEF void RSGL_button_setOnHover(RSGL_button* button, RSGL_button_src hover);
RSGLDEF void RSGL_button_setOnPress(RSGL_button* button, RSGL_button_src press);

/* draw the button */
RSGLDEF void RSGL_drawButton(RSGL_button button);

/* these functions should be run in an event loop */
RSGLDEF void RSGL_button_update(
    RSGL_button* b, /* button pointer */
    RGFW_Event e /* current event */
);

RSGLDEF float RSGL_slider_update(
    RSGL_button* b, /* button pointer */
    RGFW_Event e /* the current event, used for checking for a mouse event */
);

typedef struct  {
    RSGL_button** buttons;
    size_t buttons_len;

    RSGL_button title;
    bool held;
    RSGL_point initPoint;
} RSGL_container_src;

typedef RSGL_button RSGL_container;

RSGLDEF RSGL_button RSGL_nullButton(void);
RSGLDEF RSGL_button RSGL_label(char* text, size_t text_len, size_t textSize);

RSGLDEF RSGL_container* RSGL_initContainer(RSGL_rect r, RSGL_button** buttons, size_t len);
RSGLDEF void RSGL_freeContainer(RSGL_container* container);

RSGLDEF void RSGL_drawContainer(RSGL_container* container);
RSGLDEF void RSGL_container_setStyle(RSGL_container* button, u16 buttonStyle);

RSGLDEF void RSGL_container_setPos(RSGL_container* container, RSGL_point p);

RSGLDEF i32 RSGL_container_update(RSGL_container* container, RGFW_Event event);
#ifndef RSGL_NO_TEXT

typedef RSGL_button RSGL_textbox;

RSGLDEF RSGL_textbox* RSGL_initTextbox(size_t defaultSize);
RSGLDEF void RSGL_textbox_free(RSGL_textbox* tb); 

RSGLDEF void RSGL_textbox_draw(RSGL_textbox* tb);
RSGLDEF char* RSGL_textbox_getString(RSGL_textbox* tb, size_t* len);
RSGLDEF RSGL_select RSGL_textbox_update(RSGL_textbox* tb, RGFW_Event event);
RSGLDEF void RSGL_textbox_setTextInfo(RSGL_textbox* tb, RSGL_circle c, RSGL_color color);

/* these wrap around the same RSGL_button functions */
RSGLDEF void RSGL_textbox_alignText(RSGL_textbox* tb, u8 alignment);
RSGLDEF void RSGL_textbox_setColor(RSGL_textbox* button, RSGL_color color);
RSGLDEF void RSGL_textbox_setOutline(RSGL_textbox* button, u32 size, RSGL_color color);

RSGLDEF void RSGL_textbox_setStyle(RSGL_textbox* button, RSGL_widgetStyle buttonStyle);
RSGLDEF void RSGL_textbox_setRect(RSGL_textbox* button, RSGL_rect rect);
RSGLDEF void RSGL_textbox_setRectF(RSGL_textbox* button, RSGL_rectF rect);

RSGLDEF void RSGL_textbox_setWindow(RSGL_textbox* button, void* win);
#endif /* RSGL_NO_TEXT */

#endif /* RSGL_NO_WIDGETS */

/* 
*******
extra
*******
*/

/* wait functions */
RSGLDEF bool RSGL_wait(u32 miliseconds);
RSGLDEF bool RSGL_wait_frames(u32 frames);

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

#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main() {
    RSGL_window* win = RSGL_createWindow("name", (RSGL_rect){500, 500, 500, 500}, RSGL_CENTER);


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

#ifdef RSGL_RENDER_LEGACY
#define RFONT_RENDER_LEGACY
#endif

#define RGFW_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <assert.h>

#ifndef RSGL_NO_RGFW
#define GL_SILENCE_DEPRECATION
#ifndef RSGL_NO_DEPS_FOLDER
#include "deps/RGFW.h"
#else
#include "RGFW.h"
#endif
#endif

#ifndef RSGL_NO_TEXT
#define RFONT_IMPLEMENTATION

#define RFONT_RENDER_LEGACY
#define RFONT_NO_OPENGL

#define RFont_area RSGL_area

#ifndef RSGL_NO_DEPS_FOLDER
#include "deps/RFont.h"
#else
#include "RFont.h"
#endif
#endif /* RSGL_NO_TEXT */

#if !defined(RSGL_NO_STB_IMAGE) && !defined(RSGL_NO_STB_IMAGE_IMP)

#ifndef RSGL_NO_DEPS_FOLDER
#include "deps/stb_image.h"
#else
#include <stb_image.h>
#endif

#endif

#ifdef RSGL_NO_STB_IMAGE_IMP
u8* stbi_load            (char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
#endif

#include <time.h>

#include <stdbool.h>

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

RSGL_drawArgs RSGL_args = {NULL, 1, 0, { }, {0, 0, 0}, 1, RSGL_POINT3DF(-1, -1, -1), 1, 0, 0};
bool RSGL_argsClear = false;

u32 RSGL_windowsOpen = 0;
RSGL_image* RSGL_images = NULL;
size_t RSGL_images_len = 0;

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

RSGL_rect RSGL_alignRect(RSGL_rect larger, RSGL_rect smaller, u16 alignment) {
    RSGL_rectF r = RSGL_alignRectF(
                                    RSGL_RECTF(larger.x, larger.y, larger.w, larger.y), 
                                    RSGL_RECTF(smaller.x, smaller.y, smaller.w, smaller.h), 
                                    alignment
                                );
    
    return RSGL_RECT(r.x, r.y, r.w, r.h);
}

RSGL_rectF RSGL_alignRectF(RSGL_rectF larger, RSGL_rectF smaller, u16 alignment) {
    RSGL_rectF aligned = smaller;

    switch (alignment & RSGL_ALIGN_HORIZONTAL) {
        case RSGL_ALIGN_LEFT:
            aligned.x = larger.x;
            break;
        case RSGL_ALIGN_CENTER:
            aligned.x = larger.x + ((larger.w - smaller.w) / 2.0);
            break;
        case RSGL_ALIGN_RIGHT:
            aligned.x = (larger.x + larger.w) - smaller.w;
            break;
        default: break;
    }

    switch (alignment & RSGL_ALIGN_VERTICAL) {
        case RSGL_ALIGN_UP:
            aligned.y = larger.y;
            break;
        case RSGL_ALIGN_MIDDLE:
            aligned.y = larger.y + ((larger.h - smaller.h) / 2.0);
            break;
        case RSGL_ALIGN_DOWN:
            aligned.y = (larger.y + larger.h) - smaller.h;
            break;
        default: break;
    }

    return aligned;
}

RSGL_MATRIX RSGL_initDrawMatrix(RSGL_point3DF center) {    
    RSGL_MATRIX matrix = (RSGL_MATRIX) { 
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    };
        
    if (RSGL_args.rotate.x || RSGL_args.rotate.y || RSGL_args.rotate.z) {
        if (RSGL_args.center.x != -1 && RSGL_args.center.y != -1 &&  RSGL_args.center.z != -1)
            center = RSGL_args.center;
        
        matrix = RSGL_translatef(&matrix, center.x, center.y, center.z);
        matrix = RSGL_rotatef(&matrix, RSGL_args.rotate.z,  0, 0, 1);
        matrix = RSGL_rotatef(&matrix, RSGL_args.rotate.y, 0, 1, 0);
        matrix = RSGL_rotatef(&matrix, RSGL_args.rotate.x, 1, 0, 0);
        matrix = RSGL_translatef(&matrix, -center.x, -center.y, -center.z);
    }

    return matrix;
}

RSGL_RENDER_INFO RSGL_renderInfo = {NULL, NULL, NULL, NULL, 0, 0};

#ifndef RSGL_CUSTOM_RENDER
#include "RSGL_gl.h"
#endif

void RSGL_basicDraw(u32 type, float* points, float* texPoints, RSGL_color c, size_t len) {
    if (RSGL_renderInfo.len + 1 >= RSGL_MAX_BATCHES || RSGL_renderInfo.vert_len + len >= RSGL_MAX_VERTS) {
        RSGL_renderBatch(&RSGL_renderInfo);
    }

    RSGL_BATCH* batch = NULL;

    if (
        RSGL_renderInfo.len == 0 || 
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].tex != RSGL_args.texture  ||
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].lineWidth != RSGL_args.lineWidth ||
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].type != type ||
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].type == RSGL_TRIANGLE_FAN_2D
    ) {
        RSGL_renderInfo.len += 1;
    
        batch = &RSGL_renderInfo.batches[RSGL_renderInfo.len - 1];
        batch->start = RSGL_renderInfo.vert_len;
        batch->len = 0;
        batch->type = type;
        batch->tex = RSGL_args.texture;
        batch->lineWidth = RSGL_args.lineWidth;
    } else {
        batch = &RSGL_renderInfo.batches[RSGL_renderInfo.len - 1];
    }

    if (batch == NULL)
        return;

    batch->len += len;

    memcpy(RSGL_renderInfo.verts + (RSGL_renderInfo.vert_len * 3), points, len * sizeof(float) * 3);
    memcpy(RSGL_renderInfo.texCoords + (RSGL_renderInfo.vert_len * 2), texPoints, len * sizeof(float) * 2);

    float color[4] = {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};

    if (RSGL_args.gradient_len && RSGL_args.gradient && (i64)(len - 1) > 0) {
        memcpy(RSGL_renderInfo.colors + (RSGL_renderInfo.vert_len * 4), color, sizeof(float) * 4);
        memcpy(RSGL_renderInfo.colors + (RSGL_renderInfo.vert_len * 4) + 4, RSGL_args.gradient, (len - 1) * sizeof(float) * 4);
    }
    else {
        size_t i;
        for (i = 0; i < len * 4; i += 4)
            memcpy(RSGL_renderInfo.colors + (RSGL_renderInfo.vert_len * 4) + i, color, sizeof(float) * 4);
    }

    RSGL_renderInfo.vert_len += len;

    if (RSGL_argsClear) {
        RSGL_setTexture(0);
        RSGL_clearArgs();
    }
}

void RSGL_legacy(i32 legacy) {
    if (RSGL_args.legacy != 2)
        RSGL_args.legacy = legacy;
}

/* 
    RSGL_window
*/

#ifndef RSGL_NO_RGFW

#define RSGL_MOUSE_ARROW                RGFW_MOUSE_ARROW
#define RSGL_MOUSE_IBEAM                RGFW_MOUSE_IBEAM
#define RSGL_MOUSE_POINTING_HAND        RGFW_MOUSE_POINTING_HAND

RSGL_window* RSGL_createWindow(const char* name, RSGL_rect r, u64 args) {
    #ifdef RGFW_OPENGL
    if (RGFW_majorVersion == 0)
        RGFW_setGLVersion(3, 3);
    #endif
    
    RGFW_window* win = RGFW_createWindow(name, r, args);
    RSGL_window_makeCurrent(win);

    if (RSGL_windowsOpen == 0) {
        RSGL_args.rotate = (RSGL_point3D){0, 0, 0}; 

        if (RSGL_renderInfo.batches == NULL) {
            RSGL_renderInfo.len = 0;
            RSGL_renderInfo.vert_len = 0;
            RSGL_renderInfo.batches = (RSGL_BATCH*)RSGL_MALLOC(sizeof(RSGL_BATCH) * RSGL_MAX_BATCHES);
            RSGL_renderInfo.verts = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 3);
            RSGL_renderInfo.colors = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 4);
            RSGL_renderInfo.texCoords = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 2);
        }

        void* proc = NULL;

        #ifdef RGFW_OPENGL
        proc = RGFW_getProcAddress;
        #elif defined(RGFW_BUFFER)
        proc = (void*)&win->buffer;
        #endif

        RSGL_renderInit(proc, &RSGL_renderInfo);

        #ifndef RSGL_NO_TEXT
        RFont_init(win->r.w, win->r.h);
        RSGL_font.fonts = (RSGL_fontData*)RSGL_MALLOC(sizeof(RSGL_fontData) * RSGL_INIT_FONTS); 
        #endif
    }

    assert(win != NULL);
    RSGL_windowsOpen++;

    return win;
}

RGFW_Event* RSGL_window_checkEvent(RSGL_window* win) {
    RGFW_Event* e = RGFW_window_checkEvent(win);

    if (win->event.type == RGFW_windowAttribsChange)
        RSGL_renderViewport(0, 0, win->r.w, win->r.h);

    return e;
}

#ifndef RSGL_NO_STB_IMAGE
void RSGL_window_setIconImage(RGFW_window* win, const char* image) {
    i32 x, y, c;
    u8* img = stbi_load(image, &x, &y, &c, 0);

    RGFW_window_setIcon(win, img, RSGL_AREA(x, y), c);
    
    RSGL_FREE(img);
}
#endif

void RSGL_window_makeCurrent(RSGL_window* win) {
    RGFW_window_makeCurrent(win);

    RSGL_args.currentRect = win->r;
}

void RSGL_window_clear(RSGL_window* win, RSGL_color color) {
    RSGL_window_makeCurrent(win);

    RFont_update_framebuffer(win->r.w, win->r.h);
    
    RSGL_renderBatch(&RSGL_renderInfo);
    RGFW_window_swapBuffers(win);
    
    RSGL_renderClear(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}

void RSGL_window_close(RSGL_window* win) {
    RSGL_windowsOpen--;

    if (RSGL_windowsOpen == 0) {
        RSGL_renderFree();

        u32 i;
        #ifndef RSGL_NO_TEXT
        for (i = 0; i < RSGL_font.len; i++)
            RFont_font_free(RSGL_font.fonts[i].f);

        RSGL_FREE(RSGL_font.fonts);
        #endif
        
        for (i = 0; i < RSGL_images_len; i++)
            RSGL_renderDeleteTexture(RSGL_images[i].tex);

        if (RSGL_renderInfo.batches != NULL) {
            RSGL_FREE(RSGL_renderInfo.batches);
            RSGL_FREE(RSGL_renderInfo.verts);
            RSGL_FREE(RSGL_renderInfo.colors);
            RSGL_FREE(RSGL_renderInfo.texCoords);
            RSGL_renderInfo.batches = NULL;
            RSGL_renderInfo.len = 0;
            RSGL_renderInfo.vert_len = 0;
        }
    }

    RGFW_window_close(win);
}

void RSGL_window_setMouseStandard(RSGL_window* win, u32 cursorIcon) {
    return RGFW_window_setMouseStandard(win, cursorIcon);
}

u8 RSGL_isPressedI(RSGL_window* win, u32 key) {
    return RGFW_isPressedI(win, key);
}
u8 RSGL_wasPressedI(RSGL_window* win, u32 key) {
    return RGFW_wasPressedI(win, key);
}
u8 RSGL_isHeldI(RSGL_window* win, u32 key) {
    return RGFW_isHeldI(win, key);
}
u8 RSGL_isReleasedI(RSGL_window* win, u32 key) {
    return RGFW_isReleasedI(win, key);
}

#else /* !RSGL_NO_RGFW */

/* 
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/


void RSGL_initGraphics(RSGL_area r, void* loader) {
    RSGL_renderViewport(0, 0, r.w, r.h);
    
    RSGL_args.currentRect = (RSGL_rect){0, 0, r.w, r.h};

    #ifndef RSGL_NO_TEXT
    RFont_init(r.w, r.h);
    #endif

    if (RSGL_renderInfo.batches == NULL) {
        RSGL_renderInfo.len = 0;
        RSGL_renderInfo.vert_len = 0;
        RSGL_renderInfo.batches = (RSGL_BATCH*)RSGL_MALLOC(sizeof(RSGL_BATCH) * RSGL_MAX_BATCHES);
        RSGL_renderInfo.verts = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 3);
        RSGL_renderInfo.colors = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 4);
        RSGL_renderInfo.texCoords = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 2);
    
        RSGL_renderInit(loader, &RSGL_renderInfo);
    }
}

void RSGL_graphics_clear(RSGL_color color) {
    RSGL_renderClear(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

    RSGL_renderBatch(&RSGL_renderInfo);
}

void RSGL_graphics_updateSize(RSGL_area r) {
    RSGL_args.currentRect = (RSGL_rect){0, 0, r.w, r.h};
    RFont_update_framebuffer(r.w, r.h);
    RSGL_renderViewport(0, 0, r.w, r.h);
}

void RSGL_graphics_free() {
    u32 i;
    #ifndef RSGL_NO_TEXT
    for (i = 0; i < RSGL_font.len; i++)
        RFont_font_free(RSGL_font.fonts[i].f);

    RSGL_FREE(RSGL_font.fonts);
    #endif

    RSGL_renderFree();

    if (RSGL_renderInfo.batches != NULL) {
        RSGL_FREE(RSGL_renderInfo.batches);
        RSGL_FREE(RSGL_renderInfo.verts);
        RSGL_FREE(RSGL_renderInfo.colors);
        RSGL_FREE(RSGL_renderInfo.texCoords);
        RSGL_renderInfo.batches = NULL;
        RSGL_renderInfo.len = 0;
        RSGL_renderInfo.vert_len = 0;
    }
}
#endif /* RSGL_GRAPHICS_CONTEXT / !RSGL_NO_RGFW */

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
}
void RSGL_setProgram(u32 program) { 
    if (RSGL_args.program != program) {
        /* render using the current program */
        RSGL_renderBatch(&RSGL_renderInfo);
        RSGL_args.program = program;
    }
}

void RSGL_setGradient(float gradient[], size_t len) {
    RSGL_args.gradient_len = len;
    RSGL_args.gradient = gradient;
}
void RSGL_fill(bool fill) {
    RSGL_args.fill = fill;
}
void RSGL_center(RSGL_point3DF center) {
    RSGL_args.center = center;
}
void RSGL_setClearArgs(bool clearArgs) {
    RSGL_argsClear = clearArgs;
}
void RSGL_clearArgs() {
    RSGL_args = (RSGL_drawArgs){NULL, 0, 0, { }, {0, 0, 0}, 1, RSGL_POINT3DF(-1, -1, -1), 0, 0, 0};
}


void RSGL_drawPoint(RSGL_point p, RSGL_color c) {
    RSGL_drawPointF((RSGL_pointF){(float)p.x, (float)p.y}, c);
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

void RSGL_drawPointF(RSGL_pointF p, RSGL_color c) {
    RSGL_drawRectF((RSGL_rectF){p.x, p.y, 1.0f, 1.0f}, c);
}

void RSGL_plotLines(RSGL_pointF* lines, size_t points_count, u32 thickness, RSGL_color c) {
    size_t i;
    for (i = 0; i < points_count; i += 2) {
        RSGL_drawLineF(lines[i], lines[i + 1], thickness, c);
    }
}

void RSGL_drawTriangleF(RSGL_triangleF t, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawTriangleFOutline(t, 1, c);

    RSGL_point3DF center = {RSGL_GET_WORLD_POINT(t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);
    
    float points[] = {RSGL_GET_FINAL_POINT((float)t.p1.x, (float)t.p1.y, 0.0f), 
                      RSGL_GET_FINAL_POINT((float)t.p2.x, (float)t.p2.y, 0.0f), 
                      RSGL_GET_FINAL_POINT((float)t.p3.x, (float)t.p3.y, 0.0f)};
    
    float texPoints[] = {   
                0.0f, 1.0f, 
                1.0f, 1.0f,
                ((float)(t.p3.x - t.p1.x)/t.p2.x < 1) ? (float)(t.p3.x - t.p1.x) / t.p2.x : 0, 0.0f,
    };
    
    RSGL_basicDraw(RSGL_TRIANGLES_2D, (float*)points, (float*)texPoints, c, 3);
}

#ifndef PI
    #define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

void RSGL_drawTriangleHyp(RSGL_pointF p, size_t angle, float hypotenuse, RSGL_color color) {
    float dir = (hypotenuse > 0);
    hypotenuse = fabsf(hypotenuse);

    float base = hypotenuse * (cos(angle) * DEG2RAD);
    float opp = hypotenuse * (sin(angle) * DEG2RAD); 
    
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
        
    float texPoints[] = {
                                0.0f, 0.0f, 
                                0.0f, 1.0f, 
                                1.0f, 0.0f, 
                                1.0f, 1.0f, 
                                1.0f, 0.0f, 
                                0.0f, 1.0f
                            };

    RSGL_point3DF center = (RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    float points[] = {
                                RSGL_GET_FINAL_POINT(r.x, r.y, 0.0f), 
                                RSGL_GET_FINAL_POINT(r.x, r.y + r.h, 0.0f),       
                                RSGL_GET_FINAL_POINT(r.x + r.w, r.y, 0.0f), 

                                RSGL_GET_FINAL_POINT(r.x + r.w, r.y + r.h, 0.0f),        
                                RSGL_GET_FINAL_POINT(r.x + r.w, r.y, 0.0f),     
                                RSGL_GET_FINAL_POINT(r.x, r.y + r.h, 0.0f),  
                            };

    RSGL_basicDraw(RSGL_TRIANGLES_2D, (float*)points, (float*)texPoints, c, 6);
}

void RSGL_drawRoundRectF(RSGL_rectF r, RSGL_point rounding, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawRoundRectFOutline(r, rounding, 1, c);

    RSGL_drawRect((RSGL_rect) {r.x + (rounding.x / 2), r.y, r.w - rounding.x, r.h}, c);
    RSGL_drawRect((RSGL_rect) {r.x, r.y + (rounding.y / 2), r.w,  r.h - rounding.y}, c);

    RSGL_drawArc((RSGL_rect) {r.x, r.y, rounding.x, rounding.y}, (RSGL_point){180, 270}, c);
    RSGL_drawArc((RSGL_rect) {r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y}, (RSGL_point){90, 180}, c);
    RSGL_drawArc((RSGL_rect) {r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y), rounding.x, rounding.y}, (RSGL_point){0, 90}, c);
    RSGL_drawArc((RSGL_rect) {r.x, r.y  + (r.h - rounding.y),  rounding.x, rounding.y}, (RSGL_point){270, 360}, c);
}

void RSGL_drawPolygonFOutlinePro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c);

void RSGL_drawPolygonFPro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c) {
    static float verts[360 * 3];
    static float texcoords[360 * 2];

    if (RSGL_args.fill == false)
        return RSGL_drawPolygonFOutlinePro(o, sides, arc, c);
    
    RSGL_point3DF center =  (RSGL_point3DF){RSGL_GET_WORLD_POINT(o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0)};
    
    o = (RSGL_rectF){o.x, o.y, o.w / 2, o.h / 2};    
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    float displacement = 360.0f / (float)sides;
    float angle = displacement * arc.x;
    
    size_t vIndex = 0;
    size_t tIndex = 0; 

    u32 i;
    for (i = 0; i < sides; i++) {
        RSGL_pointF p = {sinf(angle * DEG2RAD), cosf(angle * DEG2RAD)};

        texcoords[tIndex] = (p.x + 1.0f) * 0.5;
        texcoords[tIndex + 1] = (p.y + 1.0f) * 0.5;

        memcpy(verts + vIndex, (float[3]){RSGL_GET_FINAL_POINT(o.x + o.w + (p.x * o.w), o.y + o.h + (p.y * o.h), 0.0)}, 3 * sizeof(float));

        angle += displacement;
        tIndex += 2;
        vIndex += 3;
    }

    texcoords[tIndex + 1] = 0;
    texcoords[tIndex + 2] = 0;

    RSGL_basicDraw(RSGL_TRIANGLE_FAN_2D, verts, texcoords, c, vIndex / 3);
}

void RSGL_drawPolygonF(RSGL_rectF o, u32 sides, RSGL_color c) { RSGL_drawPolygonFPro(o, sides, (RSGL_pointF){0, (int)sides}, c); }


#ifndef M_PI
#define M_PI		3.14159265358979323846	/* pi */
#endif

void RSGL_drawArcF(RSGL_rectF o, RSGL_pointF arc, RSGL_color color) {  
    u32 verts = (u32)round((float)((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10));
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
    RSGL_args.lineWidth = thickness;
    
    RSGL_point3DF center = {RSGL_GET_WORLD_POINT((p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, 0.0f)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    float points[] = {RSGL_GET_FINAL_POINT(p1.x, p1.y, 0.0f), RSGL_GET_FINAL_POINT(p2.x, p2.y, 0.0f)};
    float texPoints[] = {0, 0.0f,          0, 0.0f};

    RSGL_basicDraw(RSGL_LINES_2D, (float*)points, (float*)texPoints, c, 2);
}

void RSGL_drawTriangleFOutline(RSGL_triangleF t, u32 thickness, RSGL_color c) {
    RSGL_args.lineWidth = thickness;
    RSGL_point3DF center = {RSGL_GET_WORLD_POINT(t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    float points[] = {RSGL_GET_FINAL_POINT(t.p3.x, t.p3.y, 0.0f), 
                        RSGL_GET_FINAL_POINT(t.p1.x, t.p1.y, 0.0f),     
                        RSGL_GET_FINAL_POINT(t.p1.x, t.p1.y, 0.0f), 
                    RSGL_GET_FINAL_POINT(t.p2.x, t.p2.y, 0.0f),     
                    RSGL_GET_FINAL_POINT(t.p2.x, t.p2.y, 0.0f),     
                    RSGL_GET_FINAL_POINT(t.p3.x, t.p3.y, 0.0f)};
    
    float texCoords[18];

    RSGL_basicDraw(RSGL_LINES_2D, (float*)points, texCoords, c, 6);
}
void RSGL_drawRectFOutline(RSGL_rectF r, u32 thickness, RSGL_color c) {
    RSGL_point3DF oCenter = RSGL_args.center;

    RSGL_center((RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)});
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x + r.w, r.y}, thickness, c);

    RSGL_center((RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)});
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x, r.y + r.h}, thickness, c);

    RSGL_center((RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)});
    RSGL_drawLineF((RSGL_pointF){r.x, r.y + r.h}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness, c);

    RSGL_center((RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)});
    RSGL_drawLineF((RSGL_pointF){r.x + r.w, r.y}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness, c);

    RSGL_center(oCenter);
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
    static float verts[360 * 2 * 3];
    static float texCoords[360 * 2 * 2];

    RSGL_point3DF center = (RSGL_point3DF) {RSGL_GET_WORLD_POINT(o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0.0f)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    o = (RSGL_rectF){o.x + (o.w / 2), o.y + (o.h / 2), o.w / 2, o.h / 2};
    
    float displacement = 360.0f / (float)sides;
    float centralAngle = displacement * arc.x;

    i32 i;
    u32 j;
    size_t index = 0;

    for (i = arc.x; i < arc.y; i++) {
        for (j = 0; j < 2; j++) {
            memcpy(verts + index, (float[3]) {
                        RSGL_GET_FINAL_POINT(
                            o.x + (sinf(DEG2RAD * centralAngle) * o.w),
                            o.y + (cosf(DEG2RAD * centralAngle) * o.h),
                            (0.0))
                        }, sizeof(float) * 3);
            
            if (!j) centralAngle += displacement;
            index += 3;
        }
    }

    RSGL_basicDraw(RSGL_LINES_2D, verts, texCoords, c, index / 3);
}

void RSGL_drawPolygonFOutline(RSGL_rectF o, u32 sides, u32 thickness, RSGL_color c) {
    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlinePro(o, sides, (RSGL_pointF){0, (int)sides}, c);
}
void RSGL_drawArcFOutline(RSGL_rectF o, RSGL_pointF arc, u32 thickness, RSGL_color color) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlinePro(o, verts, arc, color);
}
void RSGL_drawCircleFOutline(RSGL_circleF c, u32 thickness, RSGL_color color) {
    float verts = ((2 * M_PI * c.d) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlinePro((RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, verts}, color);
}
void RSGL_drawOvalFOutline(RSGL_rectF o, u32 thickness, RSGL_color c) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlinePro(o, verts, (RSGL_pointF){0, verts}, c);
}

#ifndef RSGL_NO_STB_IMAGE
RSGL_image RSGL_drawImage(const char* image, RSGL_rect r) {
    RSGL_image img;
    img.tex = 0;

    #ifndef RSGL_NO_SAVE_IMAGE
    static size_t images_comp = 0;

    if (images_comp == 0) {
        RSGL_images = (RSGL_image*)RSGL_MALLOC(sizeof(RSGL_image) * RSGL_INIT_IMAGES);
        images_comp = RSGL_INIT_IMAGES;
    }

    if (RSGL_images_len) {
        size_t i; 
        for (i = 0; i < RSGL_images_len; i++) {
            if (RSGL_cstr_equal(image, RSGL_images[i].file)) {
                img.tex = RSGL_images[i].tex;
                break;
            }
        }
    }
    #endif /* RSGL_NO_SAVE_IMAGE */

    if (img.tex == 0) {
        i32 c;
        u8* bitmap = stbi_load(image, (int*)&img.srcSize.w, (int*)&img.srcSize.h, &c, 0);
        
        img.tex = RSGL_renderCreateTexture(bitmap, (RSGL_area){img.srcSize.w, img.srcSize.h}, c);

        RSGL_FREE(bitmap);

        #ifndef RSGL_NO_SAVE_IMAGE
        if (RSGL_images_len + 1 > images_comp) {
            RSGL_images = (RSGL_image*)RSGL_REALLOC(RSGL_images, sizeof(RSGL_image) * (RSGL_NEW_IMAGES + images_comp));
            images_comp += RSGL_NEW_IMAGES;
        }

        strcpy(img.file, image);

        RSGL_images[RSGL_images_len] = img;
        RSGL_images_len++;
        #endif
    }

    if (r.w || r.h) {
        u32 tex = RSGL_args.texture;
        RSGL_setTexture(img.tex);

        RSGL_drawRect(r, RSGL_RGB(255, 255, 255));

        RSGL_args.texture = tex;
    }

    return img;
}
#endif

#ifndef RSGL_NO_TEXT

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

i32 RSGL_loadFont(const char* font) {
    u32 i;
    for (i = 0; i < RSGL_font.len; i++) 
        if (RSGL_font.fonts[i].name == font)
            return i;
    
    if (access(font, 0)) {
        printf("RSGL_loadFont File %s does not exist.\n", font);
        return -1;
    }

    if (RSGL_font.len == RSGL_font.cap) {
        RSGL_font.cap += RSGL_NEW_FONTS;

        RSGL_fontData* nFonts = (RSGL_fontData*)RSGL_MALLOC(sizeof(RSGL_fontData) * RSGL_font.cap);
        memcpy(nFonts, RSGL_font.fonts, sizeof(RSGL_fontData) * RSGL_font.len);
        RSGL_FREE(RSGL_font.fonts);

        RSGL_font.fonts = nFonts;
    }


    RSGL_font.fonts[RSGL_font.len].name = (char*)font; 
    RSGL_font.fonts[RSGL_font.len].f = RFont_font_init(font);
    RSGL_font.len++;

    return RSGL_font.len - 1;
}

void RSGL_setFont(i32 font) {
    if (font == -1) {
        printf("RSGL_setFont : invalid font\n");
        return;
    }

    RSGL_font.f = RSGL_font.fonts[font].f;
}

void RSGL_setRFont(RFont_font* font) {
    RSGL_font.f = font;
}

#ifndef RSGL_NO_RGFW
void RSGL_drawFPS(RGFW_window* win, RSGL_circle c, RSGL_color color) {
    RSGL_drawText(RSGL_strFmt("FPS : %i", win->event.fps), c, color);
}
#endif

void RSGL_drawText_len(const char* text, size_t len, RSGL_circle c, RSGL_color color) {
    if (text == NULL || RSGL_font.f == NULL)
        return;

    RFont_set_color(color.r / 255.0f, color.b / 255.0f, color.g / 255.0f, color.a / 255.0f);
    RFont_draw_text_len(RSGL_font.f, text, len, c.x, c.y, c.d, 0.0f);
}

void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color) {
    RSGL_drawText_len(text, 0, c, color);
}

RSGL_circle RSGL_alignText(char* str, RSGL_circle c, RSGL_rectF larger, u8 alignment) {
    return RSGL_alignText_len(str, 0, c, larger, alignment);
}

RSGL_circle RSGL_alignText_len(char* str, size_t str_len, RSGL_circle c, RSGL_rectF larger, u8 alignment) {
    RSGL_area area = RSGL_textArea(str, c.d, str_len);
    
    RSGL_rectF smaller = RSGL_RECTF(c.x, c.y, area.w, c.d);
    RSGL_rectF r = RSGL_alignRectF(larger, smaller, alignment);

    return RSGL_CIRCLE(r.x, r.y + (c.d / 4), r.h);
}

RSGL_area RSGL_textArea(const char* text, u32 fontSize, size_t textEnd) {
    if (RSGL_font.f == NULL)
        return RSGL_AREA(0, 0);
    
    return RFont_text_area_len(RSGL_font.f, text, textEnd, fontSize, 0, 0.0);
}

RSGL_area RSGL_textLineArea(const char* text, u32 fontSize, size_t textEnd, size_t line) {
    return RFont_text_area_len(RSGL_font.f, text, textEnd, fontSize, line, 0.0);
}

RSGL_color RFontcolor = RSGL_RGBA(0, 0, 0, 0);
void RFont_render_set_color(float r, float g, float b, float a) {
    RFontcolor = RSGL_RGBA(r * 255, g * 255, b * 255, a * 255);
}

void RFont_render_text(u32 atlas, float* verts, float* tcoords, size_t nverts) {
    RSGL_drawArgs save = RSGL_args;
    RSGL_rotate(RSGL_POINT3D(0, 0, 0));
    RSGL_setTexture(atlas);
    RSGL_basicDraw(RSGL_TRIANGLES_2D_BLEND, verts, tcoords, RFontcolor, nverts);
    RSGL_args = save;
}

void RFont_render_init() { }

void RFont_render_free(u32 atlas) {
   RSGL_renderDeleteTexture(atlas);
}

void RFont_render_legacy(u8 legacy) { RSGL_UNUSED(legacy); }
#endif /* RSGL_NO_TEXT */


/*
******
RSGL_widgets
******
*/

#if !defined(RSGL_NO_WIDGETS)
bool RSGL_expandableRect_update(RSGL_rect* rect, RGFW_Event e) {
    RSGL_rectF fRect = RSGL_RECTF(rect->x, rect->y, rect->w, rect->h);

    bool output = RSGL_expandableRectF_update(&fRect, e);
    *rect = RSGL_RECT(fRect.x, fRect.y, fRect.w, fRect.h);    
    
    return output;
}

bool RSGL_expandableRectF_update(RSGL_rectF* rect, RGFW_Event e) {
    RSGL_button up;
    up.rect = RSGL_RECTF(rect->x, rect->y, rect->w, 10);
    RSGL_button down;
    down.rect = RSGL_RECTF(rect->x, rect->y + (rect->h - 3), rect->w, 3);
    RSGL_button left;
    left.rect = RSGL_RECTF(rect->x, rect->y, 3, rect->h);
    RSGL_button right;
    right.rect = RSGL_RECTF(rect->x + (rect->w), rect->y, 3, rect->h - 3);

    /*if (
            RSGL_rectCollidePointF(*rect, RSGL_POINTF(e.point.x, e.point.y)) == false// ||
            //RSGL_rectCollidePointF(RSGL_RECTF(rect->x + 4, rect->y + 4, rect->w - 4, rect->h - 4), 
            //                        RSGL_POINTF(e.point.x, e.point.y)) == true
        )
        return false;*/

    RSGL_button_update(&up, e);
    RSGL_button_update(&down, e);
    RSGL_button_update(&left, e);
    RSGL_button_update(&right, e);
    
    if (up.status == RSGL_pressed) {
        printf("h\n");
        rect->y += 3;
        return true;
    }

    return false;
}

RSGL_button RSGL_initButton(void) {
    RSGL_button button;
    button.src.tex = 1;
    button.outline = 0;
    button.points = 0;
    button.src.style = 0;
    button.status = 0;
    button.toggle = 0;
    button.src.rounding = RSGL_POINT(0, 0);
    button.src.drawArgs = (RSGL_drawArgs){NULL, 0, 0, RSGL_args.currentRect, {0, 0, 0}, 1, RSGL_POINT3DF(-1, -1, -1), 0, 0, 0};
    button.src.array_count = 0;
    button.radio_select = 0;

    button.src.text.str = NULL;
    button.src.text.text_len = 0;
    RSGL_renderInfo.vert_len = 0;
    button.src.text.c = RSGL_CIRCLE(0, 0, 0);

    button.loaded_states[0].tex = 0;
    button.loaded_states[1].tex = 0;
    button.loaded_states[2].tex = 0;

    button.src.keys_len = 0;
    button.src.window = NULL;

    return button;
}

RSGL_button RSGL_copyButton(RSGL_button button) {
    RSGL_button newButton;
    newButton.src.color = button.src.color;

    newButton.status = button.status;
    newButton.toggle = button.toggle; 

    newButton.src.text.str = button.src.text.str;
    newButton.src.text.text_len = button.src.text.text_len;
    newButton.src.text.c = button.src.text.c;
    newButton.src.text.color = button.src.text.color;

    newButton.src.tex = button.src.tex;
    newButton.rect = button.rect;
    newButton.points = button.points;
    newButton.outline = button.outline;
    newButton.src.color = button.src.color;
    newButton.src.outlineColor = button.src.outlineColor;
    newButton.src.rounding = button.src.rounding;
    newButton.src.drawArgs = button.src.drawArgs;
    newButton.src.array_count = button.src.array_count;
    newButton.radio_select = button.radio_select;
    
    newButton.src.combo = button.src.combo;

    newButton.src.style = button.src.style;

    return newButton;
}

void RSGL_button_setStyle(RSGL_button* button, RSGL_widgetStyle buttonStyle) {
    if (buttonStyle == 0)
        return;

    button->src.style |= buttonStyle;

    if (buttonStyle & RSGL_STYLE_SLIDER)
        button->src.slider_pos = button->rect.x;

    if (buttonStyle & RSGL_STYLE_ROUNDED) {
        if (buttonStyle & RSGL_STYLE_TOGGLE)
            RSGL_button_setRounding(button, RSGL_POINT(button->rect.w / 2, button->rect.w / 2));
        else
            RSGL_button_setRounding(button, RSGL_POINT(10, 10));
    }

    switch (buttonStyle & RSGL_STYLE_MODE) {
        case RSGL_STYLE_DARK: {
            RSGL_color c = RSGL_RGB(20, 20, 20);

            switch (buttonStyle & RSGL_STYLE_COLOR) {
                case RSGL_STYLE_RED:
                    c.r += 15;
                    break;
                case RSGL_STYLE_BLUE:
                    c.b += 15;
                    break;
                case RSGL_STYLE_GREEN:
                    c.g += 15;
                    break;
                case RSGL_STYLE_YELLOW:
                    c.r += 15;
                    c.g += 15;
                    break;
                case RSGL_STYLE_TEAL:
                    c.g += 15;
                    c.b += 15;
                    break;
                case RSGL_STYLE_PURPLE:
                    c.r += 15;
                    c.b += 15;
                    break;
                default: break;
            }

            RSGL_button_setOutline(button, 1 + ((buttonStyle & RSGL_STYLE_RADIO) ? 5 : 0), RSGL_RGB(c.r + 40, c.g + 40, c.b + 40));
            
            if  (buttonStyle & RSGL_STYLE_RADIO) {
                RSGL_button_setColor(button, c);
                break;
            }

            if (!(buttonStyle & RSGL_STYLE_CHECKBOX)) {   
                RSGL_button_setColor(button, c);
                RSGL_button press = RSGL_copyButton(*button);
                RSGL_button_setColor(&press, RSGL_RGB(c.r + 10, c.g + 10, c.b + 10));
                RSGL_button_setOnPress(button, press.src);
            } else {
                RSGL_button_setColor(button, button->src.outlineColor);
            }
            
            RSGL_button hover = RSGL_copyButton(*button);
            RSGL_button_setOutline(&hover, 1, RSGL_RGB(c.r + 60, c.g + 60, c.b + 60));
            RSGL_button_setOnHover(button, hover.src);
            break;
        }
        case RSGL_STYLE_LIGHT: {
            RSGL_color c = RSGL_RGB(200, 200, 200);

            switch (buttonStyle & RSGL_STYLE_COLOR) {
                case RSGL_STYLE_RED:
                    c.g -= 60;
                    c.b -= 60;
                    break;
                case RSGL_STYLE_BLUE:
                    c.r -= 60;
                    c.g -= 60;
                    break;
                case RSGL_STYLE_GREEN:
                    c.r -= 60;
                    c.b -= 60;
                    break;
                case RSGL_STYLE_YELLOW:
                    c.b -= 60;
                    break;
                case RSGL_STYLE_TEAL:
                    c.r -= 60;
                    break;
                case RSGL_STYLE_PURPLE:
                    c.g -= 60;
                    break;
                default: break;
            }

            RSGL_button_setOutline(button, 1 + ((buttonStyle & RSGL_STYLE_RADIO) ? 5 : 0), RSGL_RGB(c.r - 140, c.g - 140, c.b - 140));
            
            if (buttonStyle & RSGL_STYLE_RADIO) {
                RSGL_button_setColor(button, c);
                break;
            }

            if (!(buttonStyle & RSGL_STYLE_CHECKBOX)) {   
                RSGL_button_setColor(button, c);
                RSGL_button press = RSGL_copyButton(*button);
                RSGL_button_setColor(&press, RSGL_RGB(c.r - 10, c.g - 10, c.b - 10));
                RSGL_button_setOnPress(button, press.src);
            } else {
               RSGL_button_setColor(button, button->src.outlineColor);
            }
            
            RSGL_button_setColor(button, c);
            
            RSGL_button press = RSGL_copyButton(*button);
            RSGL_button_setColor(&press, RSGL_RGB(c.r - 20, c.g - 20, c.b - 20));
            RSGL_button_setOnPress(button, press.src);
            
            RSGL_button hover = RSGL_copyButton(*button);
            RSGL_button_setOutline(&hover, 1 + ((buttonStyle & RSGL_STYLE_RADIO) ? 5 :0), RSGL_RGB(0, 0, 0));
            RSGL_button_setOnHover(button, hover.src);
            break;
        }
        default: break;
    }
}

void RSGL_button_setRect(RSGL_button* button, RSGL_rect rect) {
    RSGL_button_setRectF(button, RSGL_RECTF(rect.x, rect.y, rect.w, rect.h));
}
void RSGL_button_setRectF(RSGL_button* button, RSGL_rectF rect) {
    button->src.style |= RSGL_SHAPE_RECTF;
    button->rect = rect;
}

void RSGL_button_setRounding(RSGL_button* button, RSGL_point rounding) { button->src.rounding = rounding; }

void RSGL_button_setPolygon(RSGL_button* button, RSGL_rect rect, u32 points) {
    RSGL_button_setPolygonF(button, RSGL_RECTF(rect.x, rect.y, rect.w, rect.h), points);
}
void RSGL_button_setPolygonF(RSGL_button* button, RSGL_rectF rect, u32 points) {
    button->src.style |= RSGL_SHAPE_POLYGONF;
    button->rect = rect;
    button->points = points;

    if (points > 20) {
        points = ((2 * M_PI * ((rect.w + rect.h) / 2.0f)) / 10);
        points = (points > 360 ? 360 : points);
    }
}

void RSGL_button_setText(RSGL_button* button, char* text, size_t text_len, RSGL_circle c, RSGL_color color) { 
    button->src.text.str = NULL;
    button->src.text.text_len = 0;
    button->src.text.c = RSGL_CIRCLE(0, 0, 0);

    button->src.text.str = text; 
    button->src.text.text_len = text_len;
    button->src.text.c = c;
    button->src.text.color = color;
}
void RSGL_button_alignText(RSGL_button* button, u8 alignment) {
    RSGL_circle cir = RSGL_alignText_len(button->src.text.str, button->src.text.text_len, button->src.text.c, button->rect, alignment);

    button->src.text.c = cir;
    button->src.text.alignment = alignment;
}
void RSGL_button_setTexture(RSGL_button* button, u32 tex) { button->src.tex = tex; }
void RSGL_button_setColor(RSGL_button* button, RSGL_color color) { button->src.color = color; }
void RSGL_button_setWindow(RSGL_button* button, void* window) {
    button->src.window = window;
}
void RSGL_button_setKeybind(RSGL_button* button, u32* keys, size_t keys_len) {
    button->src.keys = keys;
    button->src.keys_len = keys_len;
}
void RSGL_button_setOutline(RSGL_button* button, u32 size, RSGL_color color) {
    button->src.outlineColor = color;
    button->outline = size;
}
void RSGL_button_setCombo(RSGL_button* button, char** combo, size_t combo_count) {
    button->src.combo = combo;
    button->src.array_count = combo_count + 1;
}
void RSGL_button_setDrawArgs(RSGL_button* button) {
    RSGL_button_setDrawArgsData(button, RSGL_args);
}
void RSGL_button_setDrawArgsData(RSGL_button* button, RSGL_drawArgs args) {
    button->src.drawArgs = args;
}
void RSGL_button_setOnIdle(RSGL_button* button, RSGL_button_src idle) {
    button->loaded_states[RSGL_none] = idle;
}
void RSGL_button_setOnHover(RSGL_button* button, RSGL_button_src hover) {
    button->loaded_states[RSGL_hovered] = hover;
}
void RSGL_button_setOnPress(RSGL_button* button, RSGL_button_src press) {
    button->loaded_states[RSGL_pressed] = press;
}
void RSGL_button_setRadioCount(RSGL_button* button, size_t array_count) {
    button->src.array_count = array_count;
}

void RSGL_drawButton(RSGL_button button) {
    assert((button.src.style & RSGL_STYLE_SHAPE) != RSGL_SHAPE_NULL);
    u8 align = 0;   

    if (button.src.text.str != NULL)
        align = button.src.text.alignment;
    
    if (button.loaded_states[button.status].tex != 0 && !(button.src.style & RSGL_STYLE_CONTAINER)) {
        button.loaded_states[button.status].array_count = button.src.array_count;
        button.src = button.loaded_states[button.status];
    }

    if (align) 
        RSGL_button_alignText(&button, align);
    
    /* reset args, but save the old ones */
    RSGL_drawArgs args = RSGL_args;
    RSGL_rotate(button.src.drawArgs.rotate);
    RSGL_setGradient(button.src.drawArgs.gradient, button.src.drawArgs.gradient_len);
    RSGL_setTexture(button.src.drawArgs.texture);
    RSGL_fill(button.src.drawArgs.fill);
    RSGL_center(button.src.drawArgs.center);

    if (button.src.style & RSGL_STYLE_CHECKBOX && button.toggle == 0) {
        if (button.src.rounding.x || button.src.rounding.y)
            RSGL_drawRoundRectFOutline(button.rect, button.src.rounding, button.outline, button.src.outlineColor);
        else        
            RSGL_drawRectFOutline(button.rect, button.outline, button.src.outlineColor);
        
        RSGL_args = args;
        return;
    }


    RSGL_rectF rectOutline = RSGL_RECTF(
                                            button.rect.x - button.outline, 
                                            button.rect.y - button.outline, 
                                            button.rect.w + (button.outline * 2.0), 
                                            button.rect.h + (button.outline * 2.0)
                                        );

    RSGL_rectF rect;
    if (button.src.style & RSGL_STYLE_TOGGLE)
        rect = (RSGL_rectF){
            (button.rect.x + (button.rect.w / 12.0)) + ((button.rect.w / 2.0) * button.toggle),
            button.rect.y + (button.rect.h / 4.0) - (button.rect.h / 12.0),
            button.rect.w / 3.0,
            button.rect.h / 1.5,
        };
    else 
        rect = button.rect;
    
    if ((button.src.style & RSGL_STYLE_RADIO) == 0 && (button.src.style & RSGL_STYLE_SLIDER) == 0  && (button.src.style & RSGL_STYLE_COMBOBOX) == 0)
        button.src.array_count = 0;
    
    size_t i;
    for (i = 0; i < (button.src.array_count == 0) + button.src.array_count; i++)  {
        switch (button.src.style & RSGL_STYLE_SHAPE) {
            case RSGL_SHAPE_POLYGONF:
                if (button.src.style &  RSGL_STYLE_TOGGLE) {
                    if (button.src.rounding.x || button.src.rounding.y)
                        RSGL_drawRoundRectF(rectOutline, button.src.rounding, button.src.outlineColor);
                    else
                        RSGL_drawRectF(rectOutline, button.src.outlineColor);
                }

                else if (button.outline != 0) 
                    RSGL_drawPolygonF(rectOutline, button.points, button.src.outlineColor);

                if (button.src.style & RSGL_STYLE_RADIO && i != button.radio_select)
                    break;
                
                RSGL_setTexture(button.src.tex);
                RSGL_drawPolygonF(rect, button.points, button.src.color);
                break;
            case RSGL_SHAPE_RECTF:
                if (button.outline != 0) {
                    if (button.src.rounding.x || button.src.rounding.y)
                        RSGL_drawRoundRectF(rectOutline, button.src.rounding, button.src.outlineColor);
                    else
                        RSGL_drawRectF(rectOutline, button.src.outlineColor);
                
                    rectOutline.y += rectOutline.h;
                }

                RSGL_setTexture(button.src.tex);

                if (button.src.style & RSGL_STYLE_RADIO && i != button.radio_select)
                    break;

                if (button.src.rounding.x || button.src.rounding.y) {
                    RSGL_drawRoundRectF(rect, button.src.rounding, button.src.color);
                    break;
                }

                RSGL_drawRectF(rect, button.src.color);
                break;
            default: break;
        }

        if (button.src.style & RSGL_STYLE_TEXTBOX)
            break;

        if (button.src.style & RSGL_STYLE_SLIDER) {
            RSGL_circle c;
            
            if (button.src.style & RSGL_STYLE_SLIDER_VERTICAL)
                c = RSGL_CIRCLE(button.rect.x - (button.rect.w / 2), button.src.slider_pos, button.rect.w * 2);
            else
                c = RSGL_CIRCLE(button.src.slider_pos, button.rect.y - (button.rect.h / 2), button.rect.h * 2);

            if (button.src.style & RSGL_STYLE_SLIDER_CIRCLE)
                RSGL_drawCircle(c, RSGL_RGB(200, 200, 200));
            else if (button.src.style & RSGL_STYLE_ROUNDED)
                RSGL_drawRoundRect(RSGL_RECT(c.x, c.y, c.d, c.d), button.src.rounding, RSGL_RGB(200, 200, 200));
            else
                RSGL_drawRect(RSGL_RECT(c.x, c.y, c.d, c.d), RSGL_RGB(200, 200, 200));
            break;
        }

        if ((button.src.style & RSGL_STYLE_COMBOBOX)) {
            if (button.toggle == false)
                break;
            
            rect.y = rectOutline.y + (button.outline); 
            continue;
        }

        rectOutline.y += rectOutline.h + 5;
        rect.y = rectOutline.y + (button.outline); 
    }

    if (button.src.text.str != NULL) {
        char* text = button.src.text.str;
        if (button.src.style & RSGL_STYLE_COMBOBOX)
            text = button.src.combo[button.radio_select ? button.radio_select - 1 : 0];

       RSGL_drawText(text,  button.src.text.c, button.src.text.color);
    }
    
    if (button.src.style & RSGL_STYLE_CHECKBOX) {
        RSGL_pointF p1 = RSGL_POINTF(button.rect.x + button.rect.w / 4, button.rect.y + button.rect.h / 2);
        RSGL_pointF p2 = RSGL_POINTF(button.rect.x + button.rect.w / 2, button.rect.y + button.rect.h * 3 / 4);
        RSGL_pointF p3 = RSGL_POINTF(button.rect.x + button.rect.w * 3 / 4, button.rect.y + button.rect.h / 4);

        RSGL_drawLineF(p1, p2, 2, RSGL_RGB(0, 255, 0));
        RSGL_drawLineF(p2, p3, 2, RSGL_RGB(0, 255, 0));
    }
    
    if (button.src.style & RSGL_STYLE_COMBOBOX) {
        RSGL_point cen =  RSGL_POINT(button.rect.x + button.rect.w - 20, button.rect.y + button.rect.h - 5);
        size_t y = ((cen.y - button.rect.h) + 15);

        RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(cen.x - 15, y), cen, RSGL_POINT(cen.x + 15, y)), RSGL_RGB(200, 200, 200));
        
        size_t i;     
        for (i = 0; i < (button.src.array_count - 1) && button.toggle && button.src.array_count; i++) {
            button.src.text.c.y += rectOutline.h + (button.outline); 
            RSGL_drawText(button.src.combo[i],  button.src.text.c, button.src.text.color);
        }
    }

    /* set args back to the old ones */
    RSGL_args = args;
}

void RSGL_button_update(RSGL_button* b, RGFW_Event e) {
    RSGL_pointF mouse = RSGL_POINTF(e.point.x, e.point.y);

    RSGL_rectF rect = b->rect;
    
    if (b->outline > 0) {
        rect = RSGL_RECTF(
                                                b->rect.x - b->outline, 
                                                b->rect.y - b->outline, 
                                                b->rect.w + (b->outline * 2.0), 
                                                b->rect.h + (b->outline * 2.0)
                                            );
    }

    size_t i;
    if (e.type == RSGL_keyPressed) {
        for (i = 0; e.type == RSGL_keyPressed && i < b->src.keys_len; i++) {
            if (RSGL_isPressedI(b->src.window, b->src.keys[i]) == false) 
                break;

            if (i != b->src.keys_len - 1)
                continue;
            
            if (b->status != RSGL_pressed && !(b->src.style & RSGL_STYLE_SLIDER))
                b->toggle = !b->toggle;
            
            b->status = RSGL_pressed;
            return;
        }

        if (b->status == RSGL_pressed)
            b->status = RSGL_none;
        
        return;
    }

    if (e.type == RSGL_keyReleased && b->status == RSGL_pressed) {
        b->status = RSGL_none;
    }

    for (i = 0; i < (b->src.array_count == 0) + b->src.array_count; i++) {
        switch (e.type) {
            case RSGL_mouseButtonPressed:
                if (e.button != RSGL_mouseLeft)
                    break;
                
                if (RSGL_rectCollidePointF(rect, mouse)) {
                    if (b->status != RSGL_pressed && !(b->src.style & RSGL_STYLE_SLIDER))
                        b->toggle = !b->toggle;
                                    
                    if (i == 0 && b->src.style & RSGL_STYLE_COMBOBOX)
                        break;
                    
                    b->status = RSGL_pressed;
                    b->radio_select = i;
                    return;
                }
                break;
            case RSGL_mouseButtonReleased:
                if (e.button != RSGL_mouseLeft)
                    break;
                
                if (b->status == RSGL_pressed && RSGL_rectCollidePointF(rect, mouse)) {
                    b->status = RSGL_hovered;
                    return;
                }
                else
                    b->status = RSGL_none;
                break;
            case RSGL_mousePosChanged:
                if (RSGL_rectCollidePointF(rect, mouse)) {
                    b->status = RSGL_hovered;
                    if (b->src.window == NULL)
                        return;

                    if (b->src.style & RSGL_STYLE_TEXTBOX)
                        RSGL_window_setMouseStandard(b->src.window, RSGL_MOUSE_IBEAM);
                    else
                        RSGL_window_setMouseStandard(b->src.window, RSGL_MOUSE_POINTING_HAND);
                    return;
                }
                else {
                    if (b->src.window && b->status)
                        RSGL_window_setMouseStandard(b->src.window, RSGL_MOUSE_ARROW);

                    b->status = RSGL_none;
                }
                break;  
            default: break;
        }

        if (b->src.style & RSGL_STYLE_COMBOBOX && b->toggle == false)
            break;
        
        rect.y += rect.h + 5;
    }
}

float RSGL_slider_update(RSGL_button* b, RGFW_Event e) {
    RSGL_button_update(b, e);

    float room, length, rect_pos, mouse_pos;
    
    if (b->src.style & RSGL_STYLE_SLIDER_VERTICAL) {
        room = b->rect.w;
        length = b->rect.h;
        rect_pos = b->rect.y;
        mouse_pos = e.point.y;
    } else {
        room = b->rect.h;
        length = b->rect.w;
        rect_pos = b->rect.x;
        mouse_pos = e.point.x;
    }

    if (b->status == RSGL_pressed) 
        b->toggle = true;
    else if (e.type == RSGL_mouseButtonReleased)
        b->toggle = false;
    
    float radius = room * 2;

    if (b->toggle) {
        b->src.slider_pos = mouse_pos;

        if (b->src.slider_pos + radius > (rect_pos + length)) {
            b->src.slider_pos = rect_pos + length - radius;
        }
        
        else if (b->src.slider_pos < rect_pos) {
            b->src.slider_pos = rect_pos;
        }
    }

    return ((b->src.slider_pos - rect_pos) / (length - radius)) * 100;
}

RSGL_button RSGL_nullButton(void) {
    RSGL_button nullButton;
    nullButton.src.tex = 0;

    return nullButton;
}

RSGL_button RSGL_label(char* text, size_t text_len, size_t textSize) {
    RSGL_button label = RSGL_initButton();
    RSGL_button_setRect(&label, RSGL_RECT(50, 50, 100, 50));
    RSGL_button_setColor(&label, RSGL_RGBA(0, 0, 0, 0));
    RSGL_button_setText(&label, text, text_len, RSGL_CIRCLE(0, 0, textSize), RSGL_RGB(100, 100, 100));   
    label.src.text.alignment = RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE;

    return label;
}

#define RSGL_CONTAINER_SRC(c) (RSGL_button*)(c + 1)

RSGL_container* RSGL_initContainer(RSGL_rect r, RSGL_button** buttons, size_t len) {
    RSGL_container_src* new = RSGL_MALLOC(sizeof(RSGL_container_src) + sizeof(RSGL_button));
    RSGL_button* src = RSGL_CONTAINER_SRC(new);

    *src = RSGL_initButton();
    new->title = RSGL_initButton();
    new->held = false;
    new->buttons = buttons;
    new->buttons_len = len;

    RSGL_button_setRect(&new->title, RSGL_RECT(r.x, r.y - 30, r.w, 30));

    RSGL_button_setRect(src, r);
    RSGL_container_setPos(src, RSGL_POINT(r.x, r. y));
    return (RSGL_container*)(new + 1);
}

void RSGL_freeContainer(RSGL_container* con) {
    RSGL_FREE(((RSGL_container_src*)con) - 1);
}

void RSGL_container_setPos(RSGL_container* con, RSGL_point p) {
    RSGL_container_src* container = ((RSGL_container_src*)con) - 1;
    RSGL_button* src = RSGL_CONTAINER_SRC(container);

    RSGL_rectF rect = RSGL_RECTF((float)p.x, (float)p.y, src->rect.w, src->rect.h);
    RSGL_button_setRectF(src, rect);   
    RSGL_button_setRectF(&container->title, RSGL_RECTF(rect.x, rect.y - 30, rect.w, 30));    

    if (container->title.src.text.str != NULL)
        RSGL_button_alignText(&container->title, RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE);
    
    
    size_t i, x;
    float spacing = 0;

    RSGL_button** containers = container->buttons;

    for (i = 0; i < container->buttons_len; i += 3) {
        float newHeight = 0;
        for (x = 0; x < 3; x++) {

            if (containers[i + x]->src.tex == 0)
                continue;
            
            containers[i + x]->rect = RSGL_alignRectF(rect, containers[i + x]->rect, (1 << (1 + x)));

            containers[i + x]->rect.x += ((5 + containers[i + x]->outline) * !x); 
            containers[i + x]->rect.y = rect.y + spacing;

            if (containers[i + x]->src.text.str != NULL)
                RSGL_button_alignText(containers[i + x], containers[i + x]->src.text.alignment);
            
            RSGL_button_setStyle(containers[i + x], containers[i + x]->src.style);

            float height = containers[i + x]->rect.h + (containers[i + x]->outline * 2);
            if (containers[i + x]->src.style & RSGL_STYLE_RADIO) {
                height *= containers[i + x]->src.array_count;
                height += 5 * containers[i + x]->src.array_count;
            }

            if (height > newHeight)
                newHeight = height;
        }
        
        spacing += newHeight + 5;
    }   
}

void RSGL_container_setTitle(RSGL_container* con, char* name, size_t name_len) {
    RSGL_container_src* container = ((RSGL_container_src*)con) - 1;

    RSGL_button_setText(&container->title, name, name_len, RSGL_CIRCLE(0, 0, 25), RSGL_RGB(100, 100, 100));
    RSGL_button_alignText(&container->title, RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE);
}

void RSGL_container_setStyle(RSGL_container* button, u16 buttonStyle) {
    RSGL_container_src* container = ((RSGL_container_src*)button) - 1;
    RSGL_button* src = RSGL_CONTAINER_SRC(container);
    
    RSGL_button_setStyle(src, buttonStyle); 

    if (buttonStyle & RSGL_STYLE_ROUNDED)
        buttonStyle ^= RSGL_STYLE_ROUNDED;
    
    RSGL_button_setStyle(&container->title, buttonStyle);  
}

void RSGL_drawContainer(RSGL_container* con) {   
    RSGL_container_src* container = ((RSGL_container_src*)con) - 1;
    RSGL_button* src = RSGL_CONTAINER_SRC(container);

    if (container->title.toggle == false)
        RSGL_drawButton(*src);
    
    if (!(src->src.style & RSGL_STYLE_NO_TAB)) {
        RSGL_drawButton(container->title);
        
        RSGL_point cen =  RSGL_POINT(container->title.rect.x + container->title.rect.w - 20, container->title.rect.y + container->title.rect.h - 5);
        size_t y = ((cen.y - container->title.rect.h) + 15);

        RSGL_drawTriangle(RSGL_TRIANGLE(RSGL_POINT(cen.x - 15, y), cen, RSGL_POINT(cen.x + 15, y)), RSGL_RGB(200, 200, 200));
    }
    
    size_t i;
    for (i = 0; i < container->buttons_len && container->title.toggle == 0; i++) {
        if (container->buttons[i]->src.tex == 0)
            continue;
        
        switch (container->buttons[i]->src.style & RSGL_STYLE_TYPE) {
            case RSGL_STYLE_TEXTBOX:
                RSGL_textbox_draw(container->buttons[i]);
                break;
            case RSGL_STYLE_CONTAINER:
                RSGL_drawContainer(container->buttons[i]);
                break;
            default:
                RSGL_drawButton(*container->buttons[i]);
                break;
        }
    }
}

i32 RSGL_container_update(RSGL_container* con, RGFW_Event event) {
    RSGL_container_src* container = ((RSGL_container_src*)con) - 1;
    RSGL_button* src = RSGL_CONTAINER_SRC(container);

    if (!(src->src.style & RSGL_STYLE_NO_TAB)) {
        if (event.type == RSGL_mouseButtonReleased &&
            RSGL_rectCollidePoint(RSGL_RECT(container->title.rect.x + (container->title.rect.w - 35), container->title.rect.y, 35, container->title.rect.h), event.point)
        ) {
            container->title.toggle = !container->title.toggle;
            container->held = false;
        }

        
        if (event.type == RSGL_mouseButtonPressed && 
            RSGL_rectCollidePoint(RSGL_RECT(container->title.rect.x, container->title.rect.y, container->title.rect.w - 35, container->title.rect.h), event.point)
        ) {
            container->held = true;
            container->initPoint.x = -1;
        }
        else if (event.type == RSGL_mouseButtonReleased) {
            container->held = false;
            container->initPoint.x = -1;
        }

        if (container->held && event.type == RSGL_mousePosChanged) {
            if (container->initPoint.x == -1) {
                container->initPoint = event.point;
            }
            
            RSGL_container_setPos(con, RSGL_POINT(container->title.rect.x + (event.point.x - container->initPoint.x),
                                                container->title.rect.y + container->title.rect.h + (event.point.y - container->initPoint.y)));
            container->initPoint = event.point;   
        }
    }    

    RSGL_button** containers = container->buttons;

    size_t i;
    for (i = 0; i < container->buttons_len; i++) {
        if (containers[i]->src.tex == 0)
            continue;
        
        switch (container->buttons[i]->src.style & RSGL_STYLE_TYPE) {
            case RSGL_STYLE_TEXTBOX:
                RSGL_textbox_update(containers[i], event);
                break;
            case RSGL_STYLE_CONTAINER:
                RSGL_container_update(containers[i], event);
                break;
            case RSGL_STYLE_SLIDER:
                RSGL_slider_update(containers[i], event);
                break;
            default:
                RSGL_button_update(containers[i], event);
                break;
        }

        if (containers[i]->status)
            return i;
    }

    return -1;
}

#if !defined(RSGL_NO_TEXT)
RSGL_textbox* RSGL_initTextbox(size_t defaultSize) {
    if (defaultSize == 0)
        defaultSize = 2048;
    
    RSGL_textbox* textBox = (RSGL_textbox*)RSGL_MALLOC(sizeof(RSGL_textbox));
    *textBox = RSGL_initButton();

    textBox->src.style |= RSGL_STYLE_TEXTBOX;
    
    textBox->src.text.str = RSGL_MALLOC(sizeof(char) * defaultSize + 1);
    textBox->src.text.str[0] = '\0';

    textBox->src.cursorIndex = 0;
    textBox->line_count = 0;
    textBox->src.text.text_cap = defaultSize;

    return textBox;
}

void RSGL_textbox_free(RSGL_textbox* tb) {
    RSGL_FREE(tb->src.text.str);
    RSGL_FREE(tb);
}

void RSGL_textbox_setTextInfo(RSGL_textbox* tb, RSGL_circle c, RSGL_color color) {
    RSGL_button_setText(tb, tb->src.text.str, tb->src.text.text_len, c, color);
}

void RSGL_textbox_alignText(RSGL_textbox* tb, u8 alignment) {
    RSGL_button_alignText(tb, alignment);
}
void RSGL_textbox_setColor(RSGL_textbox* tb, RSGL_color color) {
    RSGL_button_setColor(tb, color);
}
void RSGL_textbox_setOutline(RSGL_textbox* tb, u32 size, RSGL_color color) {
    RSGL_button_setOutline(tb, size, color);
}
void RSGL_textbox_setStyle(RSGL_textbox* tb, RSGL_widgetStyle buttonStyle) {
    RSGL_button_setStyle(tb, buttonStyle);
}
void RSGL_textbox_setRect(RSGL_textbox* tb, RSGL_rect rect) {
    RSGL_button_setRect(tb, rect);
}
void RSGL_textbox_setRectF(RSGL_textbox* tb, RSGL_rectF rect) {
    RSGL_button_setRectF(tb, rect);
}

void RSGL_textbox_setWindow(RSGL_textbox* button, void* win) {
    RSGL_button_setWindow(button, win);
}

void RSGL_textbox_addChar(RSGL_textbox* tb, char ch) {
    if (ch == 0)
        return;
    
    size_t i, y = 0;
    for (i = 0; i < tb->src.cursorIndex && tb->src.text.str[i]; i++) {
        if (tb->src.text.str[i] == '\n')
            y++;
    }

    u32 width = RSGL_textLineArea(tb->src.text.str, tb->src.text.c.d, tb->src.text.text_len, y + 1).w;

    /* 
        if not aligned to the bottom, subtract half the text size from the box size to avoid 
        too many newlines
    */        
    size_t offset = ((tb->src.text.c.d / 2) * !(tb->src.style & RSGL_ALIGN_DOWN));

    if (ch == '\n' && (tb->line_count + tb->src.text.c.d + (tb->src.text.c.y - tb->rect.y)) < tb->rect.h - offset) {
        tb->line_count += tb->src.text.c.d;
    }
    else if (ch == '\n' || width > tb->rect.w)
        return;

    if (tb->src.cursorIndex > tb->src.text.text_cap) {
        return;
    }

    memcpy(tb->src.text.str + tb->src.cursorIndex + 1, 
            tb->src.text.str + tb->src.cursorIndex, 
            tb->src.text.text_len - tb->src.cursorIndex
        );
    
    tb->src.text.str[tb->src.cursorIndex] = ch;
    tb->src.text.text_len++; 
    tb->src.cursorIndex++;
    tb->src.text.str[tb->src.text.text_len] = '\0';  
}

i32 RSGL_strLineLenL(const char* str) {
    char* s = ((char*)str) - 1;
    for (; *s && *(s + 1) != '\n'; s--);
    
    return (str - s) + 1;
}

size_t RSGL_strLineLenR(const char* str) {
    char* s = (char*)str;
    for (; *s && *(s - 1) != '\n'; s++) { if (s){} }

    return (s - str);
}

char RSGL_keystrToChar(const char* str) {
    if (str[1] == 0)
        return str[0];

    static const char* map[] = {
        "asciitilde", "`",
        "grave", "~",
        "exclam", "!",
        "at", "@",
        "numbersign", "#",
        "dollar", "$",
        "percent", "%%",
        "asciicircum", "^",
        "ampersand", "&",
        "asterisk", "*",
        "parenleft", "(",
        "parenright", ")",
        "underscore", "_",
        "minus", "-",
        "plus", "+",
        "equal", "=",
        "braceleft", "{",
        "bracketleft", "[",
        "bracketright", "]",
        "braceright", "}",
        "colon", ":",
        "semicolon", ";",
        "quotedbl", "\"",
        "apostrophe", "'",
        "bar", "|",
        "backslash", "\'",
        "less", "<",
        "comma", ",",
        "greater", ">",
        "period", ".",
        "question", "?",
        "slash", "/",
        "space", " ",
        "Return", "\n",
        "Enter", "\n",
        "enter", "\n",
    };

    u8 i = 0;
    for (i = 0; i < (sizeof(map) / sizeof(char*)); i += 2)
        if (strcmp(map[i], str) == 0)
            return *map[i + 1];

    return '\0';
}

RSGLDEF size_t RSGL_textbox_point_to_index(RSGL_textbox* tb, RSGL_point touch);

size_t RSGL_textbox_point_to_index(RSGL_textbox* tb, RSGL_point touch) {
    touch.x -= tb->rect.x; 
    touch.y -= tb->rect.y;

    touch.y = touch.y / tb->src.text.c.d;
    size_t index = 0, indexY = 0;
    for (; tb->src.text.str[index] && indexY < (size_t)touch.y; index++) {
        if (tb->src.text.str[index] == '\n')
            indexY++;
    }
    
    size_t indexX = 0;

    if (index < tb->src.text.text_len) {
        size_t len = RSGL_strLineLenR(tb->src.text.str);
    
        for (indexX = 0; 
                RSGL_textLineArea(tb->src.text.str, tb->src.text.c.d, indexX + 1, indexY).w < (u32)touch.x && 
                indexX < len; 
            indexX++);
    }

    if (index + indexX < tb->src.text.text_len)
        return index + indexX;
    
    return 0;
}

RSGL_select RSGL_textbox_update(RSGL_textbox* tb, RGFW_Event event) {
    RSGL_select select; 
    select.selected = false;

    RSGL_button_update(tb, event);   

    if (event.type == RSGL_mouseButtonReleased && 
        RSGL_rectCollidePointF(tb->rect, RSGL_POINTF(event.point.x, event.point.y)) &&  tb->toggle) 
    {
        size_t cursorIndex = RSGL_textbox_point_to_index(tb, event.point);

        if (cursorIndex != tb->src.cursorIndex) {
            select.selectStart = tb->src.cursorIndex;
            select.selectEnd = cursorIndex;
            select.selected = true;
        }
    }

    if (event.type == RSGL_mouseButtonPressed && tb->status != RSGL_pressed)
        tb->toggle = false;
    else if (event.type == RSGL_mouseButtonPressed) {
        tb->toggle = true;
        
        tb->src.cursorIndex = RSGL_textbox_point_to_index(tb, event.point);
    }
    
    if (event.type != RSGL_keyPressed || tb->toggle == false)
        return select;

    if (tb->src.cursorIndex > tb->src.text.text_cap)
        return select;
    
    switch (event.keyCode) {
        case RSGL_BackSpace:
            if (tb->src.cursorIndex == 0)
                return select;

            if (tb->src.text.str[tb->src.cursorIndex - 1] == '\n') {
                tb->line_count -= tb->src.text.c.d; 
            }
            
            strcpy(tb->src.text.str + tb->src.cursorIndex - 1, 
                tb->src.text.str + tb->src.cursorIndex);
            
            tb->src.cursorIndex--;
            return select;
        case RSGL_Tab: {
            u8 i = 0;
            for (i = 0; i < 4; i++)
                RSGL_textbox_addChar(tb, ' ');
            return select;
        }
        case RSGL_Left:
            if (tb->src.cursorIndex == 0)
                return select;
            
            tb->src.cursorIndex--;
            return select;
        case RSGL_Right:
            if (tb->src.cursorIndex >= tb->src.text.text_len)
                return select;
            
            tb->src.cursorIndex++;
            return select;
        case RSGL_Up: {
            i32 val = RSGL_strLineLenL(tb->src.text.str + tb->src.cursorIndex);
            if ((i64)tb->src.cursorIndex - val < 0)
                break;
            
            for (size_t i = 0; i < 2; i++)
                if ((u32)(tb->src.cursorIndex - val) < tb->src.cursorIndex)
                    tb->src.cursorIndex -= val;
            
            return select;
        }
        case RSGL_Down: {
            size_t val = RSGL_strLineLenR(tb->src.text.str + tb->src.cursorIndex);
            
            for (size_t i = 0; i < 2; i++)
                if (tb->src.cursorIndex + val < tb->src.text.text_len)
                    tb->src.cursorIndex += val;
            
            return select;
        } 
        default:
            RSGL_textbox_addChar(tb, RSGL_keystrToChar(event.keyName));
            return select;
    }

    return select;
}

char* RSGL_textbox_getString(RSGL_textbox* tb, size_t* len) {
    if (len != NULL)
        *len = tb->src.text.text_len;
    
    return tb->src.text.str;
}

void RSGL_textbox_draw(RSGL_textbox* tb) {
    RSGL_drawButton(*tb);
    
    RSGL_area area = RSGL_AREA(0, 0); 
    if (tb->src.cursorIndex) {
        area = RSGL_textArea(tb->src.text.str, tb->src.text.c.d, tb->src.cursorIndex);

        if (tb->src.text.alignment & RSGL_ALIGN_CENTER)
            area.w /= 2;
        
        area.h = ((area.h - 1) / tb->src.text.c.d);
    }

    RSGL_drawRect(RSGL_RECT(tb->src.text.c.x + area.w - 2, 
                            (tb->src.text.c.y + area.h * tb->src.text.c.d) - 2, 
                            1, tb->src.text.c.d),
                 tb->src.outlineColor);
}
#endif /* RSGL_NO_TEXT */

#endif /*  RSGL_NO_WIDGETS */

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

    if (miliseconds > (u32)miliLimit)
        miliLimit = miliseconds;

    i32 passed = time(0) - start_time;

    if (passed > miliLimit)
        start_time = time(0);

    return !(passed % miliseconds) && passed;
}

bool RSGL_wait_frames(u32 frames) {
    static i32 i = 0, frameLimit = 0;
    
    if (frames > (u32)frameLimit)
        frameLimit = frames;

    if (i > frameLimit)
        i = 0;

    i++;

    return !(i % frames);
}

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

/* Multiply the current matrix by a translation matrix */
RSGL_MATRIX RSGL_translatef(RSGL_MATRIX* matrix, float x, float y, float z) {
    RSGL_MATRIX matTranslation = { 
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            x,    y,    z,    1.0f
        }
    };

    /* NOTE: We transpose matrix with multiplication order */
    return RSGL_matrixMultiply(matTranslation.m, matrix->m);
}

/* Multiply the current matrix by a rotation matrix */
RSGL_MATRIX RSGL_rotatef(RSGL_MATRIX* matrix, float angle, float x, float y, float z) {
	/* Axis vector (x, y, z) normalization */
	float lengthSquared = x * x + y * y + z * z;
	if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f)) {
		float inverseLength = 1.0f / sqrtf(lengthSquared);
		x *= inverseLength;
		y *= inverseLength;
		z *= inverseLength;
	}

	/* Rotation matrix generation */
	float sinres = sinf(DEG2RAD * angle);
	float cosres = cosf(DEG2RAD * angle);
	float t = 1.0f - cosres;

	float matRotation[16] = 
				{
					x * x * t + cosres,   	  	y * x * t + z * sinres,   	z * x * t - y * sinres,   	0.0f,
					x * y * t - z * sinres,   	y * y * t + cosres,   		z * y * t + x * sinres,   	0.0f,
					x * z * t + y * sinres,   	y * z * t - x * sinres,  	z * z * t + cosres,   		0.0f,
					0.0f,   					0.0f,   					0.0f,   					1.0f
				};

	return RSGL_matrixMultiply(matRotation, matrix->m);
}

RSGL_MATRIX RSGL_matrixMultiply(float left[16], float right[16]) {
    return (RSGL_MATRIX) {
        {
            left[0] * right[0] + left[1] * right[4] + left[2] * right[8] + left[3] * right[12],
            left[0] * right[1] + left[1] * right[5] + left[2] * right[9] + left[3] * right[13],
            left[0] * right[2] + left[1] * right[6] + left[2] * right[10] + left[3] * right[14],
            left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3] * right[15],
            left[4] * right[0] + left[5] * right[4] + left[6] * right[8] + left[7] * right[12],
            left[4] * right[1] + left[5] * right[5] + left[6] * right[9] + left[7] * right[13],
            left[4] * right[2] + left[5] * right[6] + left[6] * right[10] + left[7] * right[14],
            left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7] * right[15],
            left[8] * right[0] + left[9] * right[4] + left[10] * right[8] + left[11] * right[12],
            left[8] * right[1] + left[9] * right[5] + left[10] * right[9] + left[11] * right[13],
            left[8] * right[2] + left[9] * right[6] + left[10] * right[10] + left[11] * right[14],
            left[8] * right[3] + left[9] * right[7] + left[10] * right[11] + left[11] * right[15],
            left[12] * right[0] + left[13] * right[4] + left[14] * right[8] + left[15] * right[12],
            left[12] * right[1] + left[13] * right[5] + left[14] * right[9] + left[15] * right[13],
            left[12] * right[2] + left[13] * right[6] + left[14] * right[10] + left[15] * right[14],
            left[12] * right[3] + left[13] * right[7] + left[14] * right[11] + left[15] * right[15]
        }
    };
}
#endif /* RSGL_IMPLEMENTATION */
