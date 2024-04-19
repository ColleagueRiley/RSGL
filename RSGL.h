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
#define RSGL_TRIANGLE_FAN                         0x0006      /* GL_QUADS */

/* these ensure GL_DEPTH_TEST is disabled when they're being rendered */
#define RSGL_LINES_2D                                0x0011      /* GL_LINES */
#define RSGL_TRIANGLES_2D                            0x0014      /* GL_TRIANGLES */
#define RSGL_QUADS_2D                                0x0016      /* GL_QUADS */
#define RSGL_TRIANGLE_FAN_2D                         0x0017      /* GL_QUADS */


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
    i32 w, h;
} RSGL_area;
#define RSGL_AREA(w, h) (RSGL_area){w, h}

/*  include RGFW here  */

#ifndef RSGL_NO_WINDOW
/* so we're only using one kind of shape data */
#define RGFW_rect RSGL_rect
#define RGFW_vector RSGL_point
#define RGFW_area RSGL_area
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

#ifndef RSGL_NO_WINDOW

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
    create RSGL window, an RGFW window with an RGL setup and init stuff for RSGL 
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
RSGLDEF RSGL_area RSGL_getScreenSize();

RSGLDEF void RSGL_window_move(RSGL_window* win,
								RSGL_point p/* new pos*/
						);

RSGLDEF void RSGL_window_resize(RSGL_window* win,
								RSGL_area a/* new size*/
						);

/* set the minimum size a user can shrink a window */
RSGLDEF void RSGL_window_setMinSize(RSGL_window* win, RSGL_area a);
/* set the minimum size a user can extend a window */
RSGLDEF void RSGL_window_setMaxSize(RSGL_window* win, RSGL_area a);

RSGLDEF void RSGL_window_maximize(RSGL_window* win); /* maximize the window size */
RSGLDEF void RSGL_window_minimize(RSGL_window* win); /* minimize the window (in taskbar (per OS))*/
RSGLDEF void RSGL_window_restore(RSGL_window* win); /* restore the window from minimized (per OS)*/

RSGLDEF void RSGL_window_setName(RSGL_window* win,
								char* name
							);

void RSGL_window_setIcon(RSGL_window* win, /*!< source window */
				u8* icon /*!< icon bitmap */,
				RSGL_area a /*!< width and height of the bitmap*/,
				i32 channels /*!< how many channels the bitmap has (rgb : 3, rgba : 4) */
		); /*!< image resized by default */

/*!< sets mouse to bitmap (very simular to RSGL_window_setIcon), image NOT resized by default*/

RSGLDEF void RSGL_window_setMouse(RSGL_window* win, u8* image, RSGL_area a, i32 channels);
RSGLDEF void RSGL_window_setMouseDefault(RSGL_window* win); /* sets the mouse to1` the default mouse image */

/* where the mouse is on the screen */
RSGLDEF RSGL_point RSGL_getGlobalMousePoint(void);

/* show the mouse or hide the mouse*/
RSGLDEF void RSGL_window_showMouse(RSGL_window* win, i8 show);
/* move the mouse to a set x, y pos*/
RSGLDEF void RSGL_window_moveMouse(RSGL_window* win, RSGL_point v);

/* if the window should close (RSGL_close was sent or escape was pressed) */
RSGLDEF u8 RSGL_window_shouldClose(RSGL_window* win); 
/* if window is fullscreen'd */
RSGLDEF u8 RSGL_window_isFullscreen(RSGL_window* win);
/* if window is hidden */
RSGLDEF u8 RSGL_window_isHidden(RSGL_window* win);
/* if window is minimized */
RSGLDEF u8 RSGL_window_isMinimized(RSGL_window* win);
/* if window is maximized */
RSGLDEF u8 RSGL_window_isMaximized(RSGL_window* win);
/*!< make the window the current opengl drawing context */
RSGLDEF void RSGL_window_makeCurrent(RSGL_window* win); 

/*!< if window == NULL, it checks if the key is pressed globally. Otherwise, it checks only if the key is pressed while the window in focus.*/
RSGLDEF u8 RSGL_isPressedI(RSGL_window* win, u32 key); /*!< if key is pressed (key code)*/

/*
	!!Keycodes defined at the bottom of the header file!!
*/
 /*!< converts a key code to it's key string */
RSGLDEF char* RSGL_keyCodeTokeyStr(u32 key);
/*!< converts a string of a key to it's key code */
RSGLDEF u32 RSGL_keyStrToKeyCode(char* key);
/*!< if key is pressed (key string) */
#define RSGL_isPressedS(win, key) RSGL_isPressedI(win, RSGL_keyStrToKeyCode(key))

/*! clipboard functions*/
RSGLDEF char* RSGL_readClipboard(size_t* size); /*!< read clipboard data */
#define RSGL_clipboardFree free /* the string returned from RSGL_readClipboard must be freed */

RSGLDEF void RSGL_writeClipboard(const char* text, u32 textLen); /*!< write text to the clipboard */

/* 
	convert a keyString to a char version
*/
RSGLDEF char RSGL_keystrToChar(const char*);
/*
	ex.
	"parenleft" -> '('
	"A" -> 'A',
	"Return" -> "\n"
*/

/*! native opengl functions */

/* OpenGL init hints */
RSGLDEF void RSGL_setGLVersion(i32 major, i32 minor);
RSGLDEF void RSGL_setGLStencil(i32 stencil); /* set stencil buffer bit size (8 by default) */
RSGLDEF void RSGL_setGLSamples(i32 samples); /* set number of sampiling buffers (4 by default) */
RSGLDEF void RSGL_setGLStereo(i32 stereo); /* use GL_STEREO (GL_FALSE by default) */
RSGLDEF void RSGL_setGLAuxBuffers(i32 auxBuffers); /* number of aux buffers (0 by default) */

/* supports openGL, directX, OSMesa, EGL and software rendering */
RSGLDEF void RSGL_window_swapBuffers(RSGL_window* win); /* swap the rendering buffer */
RSGLDEF void RSGL_window_swapInterval(RSGL_window* win, i32 swapInterval); 
#else /* RSGL_NO_WINDOW */

/* 
*********************
RSGL_GRAPHICS_CONTEXT
*********************

this is for standalone RSGL graphics, no windowing / RGFW
AN example of this can be found in examples/glfw.c
*/

RSGLDEF void RSGL_initGraphics(
                            RSGL_area r, /* graphics context size */
                            void* loader /* opengl proc address ex. wglProcAddress */
                            ); 
RSGLDEF void RSGL_graphics_clear(RSGL_color c);
RSGLDEF void RSGL_graphics_free();

#endif /* RSGL_GRAPHICS_CONTEXT / !RSGL_NO_WINDOW */

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
    RSGL_color gradient[600]; 
    
    u32 texture;
    u32 gradient_len;

    RSGL_rect currentRect; /* size of current window */
    RSGL_point3D rotate; 

    bool fill;
    RSGL_point3DF center;
} RSGL_drawArgs;

RSGLDEF void RSGL_rotate(RSGL_point3D rotate); /* apply rotation to drawing */
RSGLDEF void RSGL_setTexture(u32 texture); /* apply texture to drawing */
RSGLDEF void RSGL_setGradient(
                                RSGL_color* gradient, /* array of gradients */
                                size_t len /* length of array */
                            ); /* apply gradient to drawing, based on color list*/
RSGLDEF void RSGL_fill(bool fill); /* toggle filling, if fill is false it runs RSGL_draw<whatever>_outline instead */
RSGLDEF void RSGL_center(RSGL_point3DF center); /* the center of the drawing (or shape), this is used for rotation */

/* args clear after a draw function by default, this toggles that */
RSGLDEF void RSGL_setClearArgs(bool clearArgs); /* toggles if args are cleared by default or not */
RSGLDEF void RSGL_clearArgs(); /* clears the args */

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

/* 
RSGL_basicDraw is a function used internally by RSGL, but you can use it yourself
RSGL_basicDraw renders a given set of points based on the data given
*/
RSGLDEF void RSGL_basicDraw(
                u32 RGL_TYPE, /* type of shape, RSGL_QUADS, RSGL_TRIANGLES, RSGL_LINES, RSGL_QUADS_2D */
                RSGL_point3DF* points, /* array of points */
                RSGL_pointF* texPoints, /* array of texture points (must be same length as points)*/
                RSGL_point3DF center, /* the center of the shape */
                RSGL_color c, /* the color to draw the shape */
                size_t len /* the length of the points array */
            );
/* 2D shape drawing */

/* in the function names, F means float */

RSGLDEF void RSGL_drawPoint(RSGL_point p, RSGL_color c);
RSGLDEF void RSGL_drawPointF(RSGL_pointF p, RSGL_color c);
RSGLDEF void RSGL_plotLines(RSGL_pointF* lines, size_t points_count, u32 thickness, RSGL_color c);

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

/* 3D shape drawing */
RSGLDEF void RSGL_drawCube(RSGL_cube r, RSGL_color c);
RSGLDEF void RSGL_drawCubeF(RSGL_cubeF, RSGL_color c);

/* format a string */
#ifndef RSGL_NO_TEXT
RSGLDEF const char* RFont_fmt(const char* string, ...);
#define RSGL_strFmt RFont_fmt

/* loads a font into RSGL, returns it's index into the RSGL_fonts array, this is used as an id in later functions */
RSGLDEF u32 RSGL_loadFont(const char* font);
/* sets font as the current font in use based on index in RSGL_font, given when it was loaded */
RSGLDEF void RSGL_setFont(u32 font);

typedef struct RFont_font RFont_font;
/* sets source RFont font as the current font, given when it was loaded */
RSGLDEF void RSGL_setRFont(RFont_font* font);

/* draws the current fps on the screen */
RSGLDEF void RSGL_drawFPS(RGFW_window* win, RSGL_circle c, RSGL_color color);
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
RSGLDEF u32 RSGL_textWidth(const char* text, u32 fontSize, size_t textEnd);
RSGLDEF u32 RSGL_textLineWidth(const char* text, u32 fontSize, size_t textEnd, size_t line);
#define RSGL_textWidthF(text, fontSize, textEnd) \
    RSGL_setFont(font);\
    RSGL_textWidthF(text, fontSize, textEnd);
#endif /* RSGL_NO_TEXT */

/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF u32 RSGL_createTexture(u8* bitmap, RSGL_area memsize,  u8 channels);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_updateTexture(u32 texture, u8* bitmap, RSGL_area memsize, u8 channels);
/* 
    this creates a texture based on a given image, draws it on a rectangle and then returns the loaded texture 
    
    if the rectangle's width and height are 0 it doesn't draw the image
    the texture is loaded into RSGL_image, this means it doesn't need to be freed
    but you can still free it early
*/

typedef struct RSGL_image { u32 tex; RSGL_area srcSize; char file[255]; } RSGL_image;
RSGLDEF RSGL_image RSGL_drawImage(const char* image, RSGL_rect r);

#define RSGL_loadImage(image) ((RSGL_image) RSGL_drawImage(image, (RSGL_rect){0, 0, 0, 0}))

#define RSGL_deleteTexture(texture) rglDeleteTextures(1, &texture);
#define RSGL_deleteTextures(texture, num) rglDeleteTextures(num, &texture);

/* 
    these two functions can be used before RSGL_createTexture in order to create 
    an swizzle'd texutre or atlas
*/

/* stores the pixel values using `glP ixelStorei` */
RSGLDEF void RSGL_pushPixelValues(i32 alignment, i32 rowLength, i32 skipPixels, i32 skipRows);
/* sets up a swizzle mask */
RSGLDEF void RSGL_textureSwizzleMask(u32 atlas, u32 param, i32 swizzleRgbaParams[4]);
/* load bitmap into atlas */
RSGLDEF void RSGL_atlasAddBitmap(u32 atlas, u8* bitmap, float x, float y, float w, float h);

/* 
*******
RSGL_widgets
*******
*/

#ifndef RGFW_NO_WIDGETS

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

    RSGL_window* window;

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

/* set the window for the button (this is used for RGFW_isPressed), NULL by default */
RSGLDEF void RSGL_button_setWindow(RSGL_button* button, RSGL_window* window);

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
RSGLDEF void RSGL_textbox_update(RSGL_textbox* tb, RGFW_Event event);
RSGLDEF void RSGL_textbox_setTextInfo(RSGL_textbox* tb, RSGL_circle c, RSGL_color color);

RSGLDEF void RSGL_textbox_alignText(RSGL_textbox* tb, u8 alignment);
RSGLDEF void RSGL_textbox_setColor(RSGL_textbox* button, RSGL_color color);
RSGLDEF void RSGL_textbox_setOutline(RSGL_textbox* button, u32 size, RSGL_color color);

RSGLDEF void RSGL_textbox_setStyle(RSGL_textbox* button, RSGL_widgetStyle buttonStyle);
RSGLDEF void RSGL_textbox_setRect(RSGL_textbox* button, RSGL_rect rect);
RSGLDEF void RSGL_textbox_setRectF(RSGL_textbox* button, RSGL_rectF rect);
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

RSGL_drawArgs RSGL_args = {{ }, 1, 0, { }, {0, 0, 0}, 1, RSGL_POINT3DF(-1, -1, -1)};
bool RSGL_argsClear = false;

u32 RSGL_windowsOpen = 0;
RSGL_image* RSGL_images = NULL;
size_t RSGL_images_len = 0;

RSGLDEF void RSGL_drawPrerequisites(RSGL_point3DF center, RSGL_color c);

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

void RSGL_basicDraw(u32 RGL_TYPE, RSGL_point3DF* points, RSGL_pointF* texPoints, RSGL_point3DF center, RSGL_color c, size_t len) {  
    i32 i;

    RSGL_drawPrerequisites(center, c);
        rglBegin(RGL_TYPE);
            for (i = 0; i < len; i++) {
                if (i && i <= RSGL_args.gradient_len)
                    rglColor4ub(RSGL_args.gradient[i - 1].r, RSGL_args.gradient[i - 1].g, RSGL_args.gradient[i - 1].b, RSGL_args.gradient[i - 1].a);
                
                rglTexCoord2f(texPoints[i].x, texPoints[i].y);
                rglVertex3f(points[i].x, points[i].y, points[i].z);
            }
        rglEnd();
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
    if (RGFW_majorVersion == 0)
        RGFW_setGLVersion(3, 3);
    
    RGFW_window* win = RGFW_createWindow(name, r, args);
    RSGL_window_makeCurrent(win);

    if (RSGL_windowsOpen == 0) {
        rglInit((void*)RGFW_getProcAddress);
        rglViewport(0, 0, win->r.w, win->r.h);
        
        rglDepthFunc(RGL_LEQUAL);

        #ifdef RGL_OPENGL_43
        rglClearDepth(1.0f);
        rglBlendFunc(RGL_SRC_ALPHA, RGL_ONE_MINUS_SRC_ALPHA);
        #endif

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

RGFW_Event* RSGL_window_checkEvent(RSGL_window* win) {
    RGFW_Event* e = RGFW_window_checkEvent(win);

    if (win->event.type == RGFW_windowAttribsChange)
        rglViewport(0, 0, win->r.w, win->r.h);

    return e;
}

void RSGL_window_setIconImage(RGFW_window* win, const char* image) {
    i32 x, y, c;
    u8* img = stbi_load(image, &x, &y, &c, 0);

    RSGL_window_setIcon(win, img, RSGL_AREA(x, y), c);
    
    free(img);
}

void RSGL_window_makeCurrent(RSGL_window* win) {
    RGFW_window_makeCurrent(win);

    RSGL_args.currentRect = win->r;
}

void RSGL_window_clear(RSGL_window* win, RSGL_color color) {
    RSGL_window_makeCurrent(win);
    RSGL_window_swapBuffers(win);

    #ifdef RGL_OPENGL_43
    rglClearDepth(1.0f);
    #endif

    RFont_update_framebuffer(win->r.w, win->r.h);
    rglClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    rglClear(RGL_COLOR_BUFFER_BIT | RGL_DEPTH_BUFFER_BIT);

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
            rglDeleteTextures(1, &RSGL_images[i].tex);
    }

    RGFW_window_close(win);
}

RSGL_point RSGL_getGlobalMousePoint(void) {
    return RGFW_getGlobalMousePoint();
}

RSGL_area RSGL_getScreenSize() {
    return RGFW_getScreenSize();
}

void RSGL_window_move(RSGL_window* win, RSGL_point p) {
    return RGFW_window_move(win, p);
}

void RSGL_window_resize(RSGL_window* win, RSGL_area a) {
    return RGFW_window_resize(win, a);
}

void RSGL_window_setMinSize(RSGL_window* win, RSGL_area a) {
    return RGFW_window_setMinSize(win, a);
}

void RSGL_window_setMaxSize(RSGL_window* win, RSGL_area a) {
    return RGFW_window_setMaxSize(win, a);
}

void RSGL_window_maximize(RSGL_window* win) {
    return RGFW_window_maximize(win);
}

void RSGL_window_minimize(RSGL_window* win) {
    RGFW_window_minimize(win);
}

void RSGL_window_restore(RSGL_window* win) {
    RGFW_window_restore(win);
}

void RSGL_window_setName(RSGL_window* win, char* name) {
    RGFW_window_setName(win, name);
}

void RSGL_window_setIcon(RSGL_window* win, u8* icon, RSGL_area a, i32 channels) {
    RGFW_window_setIcon(win, icon, a, channels);
}

void RSGL_window_setMouse(RSGL_window* win, u8* image, RSGL_area a, i32 channels) {
    RGFW_window_setMouse(win, image, a, channels);
}

void RSGL_window_setMouseDefault(RSGL_window* win) {
    RGFW_window_setMouseDefault(win);
}

void RSGL_window_showMouse(RSGL_window* win, i8 show) {
    RGFW_window_showMouse(win, show);
}

void RSGL_window_moveMouse(RSGL_window* win, RSGL_point v) {
    RGFW_window_moveMouse(win, v);
}

u8 RSGL_window_shouldClose(RSGL_window* win) {
    return RGFW_window_shouldClose(win);
}

u8 RSGL_window_isFullscreen(RSGL_window* win) {
    return RGFW_window_isFullscreen(win);
}

u8 RSGL_window_isHidden(RSGL_window* win) {
    return RGFW_window_isHidden(win);
}

u8 RSGL_window_isMinimized(RSGL_window* win) {
    return RGFW_window_isMinimized(win);
}

u8 RSGL_window_isMaximized(RSGL_window* win) {
    return RGFW_window_isMaximized(win);
}

u8 RSGL_isPressedI(RSGL_window* win, u32 key) {
    return RGFW_isPressedI(win, key);
}

char* RSGL_keyCodeTokeyStr(u32 key) {
    return RGFW_keyCodeTokeyStr(key);
}

u32 RSGL_keyStrToKeyCode(char* key) {
    return RGFW_keyStrToKeyCode(key);
}

char* RSGL_readClipboard(size_t* size) {
    return RGFW_readClipboard(size);
}

void RSGL_writeClipboard(const char* text, u32 textLen) {
    return RGFW_writeClipboard(text, textLen);
}

char RSGL_keystrToChar(const char* str) {
    return RGFW_keystrToChar(str);
}

void RSGL_setGLVersion(i32 major, i32 minor) {
    return RGFW_setGLVersion(major, minor);
}

void RSGL_setGLStencil(i32 stencil){ RGFW_setGLStencil(stencil); }
void RSGL_setGLSamples(i32 samples){ RGFW_setGLSamples(samples); }
void RSGL_setGLStereo(i32 stereo){ RGFW_setGLStereo(stereo); }
void RSGL_setGLAuxBuffers(i32 auxBuffers){ RGFW_setGLAuxBuffers(auxBuffers); }

void RSGL_window_swapBuffers(RSGL_window* win) {
    return RGFW_window_swapBuffers(win);
}

void RSGL_window_swapInterval(RSGL_window* win, i32 swapInterval) {
    return RGFW_window_swapInterval(win, swapInterval);
}

#else /* !RGFW_NO_WINDOW */

/* 
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/


void RSGL_initGraphics(RSGL_area r, void* loader) {
    rglInit((void*)RGFW_getProcAddress);
    rglViewport(0, 0, r.w, r.h);
    
    rglDepthFunc(RGL_LEQUAL);

    #ifdef RGL_OPENGL_43
    rglClearDepth(1.0f);
    rglBlendFunc(RGL_SRC_ALPHA, RGL_ONE_MINUS_SRC_ALPHA);
    #endif

    RSGL_args.currentRect = (RSGL_rect){0, 0, r.w, r.h};

    #ifndef RSGL_NO_TEXT
    RFont_init(r.w, r.h);
    #endif
}

void RSGL_graphics_clear(RSGL_color color) {
    #ifdef RGL_OPENGL_43
    rglClearDepth(1.0f);
    #endif

    rglClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    rglClear(RGL_COLOR_BUFFER_BIT | RGL_DEPTH_BUFFER_BIT);
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
            rglDeleteTextures(1, RSGL_images[i].tex);
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
void RSGL_center(RSGL_point3DF center) {
    RSGL_args.center = center;
}
void RSGL_setClearArgs(bool clearArgs) {
    RSGL_POINT3DF(-1, -1, -1);
    RSGL_argsClear = clearArgs;
}
void RSGL_clearArgs() {
    RSGL_args = (RSGL_drawArgs){{ }, 1, 0, { }, {0, 0, 0}, 1, RSGL_POINT3DF(-1, -1, -1)};
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

void RSGL_plotLines(RSGL_pointF* lines, size_t points_count, u32 thickness, RSGL_color c) {
    size_t i;
    for (i = 0; i < points_count; i += 2) {
        RSGL_drawLineF(lines[i], lines[i + 1], thickness, c);
    }
}

void RSGL_drawPoint3DF(RSGL_point3DF p, RSGL_color c) { RSGL_drawCubeF((RSGL_cubeF){p.x, p.y, p.z, 1, 1, 1}, c); }

void RSGL_drawTriangleF(RSGL_triangleF t, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawTriangleFOutline(t, 1, c);

    RSGL_point3DF points[] = {{(float)t.p1.x, (float)t.p1.y, 0.0f}, {(float)t.p2.x, (float)t.p2.y, 0.0f}, {(float)t.p3.x, (float)t.p3.y, 0.0f}};
    RSGL_pointF texPoints[] = {   
                {0.0f, 1.0f}, {1.0f, 1.0f},
                {((float)(t.p3.x - t.p1.x)/t.p2.x < 1) ? (float)(t.p3.x - t.p1.x) / t.p2.x : 0, 0.0f},
    };
    
    RSGL_point3DF center = {t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0};
    RSGL_basicDraw(RGL_TRIANGLES_2D, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, center, c, 3);
}

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
        
    RSGL_pointF texPoints[] = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}};
    RSGL_point3DF points[] = {
                                 {r.x, r.y, 0.0f}, {r.x, r.y + r.h, 0.0f}, {r.x + r.w, r.y, 0.0f}, 
                                 {r.x + r.w, r.y + r.h, 0.0f}, {r.x + r.w, r.y, 0.0f},   {r.x, r.y + r.h, 0.0f},  
                            };

    RSGL_point3DF center = RSGL_POINT3DF(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f);

    RSGL_basicDraw(RGL_TRIANGLES_2D, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, center, c, 6);
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

#define rglColor4ubX(x) if (RSGL_args.gradient_len >= 1) rglColor4ub(RSGL_args.gradient[x].r, RSGL_args.gradient[x].g, RSGL_args.gradient[x].b, RSGL_args.gradient[x].a);

void RSGL_drawPolygonFOutlinePro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c);

void RSGL_drawPolygonFPro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawPolygonFOutlinePro(o, sides, arc, c);
    
    RSGL_drawPrerequisites(RSGL_POINT3DF(o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0.0f), c);
    o = (RSGL_rectF){o.x, o.y, o.w / 2, o.h / 2};
    
    rglBegin(RGL_TRIANGLE_FAN_2D);

    float displacement = 360.0f / (float)sides;
    float angle = displacement * arc.x;

    u32 i;
    for (i = arc.x; i < arc.y; i++) {
        RSGL_pointF p = {sinf(angle * DEG2RAD), cosf(angle * DEG2RAD)};

        rglTexCoord2f((-p.x + 1.0f) * 0.5, (-p.y + 1.0f) * 0.5);
        rglVertex2f(o.x + (o.w) + (p.x * o.w), o.y + (o.h) + (p.y * o.h));

        angle += displacement;
    }

    rglTexCoord2f(0, 0);
    rglEnd();
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

    RSGL_point3DF center = {(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, 0.0f};

    RSGL_basicDraw(RGL_LINES_2D, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, center, c, 2);
}

void RSGL_drawTriangleFOutline(RSGL_triangleF t, u32 thickness, RSGL_color c) {
    rglLineWidth(thickness);
    RSGL_point3DF points[] = {{t.p3.x, t.p3.y, 0.0f}, {t.p1.x, t.p1.y, 0.0f}, {t.p1.x, t.p1.y, 0.0f}, {t.p2.x, t.p2.y, 0.0f}, {t.p2.x, t.p2.y, 0.0f}, {t.p3.x, t.p3.y, 0.0f}};
    RSGL_pointF texPoints[] = {{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}};

    RSGL_point3DF center = {t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0};

    RSGL_basicDraw(RGL_LINES_2D, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, center, c, 6);
}
void RSGL_drawRectFOutline(RSGL_rectF r, u32 thickness, RSGL_color c) {
    RSGL_point3DF oCenter = RSGL_args.center;

    RSGL_center(RSGL_POINT3DF(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f));
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x + r.w, r.y}, thickness, c);

    RSGL_center(RSGL_POINT3DF(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f));
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x, r.y + r.h}, thickness, c);

    RSGL_center(RSGL_POINT3DF(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f));
    RSGL_drawLineF((RSGL_pointF){r.x, r.y + r.h}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness, c);

    RSGL_center(RSGL_POINT3DF(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f));
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
    RSGL_drawPrerequisites(RSGL_POINT3DF(o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0.0f), c);
    o = (RSGL_rectF){o.x + (o.w / 2), o.y + (o.h / 2), o.w / 2, o.h / 2};
 
    float displacement = 360.0f / (float)sides;
    float centralAngle = displacement * arc.x;

    i32 i;

    for (i = arc.x; i < arc.y; i++) {
        rglBegin(RGL_LINES_2D);
        rglColor4ub(c.r, c.g, c.b, c.a);

        rglVertex2f(o.x + (sinf(DEG2RAD * centralAngle) * o.w), o.y + (cosf(DEG2RAD * centralAngle) * o.w));
        centralAngle += displacement;
        rglVertex2f(o.x + (sinf(DEG2RAD * centralAngle) * o.w), o.y + (cosf(DEG2RAD * centralAngle) * o.h));
    }
    rglEnd();
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

    /*

    // Front face  (z = 1.0f)
    rglVertex3f(1.0f, 1.0f, 1.0f);
    rglVertex3f(-1.0f, 1.0f, 1.0f);
    rglVertex3f(-1.0f, -1.0f, 1.0f);
    rglVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    rglVertex3f(1.0f, -1.0f, -1.0f);
    rglVertex3f(-1.0f, -1.0f, -1.0f);
    rglVertex3f(-1.0f, 1.0f, -1.0f);
    rglVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    rglVertex3f(-1.0f, 1.0f, 1.0f);
    rglVertex3f(-1.0f, 1.0f, -1.0f);I
    rglVertex3f(-1.0f, -1.0f, -1.0f);
    rglVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    rglVertex3f(1.0f, 1.0f, -1.0f);
    rglVertex3f(1.0f, 1.0f, 1.0f);
    rglVertex3f(1.0f, -1.0f, 1.0f);
    rglVertex3f(1.0f, -1.0f, -1.0f);
        RSGL_basicDraw(RL_QUADS, (RSGL_point3DF*)points, (RSGL_pointF*)texPoints, RSGL_RECT(r.x, r.y, r.w, r.h), c, sizeof(points)/sizeof(RSGL_point));*/
}

/* textures / images */
u32 RSGL_createTexture(u8* bitmap, RSGL_area memsize, u8 channels) {
    return rglCreateTexture(bitmap, memsize.w, memsize.h, channels);
}

void RSGL_updateTexture(u32 texture, u8* bitmap, RSGL_area memsize, u8 channels) {
    return rglUpdateTexture(texture, bitmap, memsize.w, memsize.h, channels);
}

void RSGL_pushPixelValues(i32 alignment, i32 rowLength, i32 skipPixels, i32 skipRows) {
    return rglPushPixelValues(alignment, rowLength, skipPixels, skipRows);
}

void RSGL_textureSwizzleMask(u32 atlas, u32 param, i32 swizzleRgbaParams[4]) {
    return rglTextureSwizzleMask(atlas, param, swizzleRgbaParams);
}

void RSGL_atlasAddBitmap(u32 atlas, u8* bitmap, float x, float y, float w, float h) {
    rglAtlasAddBitmap(atlas, bitmap, x, y, w, h);
}

RSGL_image RSGL_drawImage(const char* image, RSGL_rect r) {
    RSGL_image img;
    img.tex = 0;

    #ifndef RSGL_NO_SAVE_IMAGE
    static size_t images_comp = 0;

    if (images_comp == 0) {
        RSGL_images = (RSGL_image*)malloc(sizeof(RSGL_image) * RSGL_INIT_IMAGES);
        images_comp = RSGL_INIT_IMAGES;
    }

    if (RSGL_images_len) {
        i32 i; 
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
        u8* bitmap = stbi_load(image, &img.srcSize.w, &img.srcSize.h, &c, 0);
        
        img.tex = RSGL_createTexture(bitmap, (RSGL_area){img.srcSize.w, img.srcSize.h}, c);

        free(bitmap);

        #ifndef RSGL_NO_SAVE_IMAGE
        if (RSGL_images_len + 1 > images_comp) {
            RSGL_images = (RSGL_image*)realloc(RSGL_images, sizeof(RSGL_image) * (RSGL_NEW_IMAGES + images_comp));
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
    rglEnable(RGL_BLEND);

    if (text == NULL || text[0] == '\0')
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
    size_t width = RSGL_textWidth(str, c.d, str_len);

    RSGL_rectF smaller = RSGL_RECTF(c.x, c.y, width, c.d);
    RSGL_rectF r = RSGL_alignRectF(larger, smaller, alignment);

    return RSGL_CIRCLE(r.x, r.y, r.h);
}

u32 RSGL_textWidth(const char* text, u32 fontSize, size_t textEnd) {
    return RFont_text_width_len(RSGL_font.f, text, textEnd, fontSize, 0, 0.0);
}

u32 RSGL_textLineWidth(const char* text, u32 fontSize, size_t textEnd, size_t line) {
    return RFont_text_width_len(RSGL_font.f, text, textEnd, fontSize, line, 0.0);
}
#endif /* RSGL_NO_TEXT */

void RSGL_drawPrerequisites(RSGL_point3DF center, RSGL_color c) {
    rglColor4ub(c.r, c.g, c.b, c.a);
    rglMatrixMode(RGL_PROJECTION);
    rglLoadIdentity();
    
    rglMatrixMode(RGL_MODELVIEW);
    rglPushMatrix();

    rglOrtho(0, RSGL_args.currentRect.w, RSGL_args.currentRect.h, 0, -RSGL_args.currentRect.w, RSGL_args.currentRect.w);

    if (RSGL_args.rotate.x == 0 && RSGL_args.rotate.y == 0 && RSGL_args.rotate.z == 0)
        return;
    
    if (RSGL_args.center.x != -1 && RSGL_args.center.y != -1 &&  RSGL_args.center.z != -1)
        center = RSGL_args.center;

    rglTranslatef(center.x, center.y, center.z);
    rglRotatef(RSGL_args.rotate.z,  0, 0, 1);
    rglRotatef(RSGL_args.rotate.y, 0, 1, 0);
    rglRotatef(RSGL_args.rotate.x, 1, 0, 0);
    rglTranslatef(-center.x, -center.y, -center.z);
}


/*
******
RSGL_widgets
******
*/

#if !defined(RGFW_NO_WIDGETS) && defined (RGFW_mouseButtonPressed)
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
    button.src.drawArgs = (RSGL_drawArgs){{ }, 1, 0, RSGL_args.currentRect, {0, 0, 0}, 1, RSGL_POINT3DF(-1, -1, -1)};
    button.src.array_count = 0;
    button.radio_select = 0;

    button.src.text.str = NULL;
    button.src.text.text_len = 0;
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
void RSGL_button_setWindow(RSGL_button* button, RSGL_window* window) {
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
    button->src.array_count += combo_count + 1;
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

    if (button.src.style & RSGL_STYLE_RADIO == 0 && (button.src.style & RSGL_STYLE_SLIDER) == 0)
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
        for (i = 0; i < (button.src.array_count - 1) && button.toggle; i++) {
            button.src.text.c.y += rectOutline.h + (button.outline); 
            RSGL_drawText(button.src.combo[i],  button.src.text.c, button.src.text.color);
        }
    }

    /* set args back to the old ones */
    RSGL_args = args;
 
    if (button.loaded_states[button.status].tex != 0 && (button.src.style & RSGL_STYLE_CONTAINER)) {
        button.src = button.loaded_states[button.status];
        RSGL_drawButton(button);
    }
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
    if (e.type == RGFW_keyPressed) {
        for (i = 0; e.type == RGFW_keyPressed && i < b->src.keys_len; i++) {
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

    if (e.type == RGFW_keyReleased && b->status == RSGL_pressed) {
        b->status = RSGL_none;
    }

    for (i = 0; i < b->src.array_count + 1; i++) {
        switch (e.type) {
            case RGFW_mouseButtonPressed:
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
            case RGFW_mouseButtonReleased:
                if (e.button != RSGL_mouseLeft)
                    break;
                
                if (b->status == RSGL_pressed && RSGL_rectCollidePointF(rect, mouse)) {
                    b->status = RSGL_hovered;
                    return;
                }
                else
                    b->status = RSGL_none;
                break;
            case RGFW_mousePosChanged:
                if (RSGL_rectCollidePointF(rect, mouse)) {
                    b->status = RSGL_hovered;
                    return;
                }
                else
                    b->status = RSGL_none;
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
    
    if (b->toggle && RSGL_between(mouse_pos, rect_pos, rect_pos + length)) {
        b->src.slider_pos = mouse_pos;

        if (b->src.slider_pos + room * 2 > (rect_pos + length))
            b->src.slider_pos -= room * 2;
    }

    return ((b->src.slider_pos - rect_pos) / (length - (room * 2))) * 100;
}

RSGL_button RSGL_nullButton(void) {
    RSGL_button nullButton;
    nullButton.src.tex = 0;

    return nullButton;
}

RSGL_button RSGL_label(char* text, size_t text_len, size_t textSize) {
    RSGL_button label = RSGL_initButton();
    RSGL_button_setRect(&label, RSGL_RECT(50, 50, 100, 50));
    RSGL_button_setText(&label, text, text_len, RSGL_CIRCLE(0, 0, textSize), RSGL_RGB(100, 100, 100));   
    label.src.text.alignment = RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE;

    return label;
}

#define RSGL_CONTAINER_SRC(c) (RSGL_button*)(c + 1)

RSGL_container* RSGL_initContainer(RSGL_rect r, RSGL_button** buttons, size_t len) {
    RSGL_container_src* new = malloc(sizeof(RSGL_container_src) + sizeof(RSGL_button));
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
    free(((RSGL_container_src*)con) - 1);
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
        size_t y = i / 3;

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
        if (event.type == RGFW_mouseButtonReleased &&
            RSGL_rectCollidePoint(RSGL_RECT(container->title.rect.x + (container->title.rect.w - 35), container->title.rect.y, 35, container->title.rect.h), event.point)
        ) {
            container->title.toggle = !container->title.toggle;
            container->held = false;
        }

        if (event.type == RGFW_mouseButtonPressed && 
            RSGL_rectCollidePoint(RSGL_RECT(container->title.rect.x, container->title.rect.y, container->title.rect.w - 35, container->title.rect.h), event.point)
        )
            container->held = true;
        else if (event.type == RGFW_mousePosChanged && container->held)
            RSGL_container_setPos(con, RSGL_POINT(event.point.x, event.point.y + 30));
        else
            container->held = false;
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

#if !defined(RSGL_NO_TEXT) && defined(RGFW_keyPressed)
RSGL_textbox* RSGL_initTextbox(size_t defaultSize) {
    if (defaultSize == 0)
        defaultSize = 2048;
    
    RSGL_textbox* textBox = (RSGL_textbox*)malloc(sizeof(RSGL_textbox));
    *textBox = RSGL_initButton();

    textBox->src.style |= RSGL_STYLE_TEXTBOX;
    
    textBox->src.text.str = malloc(sizeof(char) * defaultSize + 1);
    textBox->src.text.str[0] = '\0';

    textBox->src.cursorIndex = 0;
    textBox->line_count = 0;
    textBox->src.text.text_cap = defaultSize;

    return textBox;
}

void RSGL_textbox_free(RSGL_textbox* tb) {
    free(tb->src.text.str);
    free(tb);
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

void RSGL_textbox_addChar(RSGL_textbox* tb, char ch) {
    if (ch == 0)
        return;
    
    size_t i, y = 0;
    for (i = 0; i < tb->src.cursorIndex && tb->src.text.str[i]; i++) {
        if (tb->src.text.str[i] == '\n')
            y++;
    }

    u32 width = RSGL_textLineWidth(tb->src.text.str, tb->src.text.c.d, tb->src.text.text_len, y + 1);
            
    if (ch == '\n' && (tb->line_count + tb->src.text.c.d) < tb->rect.h) {
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
    for (s; *s && *(s + 1) != '\n'; s--);
    
    return (str - s) + 1;
}

size_t RSGL_strLineLenR(const char* str) {
    char* s = (char*)str;
    for (s; *s && *(s - 1) != '\n'; s++);

    return (s - str);
}

void RSGL_textbox_update(RSGL_textbox* tb, RGFW_Event event) {
    RSGL_button_update(tb, event);   

    if (event.type == RSGL_mouseButtonPressed && tb->status != RSGL_pressed)
        tb->toggle = false;
    else if (event.type == RSGL_mouseButtonPressed)
        tb->toggle = true;
    
    if (event.type != RSGL_keyPressed || tb->toggle == false)
        return;

    if (tb->src.cursorIndex > tb->src.text.text_cap)
        return;
    
    switch (event.keyCode) {
        case RGFW_BackSpace:
            if (tb->src.cursorIndex == 0)
                return;

            if (tb->src.text.str[tb->src.cursorIndex - 1] == '\n') {
                tb->line_count -= tb->src.text.c.d; 
            }
            
            strcpy(tb->src.text.str + tb->src.cursorIndex - 1, 
                tb->src.text.str + tb->src.cursorIndex);
            
            tb->src.cursorIndex--;
            return;
        case RGFW_Tab: {
            u8 i = 0;
            for (i = 0; i < 4; i++)
                RSGL_textbox_addChar(tb, ' ');
            return;
        }
        case RGFW_Left:
            if (tb->src.cursorIndex == 0)
                return;
            
            tb->src.cursorIndex--;
            return;
        case RGFW_Right:
            if (tb->src.cursorIndex >= tb->src.text.text_len)
                return;
            
            tb->src.cursorIndex++;
            return;
        case RGFW_Up: {
            i32 val = RSGL_strLineLenL(tb->src.text.str + tb->src.cursorIndex);
            if (tb->src.cursorIndex - val < 0)
                break;
            
            tb->src.cursorIndex -= val;
            return;
        }
        case RGFW_Down: {
            size_t val = RSGL_strLineLenR(tb->src.text.str + tb->src.cursorIndex);

            if (tb->src.cursorIndex + val >= tb->src.text.text_len)
                break;
            
            tb->src.cursorIndex += val;
            return;
        }
        default:
            return RSGL_textbox_addChar(tb, RGFW_keystrToChar(event.keyName));
    }
}

char* RSGL_textbox_getString(RSGL_textbox* tb, size_t* len) {
    if (len != NULL)
        *len = tb->src.text.text_len;
    
    return tb->src.text.str;
}

void RSGL_textbox_draw(RSGL_textbox* tb) {
    RSGL_drawButton(*tb);
    
    u32 x = 0; 
    if (tb->src.cursorIndex)
        x = RSGL_textWidth(tb->src.text.str, tb->src.text.c.d, tb->src.cursorIndex);

    if (tb->src.text.alignment & RSGL_ALIGN_CENTER)
        x /= 2;

    size_t i, y = 0;
    for (i = 0; i < tb->src.cursorIndex && tb->src.text.str[i]; i++) {
        if (tb->src.text.str[i] == '\n')
            y++;
    }

    RSGL_drawRect(RSGL_RECT(tb->src.text.c.x + x - 2, 
                            2 + tb->src.text.c.y + y * tb->src.text.c.d, 
                            1, tb->src.text.c.d),
                 tb->src.outlineColor);
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