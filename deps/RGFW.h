/*
* Copyright (C) 2023 ColleagueRiley
*
* libpng license
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
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/

/*
    (MAKE SURE RGFW_IMPLEMENTATION is in exactly one header or you use -D RGFW_IMPLEMENTATION)
	#define RGFW_IMPLEMENTATION - makes it so source code is included with header
*/

/*
	#define RGFW_IMPLEMENTATION - (required) makes it so the source code is included
	#define RGFW_PRINT_ERRORS - (optional) makes it so RGFW prints errors when they're found
	#define RGFW_OSMESA - (optional) use OSmesa as backend (instead of system's opengl api + regular opengl)
	#define RGFW_BUFFER - (optional) just draw directly to (RGFW) window pixel buffer that is drawn to screen (the buffer is in the BGRA format)
	#define RGFW_EGL - (optional) use EGL for loading an OpenGL context (instead of the system's opengl api)
	#define RGFW_OPENGL_ES - (optional) use EGL to load and use Opengl ES for backend rendering (instead of the system's opengl api)
	#define VULKAN - (optional) use vulkan for the rendering backend (rather than opengl)
	#define DIRECTX - (optional) use directX for the rendering backend (rather than opengl) (windows only, defaults to opengl for unix)

	#define RGFW_LINK_EGL (optional) (windows only) if EGL is being used, if EGL functions should be defined dymanically (using GetProcAddress)
	#define RGFW_LINK_OSMESA (optional) (windows only) if EGL is being used, if OS Mesa functions should be defined dymanically  (using GetProcAddress)

	#define RGFW_X11 (optional) (unix only) if X11 should be used. This option is turned on by default by unix systems except for MacOS
	#define RGFW_WGL_LOAD (optional) (windows only) if WGL should be loaded dynamically during runtime
	#define RGFW_NO_X11_CURSOR (optional) (unix only) don't use XCursor
	#define RGFW_NO_X11_CURSOR_PRELOAD (optional) (unix only) Use XCursor, but don't link it in code, (you'll have to link it with -lXcursor)

	#define RGFW_ALLOC_DROPFILES (optional) if room should be allocating for drop files (by default it's global data)
	#define RGFW_MALLOC x - choose what function to use to allocate, by default the standard malloc is used
	#define RGFW_CALLOC x - choose what function to use to allocate (calloc), by default the standard calloc is used
	#define RGFW_FREE x - choose what function to use to allocated memory, by default the standard free is used
*/

/*
	Credits :
		EimaMei/Sacode : Much of the code for creating windows using winapi, Wrote the Silicon library, helped with MacOS Support

		stb - This project is heavily inspired by the stb single header files

		GLFW:
			certain parts of winapi and X11 are very poorly documented,
			GLFW's source code was referenced and used throughout the project (used code is marked in some way),
			this mainly includes, code for drag and drops, code for setting the icon to a bitmap and the code for managing the clipboard for X11 (as these parts are not documented very well)

			GLFW Copyright, https::/github.com/GLFW/GLFW

			Copyright (c) 2002-2006 Marcus Geelnard
			Copyright (c) 2006-2019 Camilla Löwy
*/

#ifndef RGFW_MALLOC
#include <stdlib.h>
#include <time.h>
#define RGFW_MALLOC malloc
#define RGFW_CALLOC calloc
#define RGFW_FREE free
#endif

#if !_MSC_VER
#ifndef inline
#ifndef __APPLE__
#define inline __inline
#endif
#endif
#endif

#ifndef RGFWDEF
#ifdef __APPLE__
#define RGFWDEF extern inline
#else
#define RGFWDEF inline
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* makes sure the header file part is only defined once by default */
#ifndef RGFW_HEADER

#define RGFW_HEADER

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

#if defined(_WIN32) && !defined(RGFW_X11) /* (if you're using X11 on windows some how) */

/* this name looks better */
/* plus it helps with cross-compiling because RGFW_X11 won't be accidently defined */
#define RGFW_WINDOWS 
#include <windows.h>

#else 
#if defined(__unix__) && !defined(__APPLE__) && !defined(RGFW_X11)
#define RGFW_X11
#include <X11/Xlib.h>
#endif
#endif

#if defined(__APPLE__) && defined(RGFW_X11)
#define RGFW_MACOS_X11
#endif

#if defined(__APPLE__) && !defined(RGFW_MACOS_X11)
#define RGFW_MACOS
#endif

#if defined(RGFW_OPENGL_ES) && !defined(RGFW_EGL)
#define RGFW_EGL
#endif
#if defined(RGFW_EGL) && defined(__APPLE__)
#warning  EGL is not supported for Cocoa, switching back to the native opengl api
#undef RGFW_EGL
#endif

#if !defined(RGFW_OSMESA) && !defined(RGFW_EGL) && !defined(RGFW_OPENGL) && !defined (RGFW_VULKAN) && !defined(RGFW_BUFFER)
#define RGFW_OPENGL
#endif

#if defined(RGFW_DIRECTX) && defined(RGFW_OPENGL) && defined(RGFW_WINDOWS)
#undef RGFW_OPENGL
#endif

#ifdef RGFW_VULKAN
#ifndef RGFW_MAX_FRAMES_IN_FLIGHT
#define RGFW_MAX_FRAMES_IN_FLIGHT 2
#endif

#ifdef RGFW_X11
#define VK_USE_PLATFORM_XLIB_KHR
#endif
#ifdef RGFW_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#ifdef RGFW_MACOS
#define VK_USE_PLATFORM_MACOS_MVK
#endif

#include <vulkan/vulkan.h>
#endif

#if defined(RGFW_X11) && defined(RGFW_OPENGL)
#ifndef GLX_MESA_swap_control
#define  GLX_MESA_swap_control
#endif
#include <GL/glx.h> /* GLX defs, xlib.h, gl.h */
#endif

#ifdef RGFW_EGL
#include <EGL/egl.h>
#endif

#ifdef RGFW_OSMESA
#ifndef __APPLE__
#include <GL/osmesa.h>
#else
#include <OpenGL/osmesa.h>
#endif
#endif

#if defined(RGFW_DIRECTX) && defined(RGFW_WINDOWS)
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#ifndef __cplusplus
#define __uuidof(T) IID_##T
#endif
#endif

/*! Optional arguments for making a windows */
#define RGFW_TRANSPARENT_WINDOW		(1L<<9) /*!< the window is transparent */
#define RGFW_NO_BORDER		(1L<<3) /*!< the window doesn't have border */
#define RGFW_NO_RESIZE		(1L<<4) /*!< the window cannot be resized  by the user */
#define RGFW_ALLOW_DND     (1L<<5) /*!< the window supports drag and drop*/
#define RGFW_HIDE_MOUSE (1L<<6) /*! the window should hide the mouse or not (can be toggled later on) using `RGFW_window_mouseShow*/
#define RGFW_FULLSCREEN (1L<<8) /* the window is fullscreen by default or not */
#define RGFW_CENTER (1L<<10) /*! center the window on the screen */
#define RGFW_OPENGL_SOFTWARE (1L<<11) /*! use OpenGL software rendering */

/*! event codes */
#define RGFW_keyPressed 2 /* a key has been pressed */
#define RGFW_keyReleased 3 /*!< a key has been released*/
/*! key event note
	the code of the key pressed is stored in
	RGFW_Event.keyCode
	!!Keycodes defined at the bottom of the header file!!
	
	while a string version is stored in
	RGFW_Event.KeyString

	RGFW_Event.lockState holds the current lockState
	this means if CapsLock, NumLock are active or not
*/
#define RGFW_mouseButtonPressed 4 /*!< a mouse button has been pressed (left,middle,right)*/
#define RGFW_mouseButtonReleased 5 /*!< a mouse button has been released (left,middle,right)*/
#define RGFW_mousePosChanged 6 /*!< the position of the mouse has been changed*/
/*! mouse event note
	the x and y of the mouse can be found in the vector, RGFW_Event.point
	
	RGFW_Event.button holds which mouse button was pressed
*/
#define RGFW_jsButtonPressed 7 /*!< a joystick button was pressed */
#define RGFW_jsButtonReleased 8 /*!< a joystick button was released */
#define RGFW_jsAxisMove 9 /*!< an axis of a joystick was moved*/
/*! joystick event note
	RGFW_Event.joystick holds which joystick was altered, if any
	RGFW_Event.button holds which joystick button was pressed

	RGFW_Event.axis holds the data of all the axis
	RGFW_Event.axisCount says how many axis there are
*/
#define RGFW_windowAttribsChange 10 /*!< the window was moved or resized (by the user) */
/* attribs change event note
	The event data is sent straight to the window structure
	with win->r.x, win->r.y, win->r.w and win->r.h
*/
#define RGFW_quit 33 /*!< the user clicked the quit button*/ 
#define RGFW_dnd 34 /*!< a file has been dropped into the window*/
#define RGFW_dnd_init 35 /*!< the start of a dnd event, when the place where the file drop is known */
/* dnd data note
	The x and y coords of the drop are stored in the vector RGFW_Event.point

	RGFW_Event.droppedFilesCount holds how many files were dropped
	
	This is also the size of the array which stores all the dropped file string,
	RGFW_Event.droppedFiles
*/

/*! mouse button codes (RGFW_Event.button) */
#define RGFW_mouseLeft  1 /*!< left mouse button is pressed*/
#define RGFW_mouseMiddle  2 /*!< mouse-wheel-button is pressed*/
#define RGFW_mouseRight  3 /*!< right mouse button is pressed*/
#define RGFW_mouseScrollUp  4 /*!< mouse wheel is scrolling up*/
#define RGFW_mouseScrollDown  5 /*!< mouse wheel is scrolling down*/

#ifndef RGFW_MAX_PATH
#define RGFW_MAX_PATH 260 /* max length of a path (for dnd) */
#endif
#ifndef RGFW_MAX_DROPS
#define RGFW_MAX_DROPS 260 /* max items you can drop at once */
#endif


/* for RGFW_Event.lockstate */
#define RGFW_CAPSLOCK (1L << 1)
#define RGFW_NUMLOCK (1L << 2)

/*
	When compiling the header, if you choose to, you will get a double link error,
	because of the joystick codes
	simply add -D RGFW_NO_JOYSTICK_CODES to fix the issue
*/
#ifndef RGFW_NO_JOYSTICK_CODES
/*! joystick button codes (based on xbox/playstation), you may need to change these values per controller */
u8 RGFW_JS_A = 0; /* or PS X button */
u8 RGFW_JS_B = 1; /* or PS circle button */
u8 RGFW_JS_Y = 2; /* or PS triangle button */
u8 RGFW_JS_X = 3; /* or PS square button */
u8 RGFW_JS_START = 9; /* start button */
u8 RGFW_JS_SELECT = 8; /* select button */
u8 RGFW_JS_HOME = 10; /* home button */
u8 RGFW_JS_UP = 13; /* dpad up */
u8 RGFW_JS_DOWN = 14; /* dpad down*/
u8 RGFW_JS_LEFT = 15; /* dpad left */
u8 RGFW_JS_RIGHT = 16; /* dpad right */
u8 RGFW_JS_L1 = 4; /* left bump */
u8 RGFW_JS_L2 = 5; /* left trigger*/
u8 RGFW_JS_R1 = 6; /* right bumper */
u8 RGFW_JS_R2 = 7; /* right trigger */
#endif /* RGFW_NO_JOYSTICK_CODES */

/* basic vector type, if there's not already a point/vector type of choice */
#ifndef RGFW_vector
typedef struct {i32 x, y;} RGFW_vector;
#endif

/* basic rect type, if there's not already a rect type of choice */
#ifndef RGFW_rect
typedef struct {i32 x, y, w, h;} RGFW_rect;
#endif

/* basic area type, if there's not already a area type of choice */
#ifndef RGFW_area
typedef struct {u32 w, h;} RGFW_area;
#endif

#define RGFW_VECTOR(x, y) (RGFW_vector){x, y}
#define RGFW_RECT(x, y, w, h) (RGFW_rect){x, y, w, h}
#define RGFW_AREA(w, h) (RGFW_area){w, h}

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
	u32 current_ticks, frames; /* this is used for counting the fps */
	
	u8 lockState;

	u16 joystick; /* which joystick this event applies to (if applicable to any) */

    u8 button; /*!< which mouse button has been clicked (0) left (1) middle (2) right OR which joystick button was pressed*/
  	
	u8 axisesCount; /* number of axises */
	RGFW_vector axis[2]; /* x, y of axises (-100 to 100) */
} RGFW_Event; /*!< Event structure for checking/getting events */

/* source data for the window (used by the APIs) */
typedef struct RGFW_window_src {
	#ifdef RGFW_WINDOWS
    HWND display; /*!< source display */
    HDC window; /*!< source window */
	#endif
	#ifdef RGFW_X11
    Display* display; /*!< source display */
    Window window; /*!< source window */
	#endif
	#ifdef RGFW_MACOS
	void* window;
	#endif

    #if defined(RGFW_OPENGL) && !defined(RGFW_OSMESA)
	#ifdef RGFW_MACOS
	void* rSurf; /*!< source graphics context */
	#endif
	#ifdef RGFW_WINDOWS
	HGLRC rSurf; /*!< source graphics context */
	#endif
	#ifdef RGFW_X11
	GLXContext rSurf; /*!< source graphics context */
	#endif
	#else
	#ifdef RGFW_VULKAN
	VkSurfaceKHR rSurf; /*!< source graphics context */

	/* vulkan data */
    VkSwapchainKHR swapchain;
    u32 image_count;
	VkImage* swapchain_images;
    VkImageView* swapchain_image_views;
	#endif

	#ifdef RGFW_OSMESA
	OSMesaContext rSurf;
	#endif
	#endif

	#ifndef RGFW_WINDOWS
	#ifdef RGFW_X11
	Cursor cursor;
	#else
	void* cursor; /* current cursor (for unix) */
	#endif
	#else
	RGFW_area maxSize, minSize;
	#if defined(RGFW_DIRECTX)
	IDXGISwapChain* swapchain;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* pDepthStencilView;
	#endif
	#endif

	#if defined(RGFW_MACOS) && !defined(RGFW_MACOS_X11)
	void* view; /*apple viewpoint thingy*/
	#endif

	#ifdef RGFW_EGL
	EGLSurface EGL_surface;
	EGLDisplay EGL_display;
	#endif

	#if defined(RGFW_OSMESA) || defined(RGFW_BUFFER) 
	#ifdef RGFW_WINDOWS
	HBITMAP bitmap;
	#endif
	#ifdef RGFW_X11
	XImage* bitmap;
	#endif
	#ifdef RGFW_MACOS
	void* bitmap; /* API's bitmap for storing or managing */
	#endif
	#if defined(RGFW_BUFFER) && defined(RGFW_WINDOWS)
	HDC hdcMem; /* window stored in memory that winapi needs to render buffers */
	#endif
	#endif

	u8 jsPressed[4][16]; /* if a key is currently pressed or not (per joystick) */

	i32 joysticks[4]; /* limit of 4 joysticks at a time */
	u16 joystickCount; /* the actual amount of joysticks */

	#ifdef RGFW_MACOS
	
	#ifdef RGFW_BUFFER
	void* rSurf; /*!< source graphics context */
	#endif

	u8 cursorChanged; /* for steve jobs */
	#endif

	u8 winArgs; /* windows args (for RGFW to check) */ 
	/*
		!< if dnd is enabled or on (based on window creating args) 
		cursorChanged
	*/
} RGFW_window_src;

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

#if defined(RGFW_X11) || defined(RGFW_MACOS)
typedef u64 RGFW_thread; /* thread type unix */
#else
typedef void* RGFW_thread; /* thread type for window */
#endif

RGFW_window* RGFW_createWindow(
	const char* name, /* name of the window */
	RGFW_rect rect, /* rect of window */
	u16 args /* extra arguments (NULL / (u16)0 means no args used)*/
); /*!< function to create a window struct */

/* get the size of the screen to an area struct */
RGFWDEF RGFW_area RGFW_getScreenSize(void);

/* 
	this function checks an *individual* event (and updates window structure attributes)
	this means, using this function without a while loop may cause event lag

	ex. 
	
	while (RGFW_window_checkEvent(win) != NULL) [this keeps checking events until it reaches the last one]
*/

RGFW_Event* RGFW_window_checkEvent(RGFW_window* win); /*!< check events (returns a pointer to win->event or NULL if there is no event)*/

/*! window managment functions*/
RGFWDEF void RGFW_window_close(RGFW_window* win); /*!< close the window and free leftover data */

RGFWDEF void RGFW_window_move(RGFW_window* win,
								RGFW_vector v/* new pos*/
						);

RGFWDEF void RGFW_window_resize(RGFW_window* win,
								RGFW_area a/* new size*/
						);

/* set the minimum size a user can shrink a window */
RGFWDEF void RGFW_window_setMinSize(RGFW_window* win, RGFW_area a);
/* set the minimum size a user can extend a window */
RGFWDEF void RGFW_window_setMaxSize(RGFW_window* win, RGFW_area a);

RGFWDEF void RGFW_window_maximize(RGFW_window* win); /* maximize the window size */
RGFWDEF void RGFW_window_minimize(RGFW_window* win); /* minimize the window (in taskbar (per OS))*/
RGFWDEF void RGFW_window_restore(RGFW_window* win); /* restore the window from minimized (per OS)*/

RGFWDEF void RGFW_window_setName(RGFW_window* win,
								char* name
							);

void RGFW_window_setIcon(RGFW_window* win, /*!< source window */
				u8* icon /*!< icon bitmap */,
				RGFW_area a /*!< width and height of the bitmap*/,
				i32 channels /*!< how many channels the bitmap has (rgb : 3, rgba : 4) */
		); /*!< image resized by default */

/*!< sets mouse to bitmap (very simular to RGFW_window_setIcon), image NOT resized by default*/
RGFWDEF void RGFW_window_setMouse(RGFW_window* win, u8* image, RGFW_area a, i32 channels);
RGFWDEF void RGFW_window_setMouseDefault(RGFW_window* win); /* sets the mouse to1` the default mouse image */

/* where the mouse is on the screen */
RGFWDEF RGFW_vector RGFW_getGlobalMousePoint(void);

/* show the mouse or hide the mouse*/
RGFWDEF void RGFW_window_showMouse(RGFW_window* win, i8 show);
/* move the mouse to a set x, y pos*/
RGFWDEF void RGFW_window_moveMouse(RGFW_window* win, RGFW_vector v);

/* if the window should close (RGFW_close was sent or escape was pressed) */
RGFWDEF u8 RGFW_window_shouldClose(RGFW_window* win); 
/* if window is fullscreen'd */
RGFWDEF u8 RGFW_window_isFullscreen(RGFW_window* win);
/* if window is hidden */
RGFWDEF u8 RGFW_window_isHidden(RGFW_window* win);
/* if window is minimized */
RGFWDEF u8 RGFW_isMinimized(RGFW_window* win);
/* if window is maximized */
RGFWDEF u8 RGFW_isMaximized(RGFW_window* win);
/*!< make the window the current opengl drawing context */
RGFWDEF void RGFW_window_makeCurrent(RGFW_window* win); 

/*error handling*/
RGFWDEF u8 RGFW_Error(); /* returns true if an error has occurred (doesn't print errors itself) */

/*!< if window == NULL, it checks if the key is pressed globally. Otherwise, it checks only if the key is pressed while the window in focus.*/
RGFWDEF u8 RGFW_isPressedI(RGFW_window* win, u32 key); /*!< if key is pressed (key code)*/

/*
	!!Keycodes defined at the bottom of the header file!!
*/
 /*!< converts a key code to it's key string */
RGFWDEF char* RGFW_keyCodeTokeyStr(u32 key);
/*!< converts a string of a key to it's key code */
RGFWDEF u32 RGFW_keyStrToKeyCode(char* key);
/*!< if key is pressed (key string) */
#define RGFW_isPressedS(win, key) RGFW_isPressedI(win, RGFW_keyStrToKeyCode(key))

/*! clipboard functions*/
RGFWDEF char* RGFW_readClipboard(size_t* size); /*!< read clipboard data */
#define RGFW_clipboardFree free /* the string returned from RGFW_readClipboard must be freed */

RGFWDEF void RGFW_writeClipboard(const char* text, u32 textLen); /*!< write text to the clipboard */

/* 
	convert a keyString to a char version
*/
RGFWDEF char RGFW_keystrToChar(const char*);
/*
	ex.
	"parenleft" -> '('
	"A" -> 'A',
	"Return" -> "\n"
*/

#ifndef RGFW_NO_THREADS
/*! threading functions*/

/*! NOTE! (for X11/linux) : if you define a window in a thread, it must be run after the original thread's window is created or else there will be a memory error */
/* 
	I'd suggest you use sili's threading functions instead
	if you're going to use sili
	which is a good idea generally
*/
RGFWDEF RGFW_thread RGFW_createThread(void* (*function_ptr)(void*), void* args); /*!< create a thread*/
RGFWDEF void RGFW_cancelThread(RGFW_thread thread); /*!< cancels a thread*/
RGFWDEF void RGFW_joinThread(RGFW_thread thread); /*!< join thread to current thread */
RGFWDEF void RGFW_setThreadPriority(RGFW_thread thread, u8 priority); /*!< sets the priority priority  */
#endif

/*! gamepad/joystick functions (linux-only currently) */

/*! joystick count starts at 0*/
/*!< register joystick to window based on a number (the number is based on when it was connected eg. /dev/js0)*/
RGFWDEF u16 RGFW_registerJoystick(RGFW_window* win, i32 jsNumber);
RGFWDEF u16 RGFW_registerJoystickF(RGFW_window* win, char* file);

RGFWDEF u8 RGFW_isPressedJS(RGFW_window* win, u16 controller, u8 button);

/*! native opengl functions */
#ifdef RGFW_OPENGL
/*! Get max OpenGL version */
RGFWDEF u8* RGFW_getMaxGLVersion();
/*! Set OpenGL version hint */
RGFWDEF void RGFW_setGLVersion(i32 major, i32 minor);
RGFWDEF void* RGFW_getProcAddress(const char* procname); /* get native opengl proc address */
#endif
/* supports openGL, directX, OSMesa, EGL and software rendering */
RGFWDEF void RGFW_window_swapBuffers(RGFW_window* win); /* swap the rendering buffer */
RGFWDEF void RGFW_window_swapInterval(RGFW_window* win, i32 swapInterval); 

#ifdef RGFW_VULKAN
typedef struct {
    VkInstance instance;
    VkPhysicalDevice physical_device;
    VkDevice device;

    VkDebugUtilsMessengerEXT debugMessenger;

    VkQueue graphics_queue;
    VkQueue present_queue;

    VkFramebuffer* framebuffers;

    VkRenderPass render_pass;
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;

    VkCommandPool command_pool;
    VkCommandBuffer* command_buffers;

    VkSemaphore* available_semaphores;
    VkSemaphore* finished_semaphore;
    VkFence* in_flight_fences;
    VkFence* image_in_flight;
    size_t current_frame;
} RGFW_vulkanInfo;

/*! initializes a vulkan rendering context for the RGFW window,
	this outputs the vulkan surface into win->src.rSurf
	other vulkan data is stored in the global instance of the RGFW_vulkanInfo structure which is returned 
	by the initVulkan() function
	RGFW_VULKAN must be defined for this function to be defined

*/
RGFWDEF RGFW_vulkanInfo* RGFW_initVulkan(RGFW_window* win);
RGFWDEF void RGFW_freeVulkan(void);

RGFWDEF int RGFW_initData(RGFW_window* win);
RGFWDEF RGFW_vulkanInfo* RGFW_createSurface(VkInstance instance, RGFW_window* win);
int RGFW_deviceInitialization(RGFW_window* win);
int RGFW_createSwapchain(RGFW_window* win);
RGFWDEF int RGFW_createRenderPass();
int RGFW_createCommandPool();
int RGFW_createCommandBuffers(RGFW_window* win);
int RGFW_createSyncObjects(RGFW_window* win);
RGFWDEF int RGFW_createFramebuffers(RGFW_window* win);
#endif

#ifdef RGFW_DIRECTX
typedef struct {
	IDXGIFactory* pFactory;
	IDXGIAdapter* pAdapter;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
} RGFW_directXinfo;

/*
	RGFW stores a global instance of RGFW_directXinfo,
	you can use this function to get a pointer the instance
*/
RGFWDEF RGFW_directXinfo* RGFW_getDirectXInfo(void);
#endif

/*! Supporting functions */
RGFWDEF void RGFW_window_checkFPS(RGFW_window* win); /*!< updates fps / sets fps to cap (ran by RGFW_window_checkEvent)*/
RGFWDEF u32 RGFW_getTime(void); /* get time in seconds */
RGFWDEF u32 RGFW_getTimeNS(void); /* get time in nanoseconds */
RGFWDEF u32 RGFW_getFPS(void); /* get current FPS (win->event.fps) */
RGFWDEF void RGFW_sleep(u32 microsecond); /* sleep for a set time */
#endif /* RGFW_HEADER */

/*
(Notes on how to manage Silicon (macos) included)

Example to get you started :

linux : gcc main.c -lX11 -lXcursor -lGL
windows : gcc main.c -lopengl32 -lshell32 -lgdi32
macos : gcc main.c -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo

MACOS NOTE(Colleague Riley): MacOS requires silicon.h to either be included with RGFW or installed globally
							This is because MacOS uses Objective-C for the API so Silicon.h is required to use it in pure C
MACOS NOTE(EimaMei): If you want the MacOS experience to be fully single header, then I'd be best to install Silicon into /usr/local/include

#define RGFW_IMPLEMENTATION
#include "RGFW.h"

u8 icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF,    0xFF, 0x00, 0x00, 0xFF,     0xFF, 0x00, 0x00, 0xFF,   0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF,     0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};

int main() {
    RGFW_window* win = RGFW_createWindow("name", RGFW_RECT(500, 500, 500, 500), (u64)0);

    RGFW_window_setIcon(win, icon, RGFW_AREA(3, 3), 4);

    for (;;) {
        RGFW_window_checkEvent(win); // NOTE: checking events outside of a while loop may cause input lag 
        if (win->event.type == RGFW_quit || RGFW_isPressedI(win, RGFW_Escape))
            break;

		RGFW_window_swapBuffers(win);

		glClearColor(0xFF, 0XFF, 0xFF, 0xFF);
		glClear(GL_COLOR_BUFFER_BIT);
    }

    RGFW_window_close(win);
}

	compiling :

	if you wish to compile the library all you have to do is create a new file with this in it

	rgfw.c
	#define RGFW_IMPLEMENTATION
	#include "RGFW.h"

	then you can use gcc (or whatever compile you wish to use) to compile the library into object file

	ex. gcc -c RGFW.c -fPIC

	after you compile the library into an object file, you can also turn the object file into an static or shared library

	(commands ar and gcc can be replaced with whatever equivalent your system uses)
	static : ar rcs RGFW.a RGFW.o
	shared :
		windows:
			gcc -shared RGFW.o -lopengl32 -lshell32 -lgdi32 -o RGFW.dll
		linux:
			gcc -shared RGFW.o -lX11 -lXcursor -lGL -o RGFW.so
		macos:
			gcc -shared RGFW.o -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo

	Silicon.h, silicon.h is a header file that either needs to be carried around with RGFW or installed into the include folder
*/

#ifdef RGFW_X11
#define RGFW_OS_BASED_VALUE(l, w, m) l
#endif
#ifdef RGFW_WINDOWS
#define RGFW_OS_BASED_VALUE(l, w, m) w
#endif
#ifdef RGFW_MACOS
#define RGFW_OS_BASED_VALUE(l, w, m) m
#endif

#ifdef RGFW_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define RGFW_ASSERT(check, str) {\
	if (!(check)) { \
		printf(str); \
		assert(check); \
	} \
}

u8 RGFW_error = 0;
u8 RGFW_Error() { return RGFW_error; }

#define SET_ATTRIB(a, v) { \
    assert(((size_t) index + 1) < sizeof(attribs) / sizeof(attribs[0])); \
    attribs[index++] = a; \
    attribs[index++] = v; \
}

#define ADD_ATTRIB(a) { \
    assert(((size_t) index + 1) < sizeof(attribs) / sizeof(attribs[0])); \
    attribs[index++] = a; \
}

#ifndef __WIN32
void RGFW_window_showMouse(RGFW_window* win, i8 show) {
	static u8 RGFW_blk[] = {0, 0, 0, 0};
	if (show == 0) 
		RGFW_window_setMouse(win, RGFW_blk, RGFW_AREA(1, 1), 4); 
	else 
		RGFW_window_setMouseDefault(win);
}
#endif

RGFWDEF RGFW_window* RGFW_window_basic_init (RGFW_rect rect, u16 args);
RGFWDEF void RGFW_init_buffer(RGFW_window* win);

RGFW_window* RGFW_window_basic_init (RGFW_rect rect, u16 args) {
	RGFW_window* win = (RGFW_window*)RGFW_MALLOC(sizeof(RGFW_window)); /* make a new RGFW struct */

	#ifdef RGFW_ALLOC_DROPFILES
    win->event.droppedFiles = (char**)RGFW_MALLOC(sizeof(char*) * RGFW_MAX_DROPS);
	
	i32 i;
	for (i = 0; i < RGFW_MAX_DROPS; i++)
		win->event.droppedFiles[i] = (char*)RGFW_CALLOC(RGFW_MAX_PATH, sizeof(char));
	#endif

	#ifndef RGFW_X11 
	RGFW_area screenR = RGFW_getScreenSize();
	#else
	win->src.display = XOpenDisplay(NULL);
	assert(win->src.display != NULL);

	Screen* scrn = DefaultScreenOfDisplay((Display*)win->src.display);
	RGFW_area screenR = RGFW_AREA(scrn->width, scrn->height);
	#endif

	if (RGFW_FULLSCREEN & args)
		rect = RGFW_RECT(0, 0, screenR.w, screenR.h);

	if (RGFW_CENTER & args)
		rect = RGFW_RECT((screenR.w - rect.w) / 2, (screenR.h - rect.h) / 2, rect.w, rect.h);

	/* set and init the new window's data */
	win->r = rect;
	win->fpsCap = 0;
	win->event.inFocus = 1;
	win->event.droppedFilesCount = 0;
	win->src.joystickCount = 0;
	#ifdef RGFW_X11
	win->src.cursor = 0;
	#endif
	#ifdef RGFW_MACOS
	win->src.cursor = NULL;
	#endif
	#ifdef RGFW_WINDOWS
	win->src.maxSize = RGFW_AREA(0, 0);
	win->src.minSize = RGFW_AREA(0, 0);
	#endif
	win->src.winArgs = 0;

	return win;
}

void RGFW_init_buffer(RGFW_window* win) {
	#if defined(RGFW_OSMESA) || defined(RGFW_BUFFER)
	#if !defined(RGFW_WINDOWS) || defined(RGFW_OSMESA)
	win->buffer = RGFW_MALLOC(win->r.w * win->r.h * 4);
	#endif

	#ifdef RGFW_OSMESA
	win->src.rSurf = OSMesaCreateContext(OSMESA_RGBA, NULL);
	OSMesaMakeCurrent(win->src.rSurf, win->buffer, GL_UNSIGNED_BYTE, win->r.w, win->r.h);
	#endif
	#ifdef RGFW_MACOS
		win->src.rSurf = NSGraphicsContext_graphicsContextWithWindow(win->src.window);
		if (win->src.rSurf == NULL)
			printf("Failed to create NSGraphicsContext\n");
	#endif
	#ifdef RGFW_X11
			win->src.bitmap = XCreateImage(win->src.display, DefaultVisual(win->src.display, XDefaultScreen(win->src.display)), DefaultDepth(win->src.display, XDefaultScreen(win->src.display)),
                                ZPixmap, 0, (char*)win->buffer, win->r.w, win->r.h, 32, 0);
	#endif
	#ifdef RGFW_WINDOWS
	BITMAPINFO bmi = {0};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = win->r.w;
	bmi.bmiHeader.biHeight = -win->r.h;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	win->src.bitmap = CreateDIBSection(win->src.window, &bmi, DIB_RGB_COLORS, (void **)&win->buffer, 0, 0);
	win->src.hdcMem = CreateCompatibleDC(win->src.window);
	#endif
	#endif
}

#ifdef RGFW_MACOS
#define GL_SILENCE_DEPRECATION
#define SILICON_IMPLEMENTATION
#include "silicon.h"
#endif

#if defined(RGFW_OPENGL) || defined(RGFW_EGL) || defined(RGFW_OSMESA)
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#endif

#ifdef RGFW_VULKAN
RGFW_vulkanInfo RGFW_vulkan_info;

static VKAPI_ATTR VkBool32 VKAPI_CALL RGFW_vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    fprintf(stderr, "validation layer: %s\n", pCallbackData->pMessage);

    return VK_FALSE;
}

RGFW_vulkanInfo* RGFW_initVulkan(RGFW_window* win) {
	assert(win != NULL);

    if (
        RGFW_initData(win) ||
        RGFW_deviceInitialization(win) ||
        RGFW_createSwapchain(win)
    )
        return NULL;
	
    u32 graphics_family_index = 0;
    u32 present_family_index = 0;

    vkGetDeviceQueue(RGFW_vulkan_info.device, graphics_family_index, 0, &RGFW_vulkan_info.graphics_queue);
    vkGetDeviceQueue(RGFW_vulkan_info.device, present_family_index, 0, &RGFW_vulkan_info.present_queue);

    if (
        RGFW_createRenderPass() ||
        RGFW_createFramebuffers(win) ||
        RGFW_createCommandPool() ||
        RGFW_createCommandBuffers(win) ||
        RGFW_createSyncObjects(win)
    )
        return NULL;
    
    return &RGFW_vulkan_info;
}

int RGFW_initData(RGFW_window* win) {
	assert(win != NULL);

    win->src.swapchain = VK_NULL_HANDLE;
    win->src.image_count = 0;
    RGFW_vulkan_info.current_frame = 0;

    return 0;
}

RGFW_vulkanInfo* RGFW_createSurface(VkInstance instance, RGFW_window* win) {
	assert(win != NULL);
	assert(instance);

	win->src.rSurf = VK_NULL_HANDLE;

	#ifdef RGFW_X11
	VkXlibSurfaceCreateInfoKHR x11 = { VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR, 0, 0, (Display*)win->src.display, (Window)win->src.window };

	vkCreateXlibSurfaceKHR(RGFW_vulkan_info.instance, &x11, NULL, &win->src.rSurf);
	#endif
	#ifdef RGFW_WINDOWS
	VkWin32SurfaceCreateInfoKHR win32 = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, 0, 0, GetModuleHandle(NULL), (HWND)win->src.display };

	vkCreateWin32SurfaceKHR(RGFW_vulkan_info.instance, &win32, NULL, &win->src.rSurf);
	#endif
	#if defined(RGFW_MACOS) && !defined(RGFW_MACOS_X11)
	VkMacOSSurfaceCreateFlagsMVK macos = { VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_KHR, 0, 0, win->src.display, win->src.window };

	vkCreateMacOSSurfaceMVK(RGFW_vulkan_info.instance, &macos, NULL, &win->src.rSurf);
	#endif

	return &RGFW_vulkan_info;
}

int RGFW_deviceInitialization(RGFW_window* win) {
 	assert(win != NULL);
	
    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "RGFW app";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    u32 rgfwExtensionCount = 2;
    const char* rgfwExtensions[2] = {
        (char[]){"VK_KHR_surface"},
#ifdef RGFW_WINDOWS
        (char[]){"VK_KHR_win32_surface"},
#elif defined(RGFW_RGFW_X11)
        (char*[]){"VK_KHR_xlib_surface"},
#elif defined(RGFW_MACOS)
        (char[]){"VK_MVK_macos_surface"},
#endif
    };

    const char** extensions;
    u32 extension_count;
    #ifdef RGFW_DEBUG
        extension_count = rgfwExtensionCount + 1;
        extensions = (const char**)malloc(sizeof(const char*) * extension_count);
        for (int i = 0; i < rgfwExtensionCount; i++) {
            extensions[i] = rgfwExtensions[i];
        }
        extensions[extension_count - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    #else
        extension_count = rgfwExtensionCount;
        extensions = rgfwExtensions;
    #endif

    VkInstanceCreateInfo instance_create_info = {0};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &appInfo;
    instance_create_info.enabledExtensionCount = extension_count;
    instance_create_info.ppEnabledExtensionNames = extensions;


    const char* validation_layer_name[] = {"VK_LAYER_KHRONOS_validation"};

    #ifdef RGFW_DEBUG
        VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {0};
        debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debug_create_info.pfnUserCallback = RGFW_vulkanDebugCallback;

        instance_create_info.enabledLayerCount = 1;
        instance_create_info.ppEnabledLayerNames = validation_layer_name;
        instance_create_info.pNext = &debug_create_info;
    #else
        instance_create_info.enabledLayerCount = 0;
    #endif

    if (vkCreateInstance(&instance_create_info, NULL, &RGFW_vulkan_info.instance) != VK_SUCCESS) {
        fprintf(stderr, "failed to create instance!\n");
        return -1;
    }


    // setup debug messenger
    #ifdef RGFW_DEBUG
        debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debug_create_info.pfnUserCallback = RGFW_vulkanDebugCallback;

        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(RGFW_vulkan_info.instance, "vkCreateDebugUtilsMessengerEXT");
        if (func == NULL) {
            fprintf(stderr, "vkCreateDebugUtilsMessengerEXT not found!\n");
            return -1;
        } else {
            if (func(RGFW_vulkan_info.instance, &debug_create_info, NULL, &RGFW_vulkan_info.debugMessenger) != VK_SUCCESS) {
                fprintf(stderr, "failed to set up debug messenger!\n");
                return -1;
            }
        }
    #endif

    RGFW_createSurface(RGFW_vulkan_info.instance, win);

    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(RGFW_vulkan_info.instance, &deviceCount, NULL);
    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * deviceCount);
    vkEnumeratePhysicalDevices(RGFW_vulkan_info.instance, &deviceCount, devices);

    RGFW_vulkan_info.physical_device = devices[0];

    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(RGFW_vulkan_info.physical_device, &queue_family_count, NULL);
    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(RGFW_vulkan_info.physical_device, &queue_family_count, queueFamilies);
    
    float queuePriority = 1.0f;

    VkPhysicalDeviceFeatures device_features = {0};

    VkDeviceCreateInfo device_create_info = {0};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    VkDeviceQueueCreateInfo queue_create_infos[2] = {
        {0},
        {0},
    };
    queue_create_infos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[0].queueCount = 1;
    queue_create_infos[0].pQueuePriorities = &queuePriority;
    queue_create_infos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[1].queueCount = 1;
    queue_create_infos[1].pQueuePriorities = &queuePriority;
    device_create_info.queueCreateInfoCount = 2;
    device_create_info.pQueueCreateInfos = queue_create_infos;

    device_create_info.enabledExtensionCount = 1;

    const char* device_extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    device_create_info.ppEnabledExtensionNames = device_extensions;
    #ifdef RGFW_DEBUG
        device_create_info.enabledLayerCount = 1;
        device_create_info.ppEnabledLayerNames = validation_layer_name;
    #else
        device_create_info.enabledLayerCount = 0;
    #endif
    device_create_info.pEnabledFeatures = &device_features;

    if (vkCreateDevice(RGFW_vulkan_info.physical_device, &device_create_info, NULL, &RGFW_vulkan_info.device) != VK_SUCCESS) {
        fprintf(stderr, "failed to create logical device!\n");
        return -1;
    }

    return 0;
}

int RGFW_createSwapchain(RGFW_window* win) {
    assert(win != NULL);
	
	VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

    VkSurfaceCapabilitiesKHR capabilities = {0};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(RGFW_vulkan_info.physical_device, win->src.rSurf, &capabilities);

    win->src.image_count = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && win->src.image_count > capabilities.maxImageCount) {
        win->src.image_count = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchain_create_info = {0};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.surface = win->src.rSurf;
    swapchain_create_info.minImageCount = win->src.image_count;
    swapchain_create_info.imageFormat = surfaceFormat.format;
    swapchain_create_info.imageColorSpace = surfaceFormat.colorSpace;
    swapchain_create_info.imageExtent = (VkExtent2D){win->r.w, win->r.h};
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchain_create_info.queueFamilyIndexCount = 2;
    swapchain_create_info.preTransform = capabilities.currentTransform;
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode = presentMode;
    swapchain_create_info.clipped = VK_TRUE;
    swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(RGFW_vulkan_info.device, &swapchain_create_info, NULL, &win->src.swapchain) != VK_SUCCESS) {
        fprintf(stderr, "failed to create swap chain!\n");
        return -1;
    }

    u32 imageCount;
    vkGetSwapchainImagesKHR(RGFW_vulkan_info.device, win->src.swapchain, &imageCount, NULL);
    win->src.swapchain_images = (VkImage*)malloc(sizeof(VkImage) * imageCount);
    vkGetSwapchainImagesKHR(RGFW_vulkan_info.device, win->src.swapchain, &imageCount, win->src.swapchain_images);

    win->src.swapchain_image_views = (VkImageView*)malloc(sizeof(VkImageView) * imageCount);
    for(u32 i=0; i < imageCount; i++){
        VkImageViewCreateInfo image_view_cre_infos = {0};
        image_view_cre_infos.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_cre_infos.image = win->src.swapchain_images[i];
        image_view_cre_infos.viewType =VK_IMAGE_VIEW_TYPE_2D;
        image_view_cre_infos.format = VK_FORMAT_B8G8R8A8_SRGB;
        image_view_cre_infos.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_cre_infos.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_cre_infos.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_cre_infos.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_cre_infos.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_cre_infos.subresourceRange.baseMipLevel = 0;
        image_view_cre_infos.subresourceRange.levelCount = 1;
        image_view_cre_infos.subresourceRange.baseArrayLayer = 0;
        image_view_cre_infos.subresourceRange.layerCount = 1;
        if (vkCreateImageView(RGFW_vulkan_info.device, &image_view_cre_infos, NULL, &win->src.swapchain_image_views[i]) != VK_SUCCESS) {
            fprintf(stderr, "failed to create image views!");
            return -1;
        }
    }

    return 0;
}

int RGFW_createRenderPass(void) {
    VkAttachmentDescription color_attachment = {0};
    color_attachment.format = VK_FORMAT_B8G8R8A8_SRGB;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {0};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {0};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkSubpassDependency dependency = {0};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_info = {0};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;

    if (vkCreateRenderPass(RGFW_vulkan_info.device, &render_pass_info, NULL, &RGFW_vulkan_info.render_pass) != VK_SUCCESS) {
        fprintf(stderr, "failed to create render pass\n");
        return -1; // failed to create render pass!
    }
    return 0;
}

int RGFW_createCommandPool(void) {
    VkCommandPoolCreateInfo pool_info = {0};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = 0;

    if (vkCreateCommandPool(RGFW_vulkan_info.device, &pool_info, NULL, &RGFW_vulkan_info.command_pool) != VK_SUCCESS) {
        fprintf(stderr, "failed to create command pool\n");
        return -1; // failed to create command pool
    }
    return 0;
}

int RGFW_createCommandBuffers(RGFW_window* win) {
	assert(win != NULL);
	
    RGFW_vulkan_info.command_buffers = (VkCommandBuffer*)malloc(sizeof(VkCommandBuffer) * win->src.image_count);

    VkCommandBufferAllocateInfo allocInfo = {0};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = RGFW_vulkan_info.command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (u32)win->src.image_count;

    if (vkAllocateCommandBuffers(RGFW_vulkan_info.device, &allocInfo, RGFW_vulkan_info.command_buffers) != VK_SUCCESS) {
        return -1; // failed to allocate command buffers;
    }

	return 0;
}

int RGFW_createSyncObjects(RGFW_window* win) {
    assert(win != NULL);
	
	RGFW_vulkan_info.available_semaphores = (VkSemaphore*)malloc(sizeof(VkSemaphore) * RGFW_MAX_FRAMES_IN_FLIGHT);
    RGFW_vulkan_info.finished_semaphore = (VkSemaphore*)malloc(sizeof(VkSemaphore) * RGFW_MAX_FRAMES_IN_FLIGHT);
    RGFW_vulkan_info.in_flight_fences = (VkFence*)malloc(sizeof(VkFence) * RGFW_MAX_FRAMES_IN_FLIGHT);
    RGFW_vulkan_info.image_in_flight = (VkFence*)malloc(sizeof(VkFence) * win->src.image_count);

    VkSemaphoreCreateInfo semaphore_info = {0};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info = {0};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < RGFW_MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(RGFW_vulkan_info.device, &semaphore_info, NULL, &RGFW_vulkan_info.available_semaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(RGFW_vulkan_info.device, &semaphore_info, NULL, &RGFW_vulkan_info.finished_semaphore[i]) != VK_SUCCESS ||
            vkCreateFence(RGFW_vulkan_info.device, &fence_info, NULL, &RGFW_vulkan_info.in_flight_fences[i]) != VK_SUCCESS) {
            fprintf(stderr, "failed to create sync objects\n");
            return -1; // failed to create synchronization objects for a frame
        }
    }

    for (size_t i = 0; i < win->src.image_count; i++) {
        RGFW_vulkan_info.image_in_flight[i] = VK_NULL_HANDLE;
    }

    return 0;
}

int RGFW_createFramebuffers(RGFW_window* win) {
    assert(win != NULL);
	
	RGFW_vulkan_info.framebuffers = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * win->src.image_count);

    for (size_t i = 0; i < win->src.image_count; i++) {
        VkImageView attachments[] = { win->src.swapchain_image_views[i] };

        VkFramebufferCreateInfo framebuffer_info = {0};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = RGFW_vulkan_info.render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = win->r.w;
        framebuffer_info.height = win->r.h;
        framebuffer_info.layers = 1;

        if (vkCreateFramebuffer(RGFW_vulkan_info.device, &framebuffer_info, NULL, &RGFW_vulkan_info.framebuffers[i]) != VK_SUCCESS) {
            return -1; // failed to create framebuffer
        }
    }
    return 0;
}

void RGFW_freeVulkan(void) {
    vkDeviceWaitIdle(RGFW_vulkan_info.device);

	for (size_t i = 0; i < RGFW_MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(RGFW_vulkan_info.device, RGFW_vulkan_info.finished_semaphore[i], NULL);
        vkDestroySemaphore(RGFW_vulkan_info.device, RGFW_vulkan_info.available_semaphores[i], NULL);
        vkDestroyFence(RGFW_vulkan_info.device, RGFW_vulkan_info.in_flight_fences[i], NULL);
    }

    vkDestroyCommandPool(RGFW_vulkan_info.device, RGFW_vulkan_info.command_pool, NULL);

    vkDestroyPipeline(RGFW_vulkan_info.device, RGFW_vulkan_info.graphics_pipeline, NULL);
    vkDestroyPipelineLayout(RGFW_vulkan_info.device, RGFW_vulkan_info.pipeline_layout, NULL);
    vkDestroyRenderPass(RGFW_vulkan_info.device, RGFW_vulkan_info.render_pass, NULL);

    #ifdef RGFW_DEBUG
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(RGFW_vulkan_info.instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != NULL) {
            func(RGFW_vulkan_info.instance, RGFW_vulkan_info.debugMessenger, NULL);
        }
    #endif

	vkDestroyDevice(RGFW_vulkan_info.device, NULL);
    vkDestroyInstance(RGFW_vulkan_info.instance, NULL);
    
    free(RGFW_vulkan_info.framebuffers);
    free(RGFW_vulkan_info.command_buffers);
    free(RGFW_vulkan_info.available_semaphores);
    free(RGFW_vulkan_info.finished_semaphore);
    free(RGFW_vulkan_info.in_flight_fences);
    free(RGFW_vulkan_info.image_in_flight);
}

#endif /* RGFW_VULKAN */

RGFW_window* RGFW_root = NULL;

#ifdef RGFW_X11
#include <X11/Xlib.h>
#ifndef RGFW_NO_X11_CURSOR
#include <X11/Xcursor/Xcursor.h>
#endif
#include <dlfcn.h>

#define RGFW_MOUSE_CHANGED	(1L<<1) /*!< mouse change (for winargs)*/
#endif

#ifdef RGFW_WINDOWS
#include <wchar.h>
#include <locale.h>
#include <winuser.h>
#include <windowsx.h>
#include <shellapi.h>
#endif

#if defined(RGFW_MACOS)
u8 RGFW_keyMap[128] = { 0 };
#endif

char* RGFW_keyCodeTokeyStr(u32 key) {				
	#if defined(RGFW_MACOS)
	static char* keyStrs[128] = {"a", "s", "d", "f", "h", "g", "z", "x", "c", "v", "0", "b", "q", "w", "e", "r", "y", "t", "1", "2", "3", "4", "6", "5", "Equals", "9", "7", "Minus", "8", "0", "CloseBracket", "o", "u", "Bracket", "i", "p", "Return", "l", "j", "Apostrophe", "k", "Semicolon", "BackSlash", "Comma", "Slash", "n", "m", "Period", "Tab", "Space", "Backtick", "BackSpace", "0", "Escape", "0", "Super", "Shift", "CapsLock", "Alt", "Control", "0", "0", "0", "0", "0", "KP_Period", "0", "KP_Minus", "0", "0", "0", "0", "Numlock", "0", "0", "0", "KP_Multiply", "KP_Return", "0", "0", "0", "0", "KP_Slash", "KP_0", "KP_1", "KP_2", "KP_3", "KP_4", "KP_5", "KP_6", "KP_7", "0", "KP_8", "KP_9", "0", "0", "0", "F5", "F6", "F7", "F3", "F8", "F9", "0", "F11", "0", "F13", "0", "F14", "0", "F10", "0", "F12", "0", "F15", "Insert", "Home", "PageUp", "Delete", "F4", "End", "F2", "PageDown", "Left", "Right", "Down", "Up", "F1"};

	return keyStrs[key];
	#endif
	#ifdef RGFW_X11
	return XKeysymToString(key);
	#endif
	#ifdef RGFW_WINDOWS
	static char keyName[16];
	GetKeyNameTextA(key, keyName, 16);
	return keyName;
	#endif
}

u32 RGFW_keyStrToKeyCode(char* key) {
#if defined(RGFW_MACOS)
	static char* keyStrs[128] = {"a", "s", "d", "f", "h", "g", "z", "x", "c", "v", "0", "b", "q", "w", "e", "r", "y", "t", "1", "2", "3", "4", "6", "5", "Equals", "9", "7", "Minus", "8", "0", "CloseBracket", "o", "u", "Bracket", "i", "p", "Return", "l", "j", "Apostrophe", "k", "Semicolon", "BackSlash", "Comma", "Slash", "n", "m", "Period", "Tab", "Space", "Backtick", "BackSpace", "0", "Escape", "0", "Super", "Shift", "CapsLock", "Alt", "Control", "0", "0", "0", "0", "0", "KP_Period", "0", "KP_Minus", "0", "0", "0", "0", "Numlock", "0", "0", "0", "KP_Multiply", "KP_Return", "0", "0", "0", "0", "KP_Slash", "KP_0", "KP_1", "KP_2", "KP_3", "KP_4", "KP_5", "KP_6", "KP_7", "0", "KP_8", "KP_9", "0", "0", "0", "F5", "F6", "F7", "F3", "F8", "F9", "0", "F11", "0", "F13", "0", "F14", "0", "F10", "0", "F12", "0", "F15", "Insert", "Home", "PageUp", "Delete", "F4", "End", "F2", "PageDown", "Left", "Right", "Down", "Up", "F1"};

    key--;
    while (key++) {
        u32 i;
        for (i = 0; i < 128; i++) {
            if (*keyStrs[i] == '\1')
                continue;

            if (*keyStrs[i] != *key) {
                keyStrs[i] = "\1";
                continue;
            }
            
            if (*keyStrs[i] == '\0' && *key == '\0')
                return i;

            else
                keyStrs[i]++;
        }

        if (*key == '\0')
            break;
    }

#endif
#ifdef RGFW_X11
	if (strcmp(key, "Space") == 0) key = (char*)"space";

    return XStringToKeysym(key);
#endif
#ifdef RGFW_WINDOWS
	if (key[1]) {
		struct{char* key; i32 code;} keyStrs[] = {
					{"Super_L", VK_LWIN}, 
					{"Super_R", VK_RWIN}, 
					{"Space", VK_SPACE}, 
					{"Return", VK_RETURN}, 
					{"Caps_Lock", VK_CAPITAL}, 
					{"Tab", VK_TAB}, 
					{"Right", VK_RIGHT}, 
					{"Left", VK_LEFT}, 
					{"Up", VK_UP}, 
					{"Down", VK_DOWN}, 
					{"ShiftL", VK_LSHIFT},
					{"ShiftR", VK_RSHIFT}, 
					{"ControlL", VK_RCONTROL},
					{"ControlR", VK_RCONTROL}
				};


		while (key++) {
			u32 i;
			for (i = 0; i < 14; i++) {
				if (*keyStrs[i].key != '\0' && *keyStrs[i].key != '\1')
					keyStrs[i].key++;

				if (*keyStrs[i].key != *key) {
					keyStrs[i].key = "\1";
					continue;
				}

				if (*keyStrs[i].key == '\0' && *key == '\0')
					return keyStrs[i].code;
			}

			if (*key == '\0')
				break;
		}
	}

	i32 vKey = VkKeyScan(key[0]);

    return vKey;
#endif /* RGFW_WINDOWS */

	return 0;
}


char RGFW_keystrToChar(const char* str) {
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
		"Return", "\n"
	};

	u8 i = 0;
	for (i = 0; i < (sizeof(map) / sizeof(char*)); i += 2)
		if (strcmp(map[i], str) == 0)
			return *map[i + 1];

	return '\0';
}

#ifndef M_PI
#define M_PI		3.14159265358979323846	/* pi */
#endif

typedef struct RGFW_Timespec {
	time_t tv_sec;		/* Seconds.  */
	u32 tv_nsec;	/* Nanoseconds.  */
} RGFW_Timespec; /*time struct for fps functions*/

u8 RGFW_isPressedJS(RGFW_window* win, u16 c, u8 button) { return win->src.jsPressed[c][button]; }

#ifdef RGFW_OPENGL
int RGFW_majorVersion = 0, RGFW_minorVersion = 0;

void RGFW_setGLVersion(i32 major, i32 minor) {
	RGFW_majorVersion = major; 
	RGFW_minorVersion = minor;
}

u8* RGFW_getMaxGLVersion() {
    RGFW_window* dummy = RGFW_createWindow("dummy", RGFW_RECT(0, 0, 1, 1), 0);

    const char* versionStr = (const char*)glGetString(GL_VERSION);

    static u8 version[2]; 
    version[0] = versionStr[0] - '0', 
    version[1] = versionStr[2] - '0';

    RGFW_window_close(dummy);

    return version;
}
#endif

#ifdef RGFW_EGL

#if defined(RGFW_LINK_EGL)
typedef EGLBoolean (EGLAPIENTRY * PFN_eglInitialize)(EGLDisplay,EGLint*,EGLint*);

PFNEGLINITIALIZEPROC eglInitializeSource;
PFNEGLGETCONFIGSPROC eglGetConfigsSource;
PFNEGLCHOOSECONFIGPROC eglChooseConfigSource;
PFNEGLCREATEWINDOWSURFACEPROC eglCreateWindowSurfaceSource;
PFNEGLCREATECONTEXTPROC eglCreateContextSource;
PFNEGLMAKECURRENTPROC eglMakeCurrentSource;
PFNEGLGETDISPLAYPROC eglGetDisplaySource;
PFNEGLSWAPBUFFERSPROC eglSwapBuffersSource;
PFNEGLSWAPINTERVALPROC eglSwapIntervalSource;
PFNEGLBINDAPIPROC eglBindAPISource;
PFNEGLDESTROYCONTEXTPROC eglDestroyContextSource;
PFNEGLTERMINATEPROC eglTerminateSource;
PFNEGLDESTROYSURFACEPROC eglDestroySurfaceSource;

#define eglInitialize eglInitializeSource
#define eglGetConfigs eglGetConfigsSource
#define eglChooseConfig eglChooseConfigSource
#define eglCreateWindowSurface eglCreateWindowSurfaceSource
#define eglCreateContext eglCreateContextSource
#define eglMakeCurrent eglMakeCurrentSource
#define eglGetDisplay eglGetDisplaySource
#define eglSwapBuffers eglSwapBuffersSource
#define eglSwapInterval eglSwapIntervalSource
#define eglBindAPI eglBindAPISource
#define eglDestroyContext eglDestroyContextSource
#define eglTerminate eglTerminateSource
#define eglDestroySurface eglDestroySurfaceSource;
#endif


#define EGL_CONTEXT_MAJOR_VERSION_KHR 0x3098
#define EGL_CONTEXT_MINOR_VERSION_KHR 0x30fb

void RGFW_createOpenGLContext(RGFW_window* win) {
	#if defined(RGFW_LINK_EGL)
    eglInitializeSource = (PFNEGLINITIALIZEPROC)eglGetProcAddress("eglInitialize");
    eglGetConfigsSource = (PFNEGLGETCONFIGSPROC)eglGetProcAddress("eglGetConfigs");
    eglChooseConfigSource = (PFNEGLCHOOSECONFIGPROC) eglGetProcAddress("eglChooseConfig");
    eglCreateWindowSurfaceSource = (PFNEGLCREATEWINDOWSURFACEPROC) eglGetProcAddress( "eglCreateWindowSurface");
    eglCreateContextSource = (PFNEGLCREATECONTEXTPROC) eglGetProcAddress("eglCreateContext");
    eglMakeCurrentSource = (PFNEGLMAKECURRENTPROC) eglGetProcAddress("eglMakeCurrent");
    eglGetDisplaySource = (PFNEGLGETDISPLAYPROC) eglGetProcAddress("eglGetDisplay");
	eglSwapBuffersSource = (PFNEGLSWAPBUFFERSPROC)  eglGetProcAddress("eglSwapBuffers");
	eglSwapIntervalSource = (PFNEGLSWAPINTERVALPROC)  eglGetProcAddress("eglSwapInterval");
	eglBindAPISource = (PFNEGLBINDAPIPROC)  eglGetProcAddress("eglBindAPI");
	eglDestroyContextSource = (PFNEGLDESTROYCONTEXTPROC)  eglGetProcAddress("eglDestroyContext");
	eglTerminateSource = (PFNEGLTERMINATEPROC)  eglGetProcAddress("eglTerminate");
	eglDestroySurfaceSource = (PFNEGLDESTROYSURFACEPROC)  eglGetProcAddress("eglDestroySurface");
	#endif /* RGFW_LINK_EGL */

    win->src.EGL_display = eglGetDisplay((EGLNativeDisplayType)win->src.display);

    EGLint major, minor;
	
    eglInitialize(win->src.EGL_display, &major, &minor);

    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,
		#ifdef RGFW_OPENGL_ES
		EGL_OPENGL_ES2_BIT,
		#else
		EGL_OPENGL_BIT,
		#endif
        EGL_NONE
    };

    EGLConfig config;
    EGLint num_configs;
    eglChooseConfig(win->src.EGL_display, config_attribs, &config, 1, &num_configs);

	#ifdef RGFW_OPENGL_ES
    eglBindAPI(EGL_OPENGL_ES_API);
	#else
	eglBindAPI(EGL_OPENGL_API);
	#endif
  
	win->src.rSurf = eglCreateContext(win->src.EGL_display, config, EGL_NO_CONTEXT, NULL);
    win->src.EGL_surface = eglCreateWindowSurface(win->src.EGL_display, config, (EGLNativeWindowType)win->src.window, NULL);

    eglMakeCurrent(win->src.EGL_display, win->src.EGL_surface, win->src.EGL_surface, win->src.rSurf);
	eglSwapBuffers(win->src.EGL_display, win->src.EGL_surface);

	eglSwapInterval(win->src.EGL_display, 1);
}

void* RGFW_getProcAddress(const char* procname) { return (void*)eglGetProcAddress(procname); }

void RGFW_closeEGL(RGFW_window* win) {
    eglDestroySurface(win->src.EGL_display, win->src.EGL_surface);
    eglDestroyContext(win->src.EGL_display, win->src.rSurf);

    eglTerminate(win->src.EGL_display);
}

#endif /* RGFW_EGL */

/*
This is where OS specific stuff starts
*/

#ifdef RGFW_X11
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysymdef.h>
#include <unistd.h>

#include <X11/XKBlib.h> /* for converting keycode to string */
#include <X11/cursorfont.h> /* for hiding */

#include <limits.h> /* for data limits (mainly used in drag and drop functions) */
#include <fcntl.h>

#ifdef __linux__
#include <linux/joystick.h>
#endif

/*atoms needed for drag and drop*/
Atom XdndAware, XdndTypeList,     XdndSelection,    XdndEnter,        XdndPosition,     XdndStatus,       XdndLeave,        XdndDrop,         XdndFinished,     XdndActionCopy,   XdndActionMove,   XdndActionLink,   XdndActionAsk, XdndActionPrivate;

Atom wm_delete_window = 0;

#if !defined(RGFW_NO_X11_CURSOR) && !defined(RGFW_NO_X11_CURSOR_PRELOAD)
typedef XcursorImage* (* PFN_XcursorImageCreate)(int,int);
typedef void (* PFN_XcursorImageDestroy)(XcursorImage*);
typedef Cursor (* PFN_XcursorImageLoadCursor)(Display*,const XcursorImage*);
#endif
#ifdef RGFW_OPENGL
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
#endif

#if !defined(RGFW_NO_X11_CURSOR) && !defined(RGFW_NO_X11_CURSOR_PRELOAD)
PFN_XcursorImageLoadCursor XcursorImageLoadCursorSrc = NULL;
PFN_XcursorImageCreate XcursorImageCreateSrc = NULL;
PFN_XcursorImageDestroy XcursorImageDestroySrc = NULL;

#define XcursorImageLoadCursor XcursorImageLoadCursorSrc
#define XcursorImageCreate XcursorImageCreateSrc
#define XcursorImageDestroy XcursorImageDestroySrc

void* X11Cursorhandle = NULL;
#endif

u32 RGFW_windowsOpen = 0;

#ifdef RGFW_OPENGL
void* RGFW_getProcAddress(const char* procname) { return (void*)glXGetProcAddress((GLubyte*)procname); }
#endif

RGFW_window* RGFW_createWindow(const char* name, RGFW_rect rect, u16 args) {
	#if !defined(RGFW_NO_X11_CURSOR) && !defined(RGFW_NO_X11_CURSOR_PRELOAD)
	if (X11Cursorhandle == NULL) {
		#if defined(__CYGWIN__)
			X11Cursorhandle = dlopen("libXcursor-1.so", RTLD_LAZY | RTLD_LOCAL);
		#elif defined(__OpenBSD__) || defined(__NetBSD__)
			X11Cursorhandle = dlopen("libXcursor.so", RTLD_LAZY | RTLD_LOCAL);
		#else
			X11Cursorhandle = dlopen("libXcursor.so.1", RTLD_LAZY | RTLD_LOCAL);
		#endif

		XcursorImageCreateSrc = (PFN_XcursorImageCreate)dlsym(X11Cursorhandle, "XcursorImageCreate");
		XcursorImageDestroySrc = (PFN_XcursorImageDestroy)dlsym(X11Cursorhandle, "XcursorImageDestroy");
        XcursorImageLoadCursorSrc = (PFN_XcursorImageLoadCursor)dlsym(X11Cursorhandle, "XcursorImageLoadCursor");
	}
	#endif

    XInitThreads(); /* init X11 threading*/

	if (RGFW_OPENGL_SOFTWARE & args)
		setenv("LIBGL_ALWAYS_SOFTWARE", "1", 1);

	RGFW_window* win = RGFW_window_basic_init(rect, args);	

   	u64 event_mask =  KeyPressMask | KeyReleaseMask  | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask | FocusChangeMask; /* X11 events accepted*/

	#ifdef RGFW_OPENGL
	static i32 visual_attribs[] = {   GLX_X_RENDERABLE    , True,   GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,  GLX_RENDER_TYPE     , GLX_RGBA_BIT,   GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,   GLX_RED_SIZE        , 8,   GLX_GREEN_SIZE      , 8,   GLX_BLUE_SIZE       , 8,   GLX_ALPHA_SIZE      , 8,   GLX_DEPTH_SIZE      , 24,   GLX_STENCIL_SIZE    , 8,   GLX_DOUBLEBUFFER    , True,    None   };
	
	i32 fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig((Display*)win->src.display, DefaultScreen(win->src.display), visual_attribs, &fbcount);

	i32 best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

	if (fbcount == 0) {
		printf("Failed to find any valid GLX configs\n");
		return NULL;
	}

	i32 i;
	for (i = 0; i < fbcount; i++) {
		XVisualInfo *vi = glXGetVisualFromFBConfig((Display*)win->src.display, fbc[i]);
		if (vi) {
			i32 samp_buf, samples;
			glXGetFBConfigAttrib((Display*)win->src.display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
			glXGetFBConfigAttrib((Display*)win->src.display, fbc[i], GLX_SAMPLES, &samples );

			if ( (best_fbc < 0 || samp_buf) && (samples > best_num_samp) )
				best_fbc = i, best_num_samp = samples;
			if ( (worst_fbc < 0 || !samp_buf) || (samples < worst_num_samp) )
				worst_fbc = i, worst_num_samp = samples;
		}
		XFree(vi);
	}

	if (best_fbc == -1) { 
		printf("Failed to get a valid GLX visual\n");
		return NULL;
	}

	GLXFBConfig bestFbc = fbc[best_fbc];
	
	/* Get a visual */
	XVisualInfo* vi = glXGetVisualFromFBConfig((Display*)win->src.display, bestFbc);
	
	XFree(fbc);

	u32 valuemask = CWBorderPixel|CWColormap;
	#else
    XVisualInfo* vi = (XVisualInfo*)malloc(sizeof(XVisualInfo));
	vi->screen = DefaultScreen((Display*)win->src.display);
	vi->visual = DefaultVisual((Display*)win->src.display, vi->screen);

	vi->depth = 0;
	u32 valuemask = 0;
	#endif
	
	/* make X window attrubutes*/
	XSetWindowAttributes swa;
	Colormap cmap;

	swa.colormap = cmap = XCreateColormap( (Display*)win->src.display,
											RootWindow( win->src.display, vi->screen ), 
											vi->visual, AllocNone );	

	swa.background_pixmap = None ;	
	swa.border_pixel = 0;
	swa.event_mask = event_mask;

	/* create the window*/
	win->src.window = XCreateWindow((Display *)win->src.display, RootWindow((Display *)win->src.display, vi->screen), win->r.x, win->r.y, win->r.w, win->r.h,
						0, vi->depth, InputOutput, vi->visual,
						valuemask|CWEventMask, &swa);


	XFreeColors((Display*)win->src.display, cmap, NULL, 0, 0);
	if (RGFW_TRANSPARENT_WINDOW & args)
		XMatchVisualInfo((Display *)win->src.display, DefaultScreen((Display *)win->src.display), 32, TrueColor, vi); /* for RGBA backgrounds*/

	XFree(vi);

	#ifdef RGFW_OPENGL
	i32 context_attribs[7] = {0, 0, 0, 0, 0, 0, 0};
	context_attribs[0] = GLX_CONTEXT_PROFILE_MASK_ARB;
	context_attribs[1] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;

	if (RGFW_majorVersion || RGFW_minorVersion) {
		context_attribs[2] = GLX_CONTEXT_MAJOR_VERSION_ARB;
		context_attribs[3] = RGFW_majorVersion;
		context_attribs[4] = GLX_CONTEXT_MINOR_VERSION_ARB;
		context_attribs[5] = RGFW_minorVersion;
	}

	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
	glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB" );

	GLXContext ctx = NULL; 
	
	if (RGFW_root != NULL)
		ctx = RGFW_root->src.rSurf;
	
	win->src.rSurf = glXCreateContextAttribsARB((Display*)win->src.display, bestFbc, ctx, True, context_attribs);
	#endif

	#ifdef RGFW_EGL
	RGFW_createOpenGLContext(win);
	#endif

	RGFW_init_buffer(win);
	
    if (RGFW_NO_RESIZE & args) { /* make it so the user can't resize the window*/
        XSizeHints* sh = XAllocSizeHints();
        sh->flags = (1L << 4) | (1L << 5);
        sh->min_width = sh->max_width = win->r.w;
        sh->min_height = sh->max_height = win->r.h;

        XSetWMSizeHints((Display *)win->src.display, (Drawable)win->src.window, sh, XA_WM_NORMAL_HINTS);
		XFree(sh);
    }

    if (RGFW_NO_BORDER & args) {
		/* Atom vars for no-border*/
        static Atom window_type = 0;
        static Atom value = 0;
		
		if (window_type == 0) {
			window_type = XInternAtom((Display *)win->src.display, "_NET_WM_WINDOW_TYPE", False);
			value = XInternAtom((Display *)win->src.display, "_NET_WM_WINDOW_TYPE_DOCK", False);
		}

        XChangeProperty((Display *)win->src.display, (Drawable)win->src.window, window_type, XA_ATOM, 32, PropModeReplace, (u8 *)&value, 1); /* toggle border*/
    }

    XSelectInput((Display *)win->src.display, (Drawable)win->src.window, event_mask); /* tell X11 what events we want*/

    /* make it so the user can't close the window until the program does*/
	if (wm_delete_window == 0)
		wm_delete_window = XInternAtom((Display *)win->src.display, "WM_DELETE_WINDOW", 1);

    XSetWMProtocols((Display *)win->src.display, (Drawable)win->src.window, &wm_delete_window, 1);
	
    /* connect the context to the window*/
    #ifdef RGFW_OPENGL
	glXMakeCurrent((Display *)win->src.display, (Drawable)win->src.window, (GLXContext)win->src.rSurf);
	#endif

    /* set the background*/
    XStoreName((Display *)win->src.display, (Drawable) win->src.window, name); /* set the name*/

    XMapWindow((Display *)win->src.display, (Drawable)win->src.window);						  /* draw the window*/
    XMoveWindow((Display *)win->src.display, (Drawable)win->src.window, win->r.x, win->r.y); /* move the window to it's proper cords*/

	if (RGFW_ALLOW_DND & args) { /* init drag and drop atoms and turn on drag and drop for this window */
		win->src.winArgs |= RGFW_ALLOW_DND;

		XdndAware         = XInternAtom((Display*)win->src.display, "XdndAware",         False);
		XdndTypeList      = XInternAtom((Display*)win->src.display, "XdndTypeList",      False);
		XdndSelection     = XInternAtom((Display*)win->src.display, "XdndSelection",     False);

		/* client messages */
		XdndEnter         = XInternAtom((Display*)win->src.display, "XdndEnter",         False);
		XdndPosition      = XInternAtom((Display*)win->src.display, "XdndPosition",      False);
		XdndStatus        = XInternAtom((Display*)win->src.display, "XdndStatus",        False);
		XdndLeave         = XInternAtom((Display*)win->src.display, "XdndLeave",         False);
		XdndDrop          = XInternAtom((Display*)win->src.display, "XdndDrop",          False);
		XdndFinished      = XInternAtom((Display*)win->src.display, "XdndFinished",      False);

		/* actions */
		XdndActionCopy    = XInternAtom((Display*)win->src.display, "XdndActionCopy",    False);
		XdndActionMove    = XInternAtom((Display*)win->src.display, "XdndActionMove",    False);
		XdndActionLink    = XInternAtom((Display*)win->src.display, "XdndActionLink",    False);
		XdndActionAsk     = XInternAtom((Display*)win->src.display, "XdndActionAsk",     False);
		XdndActionPrivate = XInternAtom((Display*)win->src.display, "XdndActionPrivate", False);
		const Atom version = 5;

		XChangeProperty((Display*)win->src.display, (Window)win->src.window,
                        XdndAware, 4, 32,
                        PropModeReplace, (u8*) &version, 1); /* turns on drag and drop */
	}

	RGFW_window_setMouseDefault(win);

	if (RGFW_HIDE_MOUSE & args)
		RGFW_window_showMouse(win, 0);

	RGFW_windowsOpen++;

	if (RGFW_root == NULL)
		RGFW_root = win;
		
    return win; /*return newly created window*/
}

RGFW_area RGFW_getScreenSize(void) {
	assert(RGFW_root != NULL);

	Screen* scrn = DefaultScreenOfDisplay((Display*)RGFW_root->src.display);
	return RGFW_AREA(scrn->width, scrn->height);
}

RGFW_vector RGFW_getGlobalMousePoint(void) {
	assert(RGFW_root != NULL);
	
	RGFW_vector RGFWMouse;

	i32 x, y;
	u32 z;
	Window window1, window2;
    XQueryPointer((Display*)RGFW_root->src.display, XDefaultRootWindow((Display*)RGFW_root->src.display), &window1, &window2, &x, &RGFWMouse.x, &RGFWMouse.y, &y, &z);

	return RGFWMouse;
}

typedef struct XDND {
	long source, version;
	i32 format;
} XDND; /* data structure for xdnd events */
XDND xdnd;

int xAxis = 0, yAxis = 0;

RGFW_Event* RGFW_window_checkEvent(RGFW_window* win) {
	assert(win != NULL);
	
	win->event.type = 0;
	
	XEvent E; /* raw X11 event */

	/* if there is no unread qued events, get a new one */
	if (XEventsQueued((Display*)win->src.display, QueuedAlready) + XEventsQueued((Display*)win->src.display, QueuedAfterReading))
		XNextEvent((Display*)win->src.display, &E);
	else {
		return NULL;
	}

	u32 i;

	if (win->event.droppedFilesCount) {
		for (i = 0; i < win->event.droppedFilesCount; i++)
			win->event.droppedFiles[i][0] = '\0';
	}

	win->event.droppedFilesCount = 0;
	win->event.type = 0;

	switch (E.type) {
		case KeyPress:
		case KeyRelease:
			/* check if it's a real key release */
			if (E.type == KeyRelease && XEventsQueued((Display *)win->src.display, QueuedAfterReading)) { /* get next event if there is one*/
				XEvent NE;
				XPeekEvent((Display *)win->src.display, &NE);

				if (E.xkey.time == NE.xkey.time && E.xkey.keycode == NE.xkey.keycode) /* check if the current and next are both the same*/
					break;
			}

			/* set event key data */
			win->event.keyCode = XkbKeycodeToKeysym((Display *)win->src.display, E.xkey.keycode, 0, E.xkey.state & ShiftMask ? 1 : 0);
			win->event.keyName = XKeysymToString(win->event.keyCode); /* convert to string */

			/* get keystate data */
			win->event.type = (E.type == KeyPress) ? RGFW_keyPressed : RGFW_keyReleased;

			if (win->event.type == RGFW_keyReleased) {
				if (win->event.keyCode == XK_Caps_Lock && win->event.lockState & RGFW_CAPSLOCK)
					win->event.lockState ^= RGFW_CAPSLOCK;
				else if (win->event.keyCode == XK_Caps_Lock)
					win->event.lockState |= RGFW_CAPSLOCK;

				else if (win->event.keyCode == XK_Num_Lock && win->event.lockState & RGFW_NUMLOCK)
					win->event.lockState ^= RGFW_NUMLOCK;
				else if (win->event.keyCode == XK_Num_Lock)
					win->event.lockState |= RGFW_NUMLOCK;
			}
			break;

		case ButtonPress:
		case ButtonRelease:
			win->event.type = (E.type == ButtonPress) ? RGFW_mouseButtonPressed : RGFW_mouseButtonReleased;
			win->event.button = E.xbutton.button;
			break;

		case MotionNotify:
			win->event.point.x = E.xmotion.x;
			win->event.point.y = E.xmotion.y;
			win->event.type = RGFW_mousePosChanged;
			break;

		case ClientMessage:
			/* if the client closed the window*/
			if (E.xclient.data.l[0] == (i64)wm_delete_window)
				win->event.type = RGFW_quit;

			/*
				much of this event (drag and drop code) is source from glfw
			*/

			if ((win->src.winArgs & RGFW_ALLOW_DND) == 0)
				break;
			
			u8 formFree = 0;
			if (E.xclient.message_type == XdndEnter) {
				u64 count;
				Atom* formats = (Atom*)0;
				Bool list = E.xclient.data.l[1] & 1;

				xdnd.source  = E.xclient.data.l[0];
				xdnd.version = E.xclient.data.l[1] >> 24;
				xdnd.format  = None;

				if (xdnd.version > 5)
					break;

				if (list) {
					Atom actualType;
					i32 actualFormat;
					u64 bytesAfter;

					XGetWindowProperty((Display*)win->src.display,
									xdnd.source,
									XdndTypeList,
									0,
									LONG_MAX,
									False,
									4,
									&actualType,
									&actualFormat,
									&count,
									&bytesAfter,
									(u8**) &formats);
				}
				else {
					formats = (Atom*)RGFW_MALLOC(E.xclient.data.l[2] + E.xclient.data.l[3] + E.xclient.data.l[4]);
					formFree = 1;

					count = 0;

					if (E.xclient.data.l[2] != None)
						formats[count++] = E.xclient.data.l[2];
					if (E.xclient.data.l[3] != None)
						formats[count++] = E.xclient.data.l[3];
					if (E.xclient.data.l[4] != None)
						formats[count++] = E.xclient.data.l[4];
				}

				u32 i;
				for (i = 0;  i < count;  i++) {
					char* name = XGetAtomName((Display*)win->src.display, formats[i]);
					
					char* links[2] = {(char*)(const char*)"text/uri-list", (char*)(const char*)"text/plain"};
					for (; 1; name++) {
						u32 j;
						for (j = 0; j < 2; j++) {
							if (*links[j] != *name) {
								links[j] = (char*)(const char*)"\1";
								continue;
							}
							
							if (*links[j] == '\0' && *name == '\0')
								xdnd.format = formats[i];

							if (*links[j] != '\0' && *links[j] != '\1') 
								links[j]++;
						}

						if (*name == '\0')
							break;
					}
				}

				if (list && formats) {
					XFree(formats);
					formats = (Atom*)0;
				}
				else if (formFree && formats != (Atom*)0) {
					RGFW_FREE(formats);

					formats = (Atom*)0;
					formFree = 1;
				}

				break;
			}
			else if (E.xclient.message_type ==  XdndPosition) {
				const i32 xabs = (E.xclient.data.l[2] >> 16) & 0xffff;
				const i32 yabs = (E.xclient.data.l[2]) & 0xffff;
				Window dummy;
				i32 xpos, ypos;

				if (xdnd.version > 5)
					break;

				XTranslateCoordinates((Display*)win->src.display,
									XDefaultRootWindow((Display*)win->src.display),
									(Window)win->src.window,
									xabs, yabs,
									&xpos, &ypos,
									&dummy);

				win->event.point.x = xpos;
				win->event.point.y = ypos;

				XEvent reply = { ClientMessage };
				reply.xclient.window = xdnd.source;
				reply.xclient.message_type = XdndStatus;
				reply.xclient.format = 32;
				reply.xclient.data.l[0] = (long)win->src.window;
				reply.xclient.data.l[2] = 0;
				reply.xclient.data.l[3] = 0;

				if (xdnd.format) {
					reply.xclient.data.l[1] = 1;
					if (xdnd.version >= 2)
						reply.xclient.data.l[4] = XdndActionCopy;
				}

				XSendEvent((Display*)win->src.display, xdnd.source, False, NoEventMask, &reply);
				XFlush((Display*)win->src.display);
			}
			else if (E.xclient.message_type == XdndDrop) {
				if (xdnd.version > 5)
					break;
				
				win->event.type = RGFW_dnd_init;

				if (xdnd.format) {
					Time time = CurrentTime;

					if (xdnd.version >= 1)
						time = E.xclient.data.l[2];

					XConvertSelection((Display*)win->src.display,
									XdndSelection,
									xdnd.format,
									XdndSelection,
									(Window)win->src.window,
									time);
				}
				else if (xdnd.version >= 2) {
					XEvent reply = { ClientMessage };
					reply.xclient.window = xdnd.source;
					reply.xclient.message_type = XdndFinished;
					reply.xclient.format = 32;
					reply.xclient.data.l[0] = (long)win->src.window;
					reply.xclient.data.l[1] = 0;
					reply.xclient.data.l[2] = None;

					XSendEvent((Display*)win->src.display, xdnd.source,
							False, NoEventMask, &reply);
					XFlush((Display*)win->src.display);
				}
			}
			break;
        case SelectionNotify:
			/* this is only for checking for xdnd drops */
			if (E.xselection.property != XdndSelection || !(win->src.winArgs | RGFW_ALLOW_DND))
				break;

			char* data;
			u64 result;

			Atom actualType;
			i32 actualFormat;
			u64 bytesAfter;

			XGetWindowProperty((Display*)win->src.display, E.xselection.requestor, E.xselection.property, 0, LONG_MAX, False, E.xselection.target, &actualType, &actualFormat, &result, &bytesAfter, (u8**) &data);

			if (result)
				break;
			
			/*
			SOURCED FROM GLFW _glfwParseUriList
			Copyright (c) 2002-2006 Marcus Geelnard
			Copyright (c) 2006-2019 Camilla Löwy
			*/

			const char* prefix = "file://";
			
			char* line;

			win->event.droppedFilesCount = 0;

			win->event.type = RGFW_dnd;

			while ((line = strtok(data, "\r\n"))) {
				char path[RGFW_MAX_PATH];

				data = NULL;

				if (line[0] == '#')
					continue;

				char* l;						
				for (l = line; 1; l++) {
					if ((l - line) > 7)
						break;
					else if (*l != prefix[(l - line)])
						break;
					else if (*l == '\0' && prefix[(l - line)] == '\0') {
						line += 7;
						while (*line != '/')
							line++;
						break;
					}
					else if (*l == '\0')
						break; 
				}

				win->event.droppedFilesCount++;

				size_t index = 0; 
				while (*line) {
					if (line[0] == '%' && line[1] && line[2]) {
						const char digits[3] = { line[1], line[2], '\0' };
						path[index] = (char) strtol(digits, NULL, 16);
						line += 2;
					}
					else
						path[index] = *line;

					index++;
					line++;
				}
				
				strcpy(win->event.droppedFiles[win->event.droppedFilesCount - 1], path);
			}

			if (data)
				XFree(data);

			if (xdnd.version >= 2) {
				XEvent reply = { ClientMessage };
				reply.xclient.window = xdnd.source;
				reply.xclient.message_type = XdndFinished;
				reply.xclient.format = 32;
				reply.xclient.data.l[0] = (long)win->src.display;
				reply.xclient.data.l[1] = result;
				reply.xclient.data.l[2] = XdndActionCopy;

				XSendEvent((Display*)win->src.display, xdnd.source, False, NoEventMask, &reply);
				XFlush((Display*)win->src.display);
			}

			break;

		case FocusIn:
			win->event.inFocus = 1;
			
			XKeyboardState keystate;
			XGetKeyboardControl((Display *)win->src.display, &keystate);
			win->event.lockState = keystate.led_mask;

			break;
		case FocusOut:
			win->event.inFocus = 0;
			RGFW_window_setMouseDefault(win);
			break;
		case ConfigureNotify: {
			#ifndef RGFW_NO_X11_WINDOW_ATTRIB
			XWindowAttributes a;
			XGetWindowAttributes((Display *)win->src.display, (Window)win->src.window, &a);
			win->r = RGFW_RECT(E.xconfigure.x, E.xconfigure.y, E.xconfigure.width, E.xconfigure.height);
			#endif

			win->event.type = RGFW_windowAttribsChange;
			break;
		}
		default: {
			#ifdef __linux__
			u8 i;
			for (i = 0; i < win->src.joystickCount; i++) {
				struct js_event e;

				if (!win->src.joysticks[i])
					continue;

				i32 flags = fcntl(win->src.joysticks[i], F_GETFL, 0);
				fcntl(win->src.joysticks[i], F_SETFL, flags | O_NONBLOCK);

				ssize_t bytes;
				while ((bytes = read(win->src.joysticks[i], &e, sizeof(e))) > 0) {
						switch (e.type) {
							case JS_EVENT_BUTTON:
								win->event.type = e.value ? RGFW_jsButtonPressed : RGFW_jsButtonReleased;
								win->event.button = e.number;

								win->src.jsPressed[i][e.number] = e.value;
								break;
							case JS_EVENT_AXIS:
								ioctl(win->src.joysticks[i], JSIOCGAXES, &win->event.axisesCount);

								if ((e.number == 0 || e.number % 2) && e.number != 1)
									xAxis = e.value;
								else
									yAxis = e.value;

								win->event.axis[e.number / 2].x = xAxis / 327.67;
								win->event.axis[e.number / 2].y = yAxis / 327.67;
								win->event.type = RGFW_jsAxisMove;
								win->event.joystick = e.number / 2;
								break;

							default: break;
						}
				}
			}
			#endif

			break;
		}
	}

	if (win->event.inFocus && (win->src.winArgs & RGFW_MOUSE_CHANGED)) {
		XDefineCursor((Display*)win->src.display, (Window)win->src.window, (Cursor)win->src.cursor);

		win->src.winArgs &= ~RGFW_MOUSE_CHANGED;
	}

	XFlush((Display*)win->src.display);

	if (win->event.type)
		return &win->event;
	else
		return NULL;
}

void RGFW_window_close(RGFW_window* win) {
	assert(win != NULL);
	
	#ifdef RGFW_VULKAN
    for (int i = 0; i < win->src.image_count; i++) {
        vkDestroyImageView(RGFW_vulkan_info.device, win->src.swapchain_image_views[i], NULL);
    }

    vkDestroySwapchainKHR(RGFW_vulkan_info.device, win->src.swapchain, NULL);
    vkDestroySurfaceKHR(RGFW_vulkan_info.instance, win->src.rSurf, NULL);
    free(win->src.swapchain_image_views);
    free(win->src.swapchain_images);
	#endif

	#ifdef RGFW_EGL
	RGFW_closeEGL(win);
	#endif

	XFreeCursor((Display*)win->src.display, (Cursor)win->src.cursor);

	#if defined(RGFW_OSMESA) || defined(RGFW_BUFFER)
	if (win->buffer != NULL) {
		XDestroyImage((XImage*)win->src.bitmap);
	}
	#endif

	if ((Display*)win->src.display) {
		#ifdef RGFW_OPENGL
		glXDestroyContext((Display *)win->src.display, win->src.rSurf);
		#endif

		if (win == RGFW_root)
			RGFW_root = NULL;
		
		if ((Drawable)win->src.window)
			XDestroyWindow((Display *)win->src.display, (Drawable)win->src.window); /* close the window*/

		if (win->src.display)
			XCloseDisplay((Display *)win->src.display); /* kill the display*/
	}

	#ifdef RGFW_ALLOC_DROPFILES
	{
		i32 i;
		for (i = 0; i < RGFW_MAX_DROPS; i++)
			RGFW_FREE(win->event.droppedFiles[i]);
    
	
		RGFW_FREE(win->event.droppedFiles);
	}
	#endif

	RGFW_windowsOpen--;
	#if !defined(RGFW_NO_X11_CURSOR_PRELOAD) && !defined(RGFW_NO_X11_CURSOR)
	if (X11Cursorhandle != NULL && RGFW_windowsOpen <= 0) {
		dlclose(X11Cursorhandle);

		X11Cursorhandle = NULL;
	}
	#endif

	/* set cleared display / window to NULL for error checking */
	win->src.display = (Display*)0;
	win->src.window = (Window)0;

	u8 i;
	for (i = 0; i < win->src.joystickCount; i++)
		close(win->src.joysticks[i]);

	RGFW_FREE(win); /* free collected window data */
}

void RGFW_window_move(RGFW_window* win, RGFW_vector v) {
	assert(win != NULL);
	win->r.x = v.x;
	win->r.y = v.y;

	XMoveWindow((Display *)win->src.display, (Window)win->src.window, v.x, v.y);
}


void RGFW_window_resize(RGFW_window* win, RGFW_area a) {
	assert(win != NULL);
	win->r.w = a.w;
	win->r.h = a.h;

	XResizeWindow((Display *)win->src.display, (Window)win->src.window, a.w, a.h);
}

void RGFW_window_setMinSize(RGFW_window* win, RGFW_area a) {
	assert(win != NULL);
	
    XSizeHints hints;
    long flags;

    XGetWMNormalHints(win->src.display, (Window)win->src.window, &hints, &flags);

    hints.flags |= PMinSize;

    hints.min_width = a.w;
    hints.min_height = a.h;

    XSetWMNormalHints(win->src.display, (Window)win->src.window, &hints);
}

void RGFW_window_setMaxSize(RGFW_window* win, RGFW_area a) {
	assert(win != NULL);
	
    XSizeHints hints;
    long flags;

    XGetWMNormalHints(win->src.display, (Window)win->src.window, &hints, &flags);

    hints.flags |= PMaxSize;

    hints.max_width = a.w;
    hints.max_height = a.h;

    XSetWMNormalHints(win->src.display, (Window)win->src.window, &hints);
}


void RGFW_window_minimize(RGFW_window* win) {
	assert(win != NULL);
	
    XIconifyWindow(win->src.display, (Window)win->src.window, DefaultScreen(win->src.display));
    XFlush(win->src.display);
}

void RGFW_window_restore(RGFW_window* win) {
	assert(win != NULL);
	
    XMapWindow(win->src.display, (Window)win->src.window);
    XFlush(win->src.display);
}

void RGFW_window_setName(RGFW_window* win, char* name) {
	assert(win != NULL);
	
	XStoreName((Display*)win->src.display, (Window)win->src.window, name);
}

/*
	the majority function is sourced from GLFW
*/

void RGFW_window_setIcon(RGFW_window* win, u8* icon, RGFW_area a, i32 channels) {
	assert(win != NULL);
	
	i32 longCount = 2 + a.w * a.h;

    u64* X11Icon = (u64*)RGFW_MALLOC(longCount * sizeof(u64));
    u64* target = X11Icon;

    *target++ = a.w;
    *target++ = a.h;

	i32 i;

    for (i = 0;  i < a.w * a.h;  i++) {
        if (channels == 3)
            *target++ = ((icon[i * 3 + 0]) << 16) |
                        ((icon[i * 3 + 1]) <<  8) |
                        ((icon[i * 3 + 2]) <<  0) |
                        (0xFF << 24);

        else if (channels == 4)
            *target++ = ((icon[i * 4 + 0]) << 16) |
                        ((icon[i * 4 + 1]) <<  8) |
                        ((icon[i * 4 + 2]) <<  0) |
                        ((icon[i * 4 + 3]) << 24);
    }
	
	static Atom NET_WM_ICON = 0;
	if (NET_WM_ICON == 0) 
    	NET_WM_ICON = XInternAtom((Display*)win->src.display, "_NET_WM_ICON", False);

    XChangeProperty((Display*)win->src.display, (Window)win->src.window,
                    NET_WM_ICON,
                    6, 32,
                    PropModeReplace,
                    (u8*) X11Icon,
                    longCount);

    RGFW_FREE(X11Icon);

    XFlush((Display*)win->src.display);
}

void RGFW_window_setMouse(RGFW_window* win, u8* image, RGFW_area a, i32 channels) {
	assert(win != NULL);
	
	#ifndef RGFW_NO_X11_CURSOR
	/* free the previous cursor */
	if (win->src.cursor && win->src.cursor != -1)
		XFreeCursor((Display*)win->src.display, (Cursor)win->src.cursor);

	XcursorImage* native = XcursorImageCreate(a.w, a.h);
    native->xhot = 0;
    native->yhot = 0;

    u8* source = (u8*)image;
    XcursorPixel* target = native->pixels;

	i32 i;
    for (i = 0;  i < a.w * a.h;  i++, target++, source += 4) {
        u8 alpha = 0xFF;
        if (channels == 4)
            alpha = source[3];

        *target = (alpha << 24) | (((source[0] * alpha) / 255) << 16) | (((source[1] * alpha) / 255) <<  8) | (((source[2] * alpha) / 255) <<  0);
    }

	win->src.winArgs |= RGFW_MOUSE_CHANGED;
    win->src.cursor = XcursorImageLoadCursor((Display*)win->src.display, native);
		
	XcursorImageDestroy(native);
	#endif
}

void RGFW_window_moveMouse(RGFW_window* win, RGFW_vector v) {
	assert(win != NULL);
	
    Window root = RootWindow(win->src.display, DefaultScreen(win->src.display));

    XWarpPointer(win->src.display, None, root, 0, 0, 0, 0, v.x, v.y);
}

void RGFW_window_setMouseDefault(RGFW_window* win) {
	assert(win != NULL);
	
	/* free the previous cursor */
	if (win->src.cursor && win->src.cursor != -1)
		XFreeCursor((Display*)win->src.display, (Cursor)win->src.cursor);

	win->src.winArgs |= RGFW_MOUSE_CHANGED;
	win->src.cursor = XCreateFontCursor((Display*)win->src.display, XC_left_ptr);
}

/*
	the majority function is sourced from GLFW
*/
char* RGFW_readClipboard(size_t* size) {
	char* result = NULL;
	u64 ressize, restail;
	i32 resbits;
	static Atom bufid = 0, fmtid, propid, incrid;

	if (bufid == 0) {
		bufid = XInternAtom((Display*)RGFW_root->src.display, "CLIPBOARD", False),
		fmtid = XInternAtom((Display*)RGFW_root->src.display, "STRING", False),
		propid = XInternAtom((Display*)RGFW_root->src.display, "XSEL_DATA", False),
		incrid = XInternAtom((Display*)RGFW_root->src.display, "INCR", False);
	}

	XEvent event;

	XSelectInput ((Display*)RGFW_root->src.display, (Window)RGFW_root->src.window, PropertyChangeMask);
	XConvertSelection((Display*)RGFW_root->src.display, bufid, fmtid, propid, (Window)RGFW_root->src.window, CurrentTime);
	do {
		XNextEvent((Display*)RGFW_root->src.display, &event);
	} while (event.type != SelectionNotify || event.xselection.selection != bufid);

	if (event.xselection.property == 0)
		return result;
	
	XGetWindowProperty((Display*)RGFW_root->src.display, (Window)RGFW_root->src.window, propid, 0, LONG_MAX/4, True, AnyPropertyType,
		&fmtid, &resbits, &ressize, &restail, (u8**)&result);

	if (fmtid != incrid)
		return result;

	if (size != NULL)
		*size = 0;

	do {
		while (event.type != PropertyNotify || event.xproperty.atom != propid || event.xproperty.state != PropertyNewValue) 
			XNextEvent((Display*)RGFW_root->src.display, &event);

		XGetWindowProperty((Display*)RGFW_root->src.display, (Window)RGFW_root->src.window, propid, 0, LONG_MAX/4, True, AnyPropertyType, 
										&fmtid, &resbits, (size != NULL) ? size : & ressize, &restail, (u8**)&result);
	} while (ressize > 0 || (size != NULL && *size));

	return result;
}

/*
	almost all of this function is sourced from GLFW
*/
void RGFW_writeClipboard(const char* text, u32 textLen) {
    static Atom CLIPBOARD = 0, 
				UTF8_STRING = 0, 
				SAVE_TARGETS = 0, 
				TARGETS = 0, 
				MULTIPLE = 0, 
				ATOM_PAIR = 0, 
				CLIPBOARD_MANAGER = 0;

	if (CLIPBOARD == 0) {
		CLIPBOARD = XInternAtom((Display*)RGFW_root->src.display, "CLIPBOARD", False);
		UTF8_STRING = XInternAtom((Display*)RGFW_root->src.display, "UTF8_STRING", False);
		SAVE_TARGETS = XInternAtom((Display*)RGFW_root->src.display, "SAVE_TARGETS", False);
		TARGETS = XInternAtom((Display*)RGFW_root->src.display, "TARGETS", False);
		MULTIPLE = XInternAtom((Display*)RGFW_root->src.display, "MULTIPLE", False);
		ATOM_PAIR = XInternAtom((Display*)RGFW_root->src.display, "ATOM_PAIR", False);
		CLIPBOARD_MANAGER = XInternAtom((Display*)RGFW_root->src.display, "CLIPBOARD_MANAGER", False);
	}

    XSetSelectionOwner((Display*)RGFW_root->src.display, CLIPBOARD, (Window)RGFW_root->src.window, CurrentTime);

    XConvertSelection((Display*)RGFW_root->src.display, CLIPBOARD_MANAGER, SAVE_TARGETS, None, (Window)RGFW_root->src.window, CurrentTime);

    for (;;) {
        XEvent event;

        XNextEvent((Display*)RGFW_root->src.display, &event);
        if (event.type != SelectionRequest)
			return; 
		
		const XSelectionRequestEvent* request = &event.xselectionrequest;

		XEvent reply = { SelectionNotify };

		char* selectionString = NULL;
		const Atom formats[] = { UTF8_STRING, XA_STRING };
		const i32 formatCount = sizeof(formats) / sizeof(formats[0]);

		selectionString = (char*)text;

		if (request->target == TARGETS) {
			const Atom targets[] = { TARGETS,
									MULTIPLE,
									UTF8_STRING,
									XA_STRING };

			XChangeProperty((Display*)RGFW_root->src.display,
							request->requestor,
							request->property,
							4,
							32,
							PropModeReplace,
							(u8*) targets,
							sizeof(targets) / sizeof(targets[0]));

			reply.xselection.property = request->property;
		}

		if (request->target == MULTIPLE) {

			Atom* targets;

			Atom actualType;
			i32 actualFormat;
			u64 count, bytesAfter;

			XGetWindowProperty((Display*)RGFW_root->src.display, request->requestor, request->property, 0, LONG_MAX, False, ATOM_PAIR,  &actualType, &actualFormat, &count, &bytesAfter, (u8**) &targets);

			u64 i;
			for (i = 0;  i < count;  i += 2) {
				i32 j;

				for (j = 0;  j < formatCount;  j++) {
					if (targets[i] == formats[j])
						break;
				}

				if (j < formatCount)
				{
					XChangeProperty((Display*)RGFW_root->src.display,
									request->requestor,
									targets[i + 1],
									targets[i],
									8,
									PropModeReplace,
									(u8 *) selectionString,
									textLen);
				}
				else
					targets[i + 1] = None;
			}

			XChangeProperty((Display*)RGFW_root->src.display,
							request->requestor,
							request->property,
							ATOM_PAIR,
							32,
							PropModeReplace,
							(u8*) targets,
							count);

			XFree(targets);

			reply.xselection.property = request->property;
		}

		reply.xselection.display = request->display;
		reply.xselection.requestor = request->requestor;
		reply.xselection.selection = request->selection;
		reply.xselection.target = request->target;
		reply.xselection.time = request->time;

		XSendEvent((Display*)RGFW_root->src.display, request->requestor, False, 0, &reply);
	}
}

u16 RGFW_registerJoystick(RGFW_window* win, i32 jsNumber) {
	assert(win != NULL);
	
	#ifdef __linux__
	char file[15];
	sprintf(file, "/dev/input/js%i", jsNumber);

	return RGFW_registerJoystickF(win, file);
	#endif
}

u16 RGFW_registerJoystickF(RGFW_window* win, char* file) {
	assert(win != NULL);
	
	#ifdef __linux__

	i32 js = open(file, O_RDONLY);

	if (js && win->src.joystickCount < 4) {
		win->src.joystickCount++;

		win->src.joysticks[win->src.joystickCount - 1] = open(file, O_RDONLY);

		u8 i;
		for (i = 0; i < 16; i++)
			win->src.jsPressed[win->src.joystickCount - 1][i] = 0;

	}

	else {
		#ifdef RGFW_PRINT_ERRORS
		RGFW_error = 1;
		fprintf(stderr, "Error RGFW_registerJoystickF : Cannot open file %s\n", file);
		#endif
	}

	return win->src.joystickCount - 1;
	#endif
}

u8 RGFW_window_isFullscreen(RGFW_window* win) {
	assert(win != NULL);
	
	XWindowAttributes windowAttributes;
    XGetWindowAttributes(win->src.display, (Window)win->src.window, &windowAttributes);
	
	/* check if the window is visable */
    if (windowAttributes.map_state != IsViewable) 
        return 0;

    /* check if the window covers the full screen */
    return (windowAttributes.x == 0 && windowAttributes.y == 0 &&
            windowAttributes.width == XDisplayWidth(win->src.display, DefaultScreen(win->src.display)) &&
            windowAttributes.height == XDisplayHeight(win->src.display, DefaultScreen(win->src.display)));
}

u8 RGFW_window_isHidden(RGFW_window* win) {
	assert(win != NULL);
	
    XWindowAttributes windowAttributes;
    XGetWindowAttributes(win->src.display, (Window)win->src.window, &windowAttributes);

    return (windowAttributes.map_state == IsUnmapped && !RGFW_isMinimized(win));
}

u8 RGFW_isMinimized(RGFW_window* win) {
	assert(win != NULL);
	
    static Atom prop = 0;
	if (prop == 0)
		prop = XInternAtom(win->src.display, "WM_STATE", False);

    Atom actual_type;
    i32 actual_format;
    u64 nitems, bytes_after;
    unsigned char *prop_data;

    i16 status = XGetWindowProperty(win->src.display, (Window)win->src.window, prop, 0, 2, False,
                                     AnyPropertyType, &actual_type, &actual_format,
                                     &nitems, &bytes_after, &prop_data);

    if (status == Success && nitems >= 1 && *((int *)prop_data) == IconicState) {
		XFree(prop_data);
		return 1;
    }

    if (prop_data != NULL)
        XFree(prop_data);

    return 0;
}

u8 RGFW_isMaximized(RGFW_window* win) {
	assert(win != NULL);
	
    static Atom net_wm_state = 0;
    static Atom net_wm_state_maximized_horz = 0;
    static Atom net_wm_state_maximized_vert = 0;

	if (net_wm_state == 0) {
		net_wm_state = XInternAtom(win->src.display, "_NET_WM_STATE", False);
		net_wm_state_maximized_vert =  XInternAtom(win->src.display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
		net_wm_state_maximized_horz = XInternAtom(win->src.display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
	}

    Atom actual_type;
    i32 actual_format;
    u64 nitems, bytes_after;
    unsigned char *prop_data;

    i16 status = XGetWindowProperty(win->src.display, (Window)win->src.window, net_wm_state, 0, 1024, False,
                                     XA_ATOM, &actual_type, &actual_format,
                                     &nitems, &bytes_after, &prop_data);

    if (status != Success) {
		if (prop_data != NULL)
			XFree(prop_data);
		
		return 0;
	}

	Atom *atoms = (Atom *)prop_data;
	u64 i;
	for (i = 0; i < nitems; ++i) {
		if (atoms[i] == net_wm_state_maximized_horz ||
			atoms[i] == net_wm_state_maximized_vert) {
			XFree(prop_data);
			return 1;
		}
	}

    return 0;
}

char keyboard[32];

u8 RGFW_isPressedI(RGFW_window* win, u32 key) {
	Display* d;
	if (win == (RGFW_window*)0)
		d = RGFW_root->src.display;
	else if (!win->event.inFocus)
		return 0;
	else
		d = (Display*)win->src.display;

	XQueryKeymap(d, keyboard); /* query the keymap */

	KeyCode kc2 = XKeysymToKeycode(d, key); /* convert the key to a keycode */
	return !!(keyboard[kc2 >> 3] & (1 << (kc2 & 7)));				/* check if the key is pressed */
}

#endif

#ifdef RGFW_WINDOWS
char* createUTF8FromWideStringWin32(const WCHAR* source);

#define GL_FRONT				0x0404
#define GL_BACK					0x0405
#define GL_LEFT					0x0406
#define GL_RIGHT				0x0407

#if defined(RGFW_OSMESA) && defined(RGFW_LINK_OSMESA)

typedef void (GLAPIENTRY * PFN_OSMesaDestroyContext)(OSMesaContext);
typedef i32 (GLAPIENTRY * PFN_OSMesaMakeCurrent)(OSMesaContext,void*,int,int,int);
typedef OSMesaContext (GLAPIENTRY * PFN_OSMesaCreateContext)(GLenum, OSMesaContext);

PFN_OSMesaMakeCurrent OSMesaMakeCurrentSource;
PFN_OSMesaCreateContext OSMesaCreateContextSource;
PFN_OSMesaDestroyContext OSMesaDestroyContextSource;

#define OSMesaCreateContext OSMesaCreateContextSource
#define OSMesaMakeCurrent OSMesaMakeCurrentSource
#define OSMesaDestroyContext OSMesaDestroyContextSource
#endif

typedef int (*PFN_wglGetSwapIntervalEXT)(void);
PFN_wglGetSwapIntervalEXT wglGetSwapIntervalEXTSrc = NULL;
#define wglGetSwapIntervalEXT wglGetSwapIntervalEXTSrc



#if defined(RGFW_DIRECTX)
RGFW_directXinfo RGFW_dxInfo;

RGFW_directXinfo* RGFW_getDirectXInfo(void) { return &RGFW_dxInfo; }
#endif

void* RGFWjoystickApi = NULL;

/* these two wgl functions need to be preloaded */
typedef HGLRC WINAPI (*wglCreateContextAttribsARB_type)(HDC hdc, HGLRC hShareContext,
        const i32 *attribList);
wglCreateContextAttribsARB_type wglCreateContextAttribsARB = NULL;

/* defines for creating ARB attributes */
#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_TRANSPARENT_ARB   					  0x200A
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201a
#define WGL_ALPHA_BITS_ARB 0x201b
#define WGL_ALPHA_SHIFT_ARB 0x201c
#define WGL_ACCUM_BITS_ARB 0x201d
#define WGL_ACCUM_RED_BITS_ARB 0x201e
#define WGL_ACCUM_GREEN_BITS_ARB 0x201f
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024
#define WGL_STEREO_ARB 0x2012
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB 					  0x2023
#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_SAMPLE_BUFFERS_ARB               0x2041
#define WGL_SAMPLES_ARB 0x2042
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB 0x20a9

#ifdef RGFW_WGL_LOAD
static HMODULE wglinstance = NULL;
typedef HGLRC(WINAPI *PFN_wglCreateContext)(HDC);
typedef BOOL(WINAPI *PFN_wglDeleteContext)(HGLRC);
typedef PROC(WINAPI *PFN_wglGetProcAddress)(LPCSTR);
typedef BOOL(WINAPI *PFN_wglMakeCurrent)(HDC, HGLRC);
typedef HDC (WINAPI *PFN_wglGetCurrentDC)();
typedef HGLRC (WINAPI *PFN_wglGetCurrentContext)();

PFN_wglCreateContext wglCreateContextSRC;
PFN_wglDeleteContext wglDeleteContextSRC;
PFN_wglGetProcAddress wglGetProcAddressSRC;
PFN_wglMakeCurrent wglMakeCurrentSRC;
PFN_wglGetCurrentDC wglGetCurrentDCSRC;
PFN_wglGetCurrentContext wglGetCurrentContextSRC;

#define wglCreateContext wglCreateContextSRC
#define wglDeleteContext wglDeleteContextSRC
#define wglGetProcAddress wglGetProcAddressSRC
#define wglMakeCurrent wglMakeCurrentSRC

#define wglGetCurrentDC wglGetCurrentDCSRC
#define wglGetCurrentContext wglGetCurrentContextSRC
#endif

#ifdef RGFW_OPENGL
void* RGFW_getProcAddress(const char* procname) { return (void*)wglGetProcAddress(procname); }

typedef BOOL (APIENTRY *PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
#endif

RGFW_window* RGFW_createWindow(const char* name, RGFW_rect rect, u16 args) {
	#ifdef RGFW_WGL_LOAD
	if (wglinstance == NULL) { 
		wglinstance = LoadLibraryA("opengl32.dll");

		wglCreateContextSRC = (PFN_wglCreateContext) GetProcAddress(wglinstance, "wglCreateContext");
		wglDeleteContextSRC = (PFN_wglDeleteContext) GetProcAddress(wglinstance, "wglDeleteContext");
		wglGetProcAddressSRC = (PFN_wglGetProcAddress) GetProcAddress(wglinstance, "wglGetProcAddress");
		wglMakeCurrentSRC = (PFN_wglMakeCurrent) GetProcAddress(wglinstance, "wglMakeCurrent");
		wglGetCurrentDCSRC = (PFN_wglGetCurrentDC) GetProcAddress(wglinstance, "wglGetCurrentDC");
		wglGetCurrentContextSRC = (PFN_wglGetCurrentContext) GetProcAddress(wglinstance, "wglGetCurrentContext");
	}
	#endif

    if (name[0] == 0) name = (char*)" ";

	RGFW_window* win = RGFW_window_basic_init(rect, args);

    HINSTANCE inh = GetModuleHandle(NULL);

    WNDCLASSA Class = {0}; /* Setup the Window class. */
	Class.lpszClassName = name;
	Class.hInstance = inh;
	Class.hCursor = LoadCursor(NULL, IDC_ARROW);
	Class.lpfnWndProc = DefWindowProc;

    RegisterClassA(&Class);

	DWORD window_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    RECT windowRect, clientRect;

	if (!(RGFW_NO_BORDER & args)) {
		window_style |= WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_VISIBLE | WS_MINIMIZEBOX;
		
		if (!(RGFW_NO_RESIZE & args))
			window_style |= WS_SIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
	}
	else
		window_style |= WS_POPUP | WS_VISIBLE |  WS_SYSMENU | WS_MINIMIZEBOX;

	HWND dummyWin = CreateWindowA(Class.lpszClassName, name, window_style, win->r.x, win->r.y, win->r.w, win->r.h, 0, 0, inh, 0);

	GetWindowRect(dummyWin, &windowRect);
	GetClientRect(dummyWin, &clientRect);

	u32 hOffset = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);
    win->src.display = CreateWindowA( Class.lpszClassName, name, window_style, win->r.x, win->r.y, win->r.w, win->r.h + hOffset, 0, 0, inh, 0);

	if (RGFW_TRANSPARENT_WINDOW & args) {
		SetWindowLong((HWND)win->src.display, GWL_EXSTYLE, GetWindowLong((HWND)win->src.display, GWL_EXSTYLE) | WS_EX_LAYERED);
	}
	if (RGFW_ALLOW_DND & args) {
		win->src.winArgs |= RGFW_ALLOW_DND;
		DragAcceptFiles((HWND)win->src.display, TRUE);
	}
    win->src.window = GetDC((HWND)win->src.display);

	#ifdef RGFW_DIRECTX
    assert(FAILED(CreateDXGIFactory(&__uuidof(IDXGIFactory), (void**)&RGFW_dxInfo.pFactory)) == 0);

    if (FAILED(RGFW_dxInfo.pFactory->lpVtbl->EnumAdapters(RGFW_dxInfo.pFactory, 0, &RGFW_dxInfo.pAdapter))) {
    	fprintf(stderr, "Failed to enumerate DXGI adapters\n");
        RGFW_dxInfo.pFactory->lpVtbl->Release(RGFW_dxInfo.pFactory);
        return NULL;
    }

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    if (FAILED(D3D11CreateDevice(RGFW_dxInfo.pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &RGFW_dxInfo.pDevice, NULL, &RGFW_dxInfo.pDeviceContext))) {
        fprintf(stderr, "Failed to create Direct3D device\n");
        RGFW_dxInfo.pAdapter->lpVtbl->Release(RGFW_dxInfo.pAdapter);
        RGFW_dxInfo.pFactory->lpVtbl->Release(RGFW_dxInfo.pFactory);
        return NULL;
    }

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = win->r.w;
    swapChainDesc.BufferDesc.Height = win->r.h;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = (HWND)win->src.display;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    RGFW_dxInfo.pFactory->lpVtbl->CreateSwapChain(RGFW_dxInfo.pFactory, (IUnknown*)RGFW_dxInfo.pDevice, &swapChainDesc, &win->src.swapchain);
	
	ID3D11Texture2D* pBackBuffer;
	win->src.swapchain->lpVtbl->GetBuffer(win->src.swapchain, 0, &__uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    RGFW_dxInfo.pDevice->lpVtbl->CreateRenderTargetView(RGFW_dxInfo.pDevice, (ID3D11Resource*)pBackBuffer, NULL, &win->src.renderTargetView);
    pBackBuffer->lpVtbl->Release(pBackBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = win->r.w;
	depthStencilDesc.Height = win->r.h;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* pDepthStencilTexture = NULL;
	RGFW_dxInfo.pDevice->lpVtbl->CreateTexture2D(RGFW_dxInfo.pDevice, &depthStencilDesc, NULL, &pDepthStencilTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	RGFW_dxInfo.pDevice->lpVtbl->CreateDepthStencilView(RGFW_dxInfo.pDevice, (ID3D11Resource*)pDepthStencilTexture, &depthStencilViewDesc, &win->src.pDepthStencilView);
	
	pDepthStencilTexture->lpVtbl->Release(pDepthStencilTexture);

	RGFW_dxInfo.pDeviceContext->lpVtbl->OMSetRenderTargets(RGFW_dxInfo.pDeviceContext, 1, &win->src.renderTargetView, win->src.pDepthStencilView);
	#endif

 	#ifdef RGFW_OPENGL 
    
	HDC dummy_dc = GetDC(dummyWin);

    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(pfd),
        .nVersion = 1,
        .iPixelType = PFD_TYPE_RGBA,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .cColorBits = 32,
        .cAlphaBits = 8,
        .iLayerType = PFD_MAIN_PLANE,
        .cDepthBits = 24,
        .cStencilBits = 8,
    };

    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    SetPixelFormat(dummy_dc, pixel_format, &pfd);

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    wglMakeCurrent(dummy_dc, dummy_context);

	if (wglChoosePixelFormatARB == NULL) {
		wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type)wglGetProcAddress("wglCreateContextAttribsARB");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	}

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummyWin, dummy_dc);

    if (wglCreateContextAttribsARB != NULL) {
		PIXELFORMATDESCRIPTOR pfd = {sizeof(pfd), 1, PFD_TYPE_RGBA, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 32, 8, PFD_MAIN_PLANE, 24, 8};

		if (RGFW_OPENGL_SOFTWARE & args)
			pfd.dwFlags |= PFD_GENERIC_FORMAT | PFD_GENERIC_ACCELERATED;

		if (wglChoosePixelFormatARB != NULL) {
			int pixel_format_attribs[] = {
				WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
				WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
				WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB,         32,
				WGL_DEPTH_BITS_ARB,         24,
				WGL_STENCIL_BITS_ARB,       8,
				WGL_SAMPLES_ARB,            4,
				0
			};

			int pixel_format;
			UINT num_formats;
			wglChoosePixelFormatARB(win->src.window, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
			if (!num_formats) {
				printf("Failed to set the OpenGL 3.3 pixel format.\n");
			}	

			DescribePixelFormat(win->src.window, pixel_format, sizeof(pfd), &pfd);
			if (!SetPixelFormat(win->src.window, pixel_format, &pfd)) {
				printf("Failed to set the OpenGL 3.3 pixel format.\n");
			}
		}

		i32 index = 0;
		i32 attribs[40];

		SET_ATTRIB(WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB);

		if (RGFW_majorVersion || RGFW_minorVersion) {
			SET_ATTRIB(WGL_CONTEXT_MAJOR_VERSION_ARB, RGFW_majorVersion);
			SET_ATTRIB(WGL_CONTEXT_MINOR_VERSION_ARB, RGFW_minorVersion);
		}

		SET_ATTRIB(WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB);

		if (RGFW_majorVersion || RGFW_minorVersion) {
			SET_ATTRIB(WGL_CONTEXT_MAJOR_VERSION_ARB, RGFW_majorVersion);
			SET_ATTRIB(WGL_CONTEXT_MINOR_VERSION_ARB, RGFW_minorVersion);
		}

		SET_ATTRIB(0, 0);

		win->src.rSurf = wglCreateContextAttribsARB((HDC)win->src.window, NULL, attribs);
	}
	else {
		fprintf(stderr, "Failed to create an accelerated OpenGL Context\n");

		int pixel_format = ChoosePixelFormat(win->src.window, &pfd);
		SetPixelFormat(win->src.window, pixel_format, &pfd);
		
		win->src.rSurf = wglCreateContext((HDC)win->src.window);
		wglMakeCurrent(win->src.window, win->src.rSurf);
	}
	#endif

	#ifdef RGFW_OPENGL
	if (RGFW_root != NULL && wglShareLists((HGLRC)RGFW_root->src.rSurf, (HGLRC)win->src.rSurf) == 0)
		fprintf(stderr, "Failed to link to dummy context : %li\n", GetLastError());
	#endif

	#ifdef RGFW_OSMESA
	#ifdef RGFW_LINK_OSMESA
	OSMesaMakeCurrentSource = (PFN_OSMesaMakeCurrent) GetProcAddress(win->src.display, "OSMesaMakeCurrent");
	OSMesaCreateContextSource = (PFN_OSMesaCreateContext) GetProcAddress(win->src.display, "OSMesaCreateContext");
	OSMesaDestroyContextSource = (PFN_OSMesaDestroyContext) GetProcAddress(win->src.display, "OSMesaDestroyContext");
	#endif
	#endif

	#ifdef RGFW_OPENGL
		ReleaseDC((HWND)win->src.display, (HDC)win->src.window);
		win->src.window = GetDC((HWND)win->src.display);
		wglMakeCurrent((HDC)win->src.window, (HGLRC)win->src.rSurf);
	#endif

	DestroyWindow(dummyWin);
	RGFW_init_buffer(win);
	
	#ifdef RGFW_EGL
	RGFW_createOpenGLContext(win);
	#endif

	if (RGFW_HIDE_MOUSE & args)
		RGFW_window_showMouse(win, 0);

    ShowWindow((HWND)win->src.display, SW_SHOWNORMAL);

	if (RGFW_root == NULL)
		RGFW_root = win;
	
    return win;
}


RGFW_area RGFW_getScreenSize(void) {
	return RGFW_AREA(GetDeviceCaps(GetDC(NULL), HORZRES), GetDeviceCaps(GetDC(NULL), VERTRES));
}

RGFW_vector RGFW_getGlobalMousePoint(void) {
	POINT p; 
	GetCursorPos(&p);

	return RGFW_VECTOR(p.x, p.y);
}

void RGFW_window_setMinSize(RGFW_window* win, RGFW_area a) {
	assert(win != NULL);
	win->src.minSize = a;
}

void RGFW_window_setMaxSize(RGFW_window* win, RGFW_area a) {
	assert(win != NULL);
	win->src.maxSize = a;
}


void RGFW_window_minimize(RGFW_window* win) {
	assert(win != NULL);
	
    ShowWindow((HWND)win->src.display, SW_MINIMIZE);
}

void RGFW_window_restore(RGFW_window* win) {
	assert(win != NULL);
	
	ShowWindow((HWND)win->src.display, SW_RESTORE);
}

RGFW_Event* RGFW_window_checkEvent(RGFW_window* win) {
	assert(win != NULL);
	
	MSG msg;

	if (win->event.droppedFilesCount) {
		i32 i;
		for (i = 0; i < win->event.droppedFilesCount; i++)
			win->event.droppedFiles[i][0] = '\0';
	}

	win->event.droppedFilesCount = 0;

    win->event.inFocus = (GetForegroundWindow() == win->src.display);

	if (PeekMessage(&msg, (HWND)win->src.display, 0u, 0u, PM_REMOVE)) {
		switch (msg.message) {
			case WM_CLOSE:
			case WM_QUIT:
				win->event.type = RGFW_quit;
				break;

			case WM_KEYUP:
				win->event.keyCode = msg.wParam;
				GetKeyNameTextA(msg.lParam, win->event.keyName, 16);
				win->event.type = RGFW_keyReleased;
				break;

			case WM_KEYDOWN:
				win->event.keyCode = msg.wParam;
				
				GetKeyNameTextA(msg.lParam, win->event.keyName, 16);
				win->event.type = RGFW_keyPressed;
				break;

			case WM_MOUSEMOVE:
				win->event.point.x = GET_X_LPARAM(msg.lParam);
				win->event.point.y = GET_Y_LPARAM(msg.lParam);

				win->event.type = RGFW_mousePosChanged;
				break;

			case WM_LBUTTONDOWN:
				win->event.button = RGFW_mouseLeft;
				win->event.type = RGFW_mouseButtonPressed;
				break;
			case WM_RBUTTONDOWN:
				win->event.button = RGFW_mouseRight;
				win->event.type = RGFW_mouseButtonPressed;
				break;
			case WM_MBUTTONDOWN:
				win->event.button = RGFW_mouseMiddle;
				win->event.type = RGFW_mouseButtonPressed;
				break;

			case WM_MOUSEWHEEL:
				if (msg.wParam > 0)
					win->event.button = RGFW_mouseScrollUp;
				else
					win->event.button = RGFW_mouseScrollDown;

				win->event.type = RGFW_mouseButtonPressed;
				break;

			case WM_LBUTTONUP:
				win->event.button = RGFW_mouseLeft;
				win->event.type = RGFW_mouseButtonReleased;
				break;
			case WM_RBUTTONUP:
				win->event.button = RGFW_mouseRight;
				win->event.type = RGFW_mouseButtonReleased;
				break;
			case WM_MBUTTONUP:
				win->event.button = RGFW_mouseMiddle;
				win->event.type = RGFW_mouseButtonReleased;
				break;

			/*
				much of this event is source from glfw
			*/
			case WM_DROPFILES: {
					win->event.type = RGFW_dnd;

					HDROP drop = (HDROP)msg.wParam;
					POINT pt;
					i32 i;

					win->event.droppedFilesCount = DragQueryFileW(drop, 0xffffffff, NULL, 0);
					//win->event.droppedFiles = (char**)RGFW_CALLOC(win->event.droppedFilesCount, sizeof(char*));

					/* Move the mouse to the position of the drop */
					DragQueryPoint(drop, &pt);

					win->event.point.x = pt.x;
					win->event.point.y = pt.y;

					for (i = 0;  i < win->event.droppedFilesCount;  i++) {
						const UINT length = DragQueryFileW(drop, i, NULL, 0);
						WCHAR* buffer = (WCHAR*)RGFW_CALLOC((size_t) length + 1, sizeof(WCHAR));

						DragQueryFileW(drop, i, buffer, length + 1);
						strcpy(win->event.droppedFiles[i], createUTF8FromWideStringWin32(buffer));

						RGFW_FREE(buffer);
					}

					DragFinish(drop);
				}
				break;

			case WM_SIZE: {
				win->event.type = RGFW_windowAttribsChange;
				win->r.w = LOWORD(msg.lParam);
				win->r.h = HIWORD(msg.lParam);
				break;
			}
			case WM_MOVE: {
				win->event.type = RGFW_windowAttribsChange;
				win->r.x = GET_X_LPARAM(msg.lParam);
				win->r.y = GET_Y_LPARAM(msg.lParam);
				break;
			}
			case WM_GETMINMAXINFO:
			{
				if (win->src.maxSize.w == 0 && win->src.maxSize.h == 0)
					break;

				MINMAXINFO* mmi = (MINMAXINFO*)msg.lParam;
				mmi->ptMinTrackSize.x = win->src.minSize.w;
				mmi->ptMinTrackSize.y = win->src.minSize.h;
				mmi->ptMaxTrackSize.x = win->src.maxSize.w;
				mmi->ptMaxTrackSize.y = win->src.maxSize.h;
				return 0;
			}
			default:
				win->event.type = 0;
				break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	else 
		win->event.type = 0;

	win->event.lockState = 0;

	if ((GetKeyState(VK_CAPITAL) & 0x0001)!=0)
		win->event.lockState |= RGFW_CAPSLOCK;
	if ((GetKeyState(VK_NUMLOCK) & 0x0001)!=0)
		win->event.lockState |= RGFW_NUMLOCK;
	if ((GetKeyState(VK_SCROLL) & 0x0001)!=0)
		win->event.lockState |= 3;


	if (!IsWindow((HWND)win->src.display))
		win->event.type = RGFW_quit;

	if (win->event.type)
		return &win->event;
	else
		return NULL;
}

u8 RGFW_window_isFullscreen(RGFW_window* win) {
	assert(win != NULL);
	
    WINDOWPLACEMENT placement;
    GetWindowPlacement((HWND)win->src.display, &placement);
    return placement.showCmd == SW_SHOWMAXIMIZED;
}

u8 RGFW_window_isHidden(RGFW_window* win) {
	assert(win != NULL);
	
    return IsWindowVisible((HWND)win->src.display) == 0 && !RGFW_isMinimized(win);
}

u8 RGFW_isMinimized(RGFW_window* win) {
	assert(win != NULL);
	
    WINDOWPLACEMENT placement;
    GetWindowPlacement((HWND)win->src.display, &placement);
    return placement.showCmd == SW_SHOWMINIMIZED;
}

u8 RGFW_isMaximized(RGFW_window* win) {
	assert(win != NULL);
	
    WINDOWPLACEMENT placement;
    GetWindowPlacement((HWND)win->src.display, &placement);
    return placement.showCmd == SW_SHOWMAXIMIZED;
}

u8 RGFW_isPressedI(RGFW_window* win, u32 key) {
	if (win != NULL && !win->event.inFocus)
		return 0;

	if (GetAsyncKeyState(key) & 0x8000)
		return 1;
	else return 0;
}

HICON RGFW_loadHandleImage(RGFW_window* win, u8* src, RGFW_area a, BOOL icon) {
	assert(win != NULL);
	
  	i32 i;
    HDC dc;
    HICON handle;
    HBITMAP color, mask;
    BITMAPV5HEADER bi;
    ICONINFO ii;
    u8* target = NULL;
    u8* source = src;

    ZeroMemory(&bi, sizeof(bi));
    bi.bV5Size        = sizeof(bi);
    bi.bV5Width       = a.w;
    bi.bV5Height      = -a.h;
    bi.bV5Planes      = 1;
    bi.bV5BitCount    = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask     = 0x00ff0000;
    bi.bV5GreenMask   = 0x0000ff00;
    bi.bV5BlueMask    = 0x000000ff;
    bi.bV5AlphaMask   = 0xff000000;

    dc = GetDC(NULL);
    color = CreateDIBSection(dc,
                             (BITMAPINFO*) &bi,
                             DIB_RGB_COLORS,
                             (void**) &target,
                             NULL,
                             (DWORD) 0);
    ReleaseDC(NULL, dc);

    mask = CreateBitmap(a.w, a.h, 1, 1, NULL);

    for (i = 0;  i < a.w * a.h;  i++) {
        target[0] = source[2];
        target[1] = source[1];
        target[2] = source[0];
        target[3] = source[3];
        target += 4;
        source += 4;
    }

    ZeroMemory(&ii, sizeof(ii));
    ii.fIcon    = icon;
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask  = mask;
    ii.hbmColor = color;

    handle = CreateIconIndirect(&ii);

    DeleteObject(color);
    DeleteObject(mask);

    return handle;
}

void RGFW_window_setMouse(RGFW_window* win, u8* image, RGFW_area a, i32 channels) {
	assert(win != NULL);
	
	HCURSOR cursor = (HCURSOR)RGFW_loadHandleImage(win, image, a, FALSE);
	SetClassLongPtr(win->src.display, GCLP_HCURSOR, (LPARAM)cursor);
	SetCursor(cursor);
	DestroyCursor(cursor);
}

void RGFW_window_setMouseDefault(RGFW_window* win) {
	assert(win != NULL);
	
	RGFW_window_showMouse(win, 1);
	SetClassLongPtr(win->src.display, GCLP_HCURSOR, (LPARAM)LoadCursor(NULL, IDC_ARROW));
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void RGFW_window_close(RGFW_window* win) {
	assert(win != NULL);
	
	#ifdef RGFW_VULKAN
    for (int i = 0; i < win->src.image_count; i++) {
        vkDestroyFramebuffer(RGFW_vulkan_info.device, RGFW_vulkan_info.framebuffers[i], NULL);
    }

    for (int i = 0; i < win->src.image_count; i++) {
        vkDestroyImageView(RGFW_vulkan_info.device, win->src.swapchain_image_views[i], NULL);
    }

    vkDestroySwapchainKHR(RGFW_vulkan_info.device, win->src.swapchain, NULL);
    vkDestroySurfaceKHR(RGFW_vulkan_info.instance, win->src.rSurf, NULL);
    free(win->src.swapchain_image_views);
    free(win->src.swapchain_images);
	#endif

	#ifdef RGFW_EGL
	RGFW_closeEGL(win);
	#endif

	if (win == RGFW_root) {
		#ifdef RGFW_DIRECTX
		RGFW_dxInfo.pDeviceContext->lpVtbl->Release(RGFW_dxInfo.pDeviceContext);
		RGFW_dxInfo.pDevice->lpVtbl->Release(RGFW_dxInfo.pDevice);
		RGFW_dxInfo.pAdapter->lpVtbl->Release(RGFW_dxInfo.pAdapter);
		RGFW_dxInfo.pFactory->lpVtbl->Release(RGFW_dxInfo.pFactory);
		#endif
		
		RGFW_root = NULL;
	}

	#ifdef RGFW_DIRECTX
	win->src.swapchain->lpVtbl->Release(win->src.swapchain);
	win->src.renderTargetView->lpVtbl->Release(win->src.renderTargetView);
	win->src.pDepthStencilView->lpVtbl->Release(win->src.pDepthStencilView);
	#endif

	#ifdef RGFW_BUFFER
	DeleteDC(win->src.hdcMem);
	DeleteObject(win->src.bitmap);
	#endif

	#ifdef RGFW_OPENGL
	wglDeleteContext((HGLRC)win->src.rSurf); /* delete opengl context */
	#endif
	DeleteDC((HDC)win->src.window); /* delete window */
	DestroyWindow((HWND)win->src.display); /* delete display */

	#if defined(RGFW_OSMESA)
	if (win->buffer != NULL)
		RGFW_FREE(win->buffer);
	#endif

	#ifdef RGFW_ALLOC_DROPFILES
	{
		i32 i;
		for (i = 0; i < RGFW_MAX_DROPS; i++)
			RGFW_FREE(win->event.droppedFiles[i]);
    
	
		RGFW_FREE(win->event.droppedFiles);
	}
	#endif

	RGFW_FREE(win);
}

void RGFW_window_move(RGFW_window* win, RGFW_vector v) {
	assert(win != NULL);
	
	win->r.x = v.x;
	win->r.y = v.y;
	SetWindowPos((HWND)win->src.display, (HWND)win->src.display, win->r.x, win->r.y, win->r.w, win->r.h, 0);
}

void RGFW_window_resize(RGFW_window* win, RGFW_area a) {
	assert(win != NULL);
	
	win->r.w = a.w;
	win->r.h = a.h;
	SetWindowPos((HWND)win->src.display, (HWND)win->src.display, win->r.x, win->r.y, win->r.w, win->r.h, 0);
}


void RGFW_window_setName(RGFW_window* win, char* name) {
	assert(win != NULL);
	
	SetWindowTextA((HWND)win->src.display, name);
}

/* much of this function is sourced from GLFW */
void RGFW_window_setIcon(RGFW_window* win, u8* src, RGFW_area a, i32 channels) {
	assert(win != NULL);
	
    HICON handle = RGFW_loadHandleImage(win, src, a, TRUE);

	SetClassLongPtr(win->src.display, GCLP_HICON, (LPARAM)handle); 

	DestroyIcon(handle);
}

char* RGFW_readClipboard(size_t* size) {
    /* Open the clipboard */
    if (OpenClipboard(NULL) == 0)
        return (char*)"";

    /* Get the clipboard data as a Unicode string */
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData == NULL) {
        CloseClipboard();
        return (char*)"";
    }

	wchar_t* wstr = (wchar_t*)GlobalLock(hData);

	char* text;
	
	{
		setlocale(LC_ALL, "en_US.UTF-8");

		size_t textLen = wcstombs(NULL, wstr, 0);
		if (textLen == 0)
			return (char*)"";
		
		text = (char*)malloc((textLen * sizeof(char)) + 1);
		
		wcstombs(text, wstr, (textLen) + 1);
		
		if (size != NULL)
			*size = textLen + 1;
	}

    /* Release the clipboard data */
    GlobalUnlock(hData);
    CloseClipboard();

	return text;
}

void RGFW_writeClipboard(const char* text, u32 textLen) {
    HANDLE object;
    WCHAR* buffer;

    object = GlobalAlloc(GMEM_MOVEABLE, (1 + textLen) * sizeof(WCHAR));
    if (!object)
        return;

    buffer = (WCHAR*)GlobalLock(object);
    if (!buffer) {
        GlobalFree(object);
        return;
    }

    MultiByteToWideChar(CP_UTF8, 0, text, -1, buffer, textLen);
    GlobalUnlock(object);

    if (!OpenClipboard((HWND)RGFW_root->src.display)) {
        GlobalFree(object);
        return;
    }

    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, object);
    CloseClipboard();
}

u16 RGFW_registerJoystick(RGFW_window* win, i32 jsNumber) {
	assert(win != NULL);
	
	return RGFW_registerJoystickF(win, (char*)"");
}

u16 RGFW_registerJoystickF(RGFW_window* win, char* file) {
	assert(win != NULL);
	

	return win->src.joystickCount - 1;
}

void RGFW_window_showMouse(RGFW_window* win, i8 show) { 
	assert(win != NULL);
	
	ShowCursor(show); 
}
void RGFW_window_moveMouse(RGFW_window* win, RGFW_vector p) { 
	assert(win != NULL);
	
	SetCursorPos(p.x, p.y); 
}

char* createUTF8FromWideStringWin32(const WCHAR* source) {
    char* target;
  	i32 size;

    size = WideCharToMultiByte(CP_UTF8, 0, source, -1, NULL, 0, NULL, NULL);
    if (!size) {
        return NULL;
    }

    target = (char*)RGFW_CALLOC(size, 1);

    if (!WideCharToMultiByte(CP_UTF8, 0, source, -1, target, size, NULL, NULL)) {
        RGFW_FREE(target);
        return NULL;
    }

    return target;
}

#ifndef RGFW_NO_THREADS
RGFW_thread RGFW_createThread(void* (*function_ptr)(void*), void* args) { return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)*function_ptr, args, 0, NULL);  }
void RGFW_cancelThread(RGFW_thread thread) { CloseHandle((HANDLE)thread);  }
void RGFW_joinThread(RGFW_thread thread) { WaitForSingleObject((HANDLE)thread, INFINITE); }
void RGFW_setThreadPriority(RGFW_thread thread, u8 priority) { SetThreadPriority((HANDLE)thread, priority); }
#endif
#endif

#if defined(RGFW_MACOS)
	
void* RGFWnsglFramework = NULL; 

#ifdef RGFW_OPENGL
void* RGFW_getProcAddress(const char* procname) {
	if (RGFWnsglFramework == NULL)
		RGFWnsglFramework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));

    CFStringRef symbolName = CFStringCreateWithCString(kCFAllocatorDefault, procname, kCFStringEncodingASCII);

    void* symbol = CFBundleGetFunctionPointerForName(RGFWnsglFramework, symbolName);

    CFRelease(symbolName);

    return symbol;
}
#endif
	
CVReturn displayCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *inNow, const CVTimeStamp *inOutputTime, CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext) { return kCVReturnSuccess; }

RGFW_window** RGFW_windows;
u32 RGFW_windows_size = 0;

u32 RGFW_OnClose(void* self) {
	u32 i;
	for (i = 0; i < RGFW_windows_size; i++)
		if (RGFW_windows[i] && RGFW_windows[i]->src.window == self)
			break;

	RGFW_windows[i]->event.type = RGFW_quit;
	return true;
}

/* NOTE(EimaMei): Fixes the constant clicking when the app is running under a terminal. */
bool acceptsFirstResponder() { return true; }
bool performKeyEquivalent(NSEvent* event) { return true; }

NSDragOperation draggingEntered(NSDraggingInfo* sender) { return NSDragOperationCopy; }
NSDragOperation draggingUpdated(NSDraggingInfo* sender) { return NSDragOperationCopy; }
bool prepareForDragOperation(NSDraggingInfo* sender) { return true; }

/* NOTE(EimaMei): Usually, you never need 'id self, SEL cmd' for C -> Obj-C methods. This isn't the case. */
bool performDragOperation(id self, SEL cmd, NSDraggingInfo* sender) {
	NSWindow* window = NSDraggingInfo_draggingDestinationWindow(sender);

	u32 i;
	bool found = false;

	for (i = 0; i < RGFW_windows_size; i++)
		if (RGFW_windows[i]->src.window == window){
			found = true;
			break;
		}

	if (!found)
		i = 0;

	siArray(Class) array = si_array_init((Class[]){SI_NS_CLASSES[NS_URL_CODE]}, sizeof(*array), 1);
	siArray(char*) droppedFiles = (siArray(char*))NSPasteboard_readObjectsForClasses(NSDraggingInfo_draggingPasteboard(sender), array, NULL);

	RGFW_windows[i]->event.droppedFilesCount = si_array_len(droppedFiles);

	si_array_free(array);

	u32 y;

	for (y = 0; y < RGFW_windows[i]->event.droppedFilesCount; y++)
		strcpy(RGFW_windows[i]->event.droppedFiles[y], droppedFiles[y]);

	RGFW_windows[i]->event.type = RGFW_dnd;

	NSPoint p = NSDraggingInfo_draggingLocation(sender);
	RGFW_windows[i]->event.point.x = p.x;
	RGFW_windows[i]->event.point.x = p.y;

    return true;
}

typedef void NSNotification;
void RGFW_windowDidResize(void* sender, NSNotification *notification) {
printf("resize\n");
    // Handle window resize logic here...
}

NSApplication* NSApp;

RGFW_window* RGFW_createWindow(const char* name, RGFW_rect rect, u16 args) {
    static u8 RGFW_loaded = 0;

	/* NOTE(EimaMei): Why does Apple hate good code? Like wtf, who thought of methods being a great idea???
	Imagine a universe, where MacOS had a proper system API (we would probably have like 20% better performance).
	*/
	si_func_to_SEL_with_name(SI_DEFAULT, "windowShouldClose", RGFW_OnClose);
	si_func_to_SEL_with_name(SI_DEFAULT, "NSWindowDidResizeNotification", RGFW_windowDidResize);
	si_func_to_SEL_with_name("NSWindow", "windowDidResize", RGFW_windowDidResize);

	/* NOTE(EimaMei): Fixes the 'Boop' sfx from constantly playing each time you click a key. Only a problem when running in the terminal. */
	si_func_to_SEL("NSWindow", acceptsFirstResponder);
	si_func_to_SEL("NSWindow", performKeyEquivalent);

	void* WindowDelegateClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "WindowDelegate", 0);
	class_addMethod(WindowDelegateClass, sel_registerName("windowDidResize:"), (IMP)RGFW_windowDidResize,  "v@:@");

    NSApp = NSApplication_sharedApplication();

	RGFW_window* win = RGFW_window_basic_init(rect, args);

	NSRect windowRect = NSMakeRect(win->r.x, win->r.y, win->r.w, win->r.h);
	NSBackingStoreType macArgs = NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSBackingStoreBuffered | NSWindowStyleMaskTitled;

	if (!(RGFW_NO_RESIZE & args))
		macArgs |= NSWindowStyleMaskResizable;
	if (!(RGFW_NO_BORDER & args))
		macArgs |= NSWindowStyleMaskTitled;
	else
		macArgs |= NSWindowStyleMaskBorderless;

    win->src.window = NSWindow_init(windowRect, macArgs, NSBackingStoreBuffered, false);
	NSWindow_contentView_setWantsLayer(win->src.window, true);
	NSRetain(win->src.window);
	NSWindow_setTitle(win->src.window, name);

    if (RGFW_TRANSPARENT_WINDOW & args) {
		#ifdef RGFW_OPENGL
		i32 opacity = 0;
		NSOpenGLContext_setValues(win->src.rSurf, &opacity, NSOpenGLContextParameterSurfaceOpacity);
		#endif
		NSWindow_setOpaque(win->src.window, false);
		NSWindow_setBackgroundColor(win->src.window, NSColor_colorWithSRGB(0, 0, 0, 0));
		NSWindow_setAlphaValue(win->src.window, 0x00);
	}

	#ifdef RGFW_OPENGL
	NSOpenGLPixelFormatAttribute attributes[] = {
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFADepthSize, 24,
		NSOpenGLPFAStencilSize, 8,
		NSOpenGLPFANoRecovery,
		0, 0, 0
	};
	
	if (RGFW_OPENGL_SOFTWARE & args)
		attributes[2] = 0;

	if (RGFW_majorVersion >= 4 || RGFW_majorVersion  >= 3) {
		attributes[11] = NSOpenGLPFAOpenGLProfile;
        attributes[12] = (RGFW_majorVersion  >= 4) ? NSOpenGLProfileVersion4_1Core : NSOpenGLProfileVersion3_2Core;
	}

	NSOpenGLPixelFormat* format = NSOpenGLPixelFormat_initWithAttributes(attributes);
	win->src.view = NSOpenGLView_initWithFrame(NSMakeRect(0, 0, win->r.w, win->r.h), format);
	NSOpenGLView_prepareOpenGL(win->src.view);

	NSOpenGLContext_makeCurrentContext(win->src.rSurf);

	#else
    NSRect contentRect = NSMakeRect(0, 0, win->r.w, win->r.h);
    win->src.view = NSView_initWithFrame(contentRect);
	#endif

	RGFW_init_buffer(win);

    NSWindow_setContentView(win->src.window, win->src.view);

	if (RGFW_ALLOW_DND & args) {
		win->src.winArgs |= RGFW_ALLOW_DND;
		
		siArray(NSPasteboardType) array = si_array_init((NSPasteboardType[]){NSPasteboardTypeURL, NSPasteboardTypeFileURL, NSPasteboardTypeString}, sizeof(*array), 3);
	    NSView_registerForDraggedTypes(win->src.view, array);
		si_array_free(array);

		/* NOTE(EimaMei): Drag 'n Drop requires too many damn functions for just a Drag 'n Drop event. */
		si_func_to_SEL("NSWindow", draggingEntered);
		si_func_to_SEL("NSWindow", draggingUpdated);
		si_func_to_SEL("NSWindow", prepareForDragOperation);
		si_func_to_SEL("NSWindow", performDragOperation);
	}

	if (RGFW_HIDE_MOUSE & args)
		RGFW_window_showMouse(win, 0);
	
    // Show the window
    NSWindow_makeKeyAndOrderFront(win->src.window, NULL);
	NSWindow_setIsVisible(win->src.window, true);

	NSApplication_setActivationPolicy(NSApp, NSApplicationActivationPolicyRegular);
	NSApplication_finishLaunching(NSApp);
		
	if (!RGFW_loaded) {	
		NSWindow_makeMainWindow(win->src.window);

		RGFW_loaded = 1;
    }
	
	RGFW_windows_size++;

	RGFW_window** nWins = (RGFW_window**)malloc(sizeof(RGFW_window*) * RGFW_windows_size);
	
	memcpy(nWins, RGFW_windows, sizeof(RGFW_window) * (RGFW_windows_size - 1));
	free(RGFW_windows);
	
	RGFW_windows = nWins;

	RGFW_windows[RGFW_windows_size - 1] = NULL;
	
	size_t i;
	for (i = 0; i < RGFW_windows_size; i++)
		if (!RGFW_windows[i]){
			RGFW_windows[i] = win;
			break;
		}

	if (RGFW_root == NULL)
		RGFW_root = win;
	
    return win;
}


RGFW_area RGFW_getScreenSize(void){
	static CGDirectDisplayID display = 0;
	
	if (display == 0)
		display = CGMainDisplayID();

	return RGFW_AREA(CGDisplayPixelsWide(display), CGDisplayPixelsHigh(display));
}

RGFW_vector RGFW_getGlobalMousePoint(void) {
	assert(RGFW_root != NULL);

	return RGFW_root->event.point; /* the point is loaded during event checks */
}

u32 RGFW_keysPressed[10]; /*10 keys at a time*/

RGFW_Event* RGFW_window_checkEvent(RGFW_window* win) {
	assert(win != NULL);
	
	if (win->event.droppedFilesCount) {
		i32 i;
		for (i = 0; i < win->event.droppedFilesCount; i++)
			win->event.droppedFiles[i][0] = '\0';
	}

	win->event.droppedFilesCount = 0;

	win->event.inFocus = NSWindow_isKeyWindow(win->src.window);

	/* NOTE(EimaMei): This is super janky code, THANKS APPLE. For some reason it takes a few frames AFTER becoming focused to allow setting the cursor. */
	if (win->event.inFocus && win->src.cursor != NULL && win->src.cursor != NULL && (win->src.cursorChanged != 2 || NSCursor_currentCursor() != win->src.cursor)) {
		if (win->src.cursorChanged != 2)
			win->src.cursorChanged++;

		if (win->src.cursorChanged != 2 || NSCursor_currentCursor() != win->src.cursor)
			NSCursor_set(win->src.cursor);
	}

	NSEvent* e = NSApplication_nextEventMatchingMask(NSApp, NSEventMaskAny, NSDate_distantFuture(), NSDefaultRunLoopMode, true);

	if (NSEvent_window(e) == win->src.window) {
		u8 button = 0;

		switch (NSEvent_type(e)) {
			case NSEventTypeKeyDown:
				win->event.type = RGFW_keyPressed;
				win->event.keyCode = (u16)NSEvent_keyCode(e);
				win->event.keyName = (char*)NSEvent_characters(e);

				RGFW_keyMap[win->event.keyCode] = 1;
				break;

			case NSEventTypeKeyUp:
				win->event.type = RGFW_keyReleased;
				win->event.keyCode = (u16)NSEvent_keyCode(e);
				win->event.keyName = (char*)NSEvent_characters(e);

				RGFW_keyMap[win->event.keyCode] = 0;

				break;

			case NSEventTypeLeftMouseDown:
				button = 1;
				win->event.button = 2;
			case NSEventTypeOtherMouseDown:
				button = 1;
				if (!button)
				win->event.button = 1;
			case NSEventTypeRightMouseDown:
				if (!button)
				win->event.button = 0;

				win->event.type = RGFW_mouseButtonPressed;
				break;

			case NSEventTypeLeftMouseUp:
				button = 1;
				if (NSEvent_type(e) == NSEventTypeLeftMouseUp)
					win->event.button = RGFW_mouseLeft;
			case NSEventTypeOtherMouseUp:
				if (!button && NSEvent_type(e) == NSEventTypeOtherMouseUp)
				win->event.button = RGFW_mouseMiddle;
				button = 1;
			case NSEventTypeScrollWheel:
				if (!button && NSEvent_type(e) == NSEventTypeScrollWheel){
					double deltaY = NSEvent_deltaY(e);

					if (deltaY > 0)
						win->event.button = RGFW_mouseScrollUp;

					else if (deltaY < 0)
						win->event.button = RGFW_mouseScrollDown;
				}
				button = 1;
			case NSEventTypeRightMouseUp:
				if (!button)
				win->event.button = RGFW_mouseRight;

				win->event.type = RGFW_mouseButtonReleased;
				break;

			case NSEventTypeMouseMoved:
				win->event.type = RGFW_mousePosChanged;

				NSPoint p = NSEvent_locationInWindow(e);

				win->event.point.x = p.x;
				win->event.point.y = p.y;
				break;


			default: break;
		}

		if (win->src.cursorChanged && win->event.inFocus) {
			if (win->src.cursor == NULL)
				CGDisplayHideCursor(kCGDirectMainDisplay);
			else {
				CGDisplayShowCursor(kCGDirectMainDisplay);

				if (win->src.cursor == NULL) 
					NSCursor_set(NSCursor_arrowCursor());
				else 
					NSCursor_set(win->src.cursor);
			}

			win->src.cursorChanged = false;
		}
	}

	NSApplication_sendEvent(NSApp, e);

	NSApplication_updateWindows(NSApp);

	if (win->event.type)
		return &win->event;
	else
		return NULL;
}


void RGFW_window_move(RGFW_window* win, RGFW_vector v) {
	assert(win != NULL);

	win->r.x = v.x;
	win->r.y = v.y;
	NSWindow_setFrameAndDisplay(win->src.window, NSMakeRect(win->r.x, win->r.y, win->r.w, win->r.h), true, true);
}

void RGFW_window_resize(RGFW_window* win, RGFW_area a) {
	assert(win != NULL);
	
	win->r.w = a.w;
	win->r.h = a.h;
	NSWindow_setFrameAndDisplay(win->src.window, NSMakeRect(win->r.x, win->r.y, win->r.w, win->r.h), true, true);
}

void RGFW_window_minimize(RGFW_window* win) {
	assert(win != NULL);
	
	NSWindow_performMiniaturize(win->src.window, NULL);
}

void RGFW_window_restore(RGFW_window* win) {
	assert(win != NULL);
	
	NSWindow_performZoom(win->src.window, NULL);
}

void RGFW_window_setName(RGFW_window* win, char* name) {
	assert(win != NULL);
	
	NSWindow_setTitle(win->src.window, name);
}

void RGFW_window_setMinSize(RGFW_window* win, RGFW_area a) {
	NSWindow_setMinSize(win->src.window, NSMakeSize(a.w, a.h));
}

void RGFW_window_setMaxSize(RGFW_window* win, RGFW_area a) {
    NSWindow_setMaxSize(win->src.window, NSMakeSize(a.w, a.h));
}

void RGFW_window_setIcon(RGFW_window* win, u8* data, RGFW_area area, i32 channels) {
	assert(win != NULL);
	
	/* code by EimaMei  */
    // Make a bitmap representation, then copy the loaded image into it.
    NSBitmapImageRep* representation = NSBitmapImageRep_initWithBitmapData(NULL, area.w, area.h, 8, channels, (channels == 4), false, "NSCalibratedRGBColorSpace", NSBitmapFormatAlphaNonpremultiplied, area.w * channels, 8 * channels);
    memcpy(NSBitmapImageRep_bitmapData(representation), data, area.w * area.h * channels);

    // Add ze representation.
    NSImage* dock_image = NSImage_initWithSize(NSMakeSize(area.w, area.h));
    NSImage_addRepresentation(dock_image, (NSImageRep*)representation);

    // Finally, set the dock image to it.
    NSApplication_setApplicationIconImage(NSApp, dock_image);

    // Free the garbage.
    release(dock_image);
    release(representation);
}

void RGFW_window_setMouse(RGFW_window* win, u8* image, RGFW_area a, i32 channels) {
	assert(win != NULL);
	
	if (image == NULL) {
		NSCursor_set(NSCursor_arrowCursor());
		win->src.cursor = NULL;

		return ;
	}

	if (win->src.cursor != NULL && win->src.cursor != NULL)
		release(win->src.cursor);

	/* NOTE(EimaMei): Code by yours truly. */
    // Make a bitmap representation, then copy the loaded image into it.
    NSBitmapImageRep* representation = NSBitmapImageRep_initWithBitmapData(NULL, a.w, a.h, 8, channels, (channels == 4), false, "NSCalibratedRGBColorSpace", NSBitmapFormatAlphaNonpremultiplied, a.w * channels, 8 * channels);
    memcpy(NSBitmapImageRep_bitmapData(representation), image, a.w * a.h * channels);

    // Add ze representation.
    NSImage* cursor_image = NSImage_initWithSize(NSMakeSize(a.w, a.h));
    NSImage_addRepresentation(cursor_image, (NSImageRep*)representation);

    // Finally, set the cursor image.
    NSCursor* cursor = NSCursor_initWithImage(cursor_image, NSMakePoint(0, 0));

	win->src.cursor = cursor;
	win->src.cursorChanged = true;

    // Free the garbage.
    release(cursor_image);
    release(representation);
}

void RGFW_window_setMouseDefault(RGFW_window* win) {
	assert(win != NULL);
	
	if (win->src.cursor != NULL && win->src.cursor != NULL)
		release(win->src.cursor);
	
	RGFW_window_setMouse(win, NULL, RGFW_AREA(0, 0), 0);
}

void RGFW_window_moveMouse(RGFW_window* win, RGFW_vector v) {
	assert(win != NULL);
	
	CGEventRef moveEvent = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, CGPointMake(v.x, v.y), kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, moveEvent);
	CFRelease(moveEvent);
}

u8 RGFW_window_isFullscreen(RGFW_window* win) {
	assert(win != NULL);
	
    return (NSWindow_styleMask(win->src.window) & NSFullScreenWindowMask) == NSFullScreenWindowMask;
}

u8 RGFW_window_isHidden(RGFW_window* win) {
	assert(win != NULL);
	
    return NSWindow_isVisible(win) == NO && !RGFW_isMinimized(win);
}

u8 RGFW_isMinimized(RGFW_window* win) {
	assert(win != NULL);
	
    return NSWindow_isMiniaturized(win->src.window) == YES;
}

u8 RGFW_isMaximized(RGFW_window* win) {
	assert(win != NULL);
	
    return NSWindow_isZoomed(win->src.window) == YES;
}

u8 RGFW_isPressedI(RGFW_window* win, u32 key) {
	if (key >= 128){
		#ifdef RGFW_PRINT_ERRORS
		fprintf(stderr, "RGFW_isPressedI : invalid keycode\n");
		#endif
		RGFW_error = 1;
	}

	return RGFW_keyMap[key];
}

char* RGFW_readClipboard(size_t* size){
	char* str = (char*)NSPasteboard_stringForType(NSPasteboard_generalPasteboard(), NSPasteboardTypeString); 
	if (size != NULL)
		*size = strlen(str);
	return str;
}

void RGFW_writeClipboard(const char* text, u32 textLen) {
	siArray(NSPasteboardType) array = si_array_init((NSPasteboardType[]){NSPasteboardTypeString}, sizeof(*array), 1);
	NSPasteBoard_declareTypes(NSPasteboard_generalPasteboard(), array, NULL);

	NSPasteBoard_setString(NSPasteboard_generalPasteboard(), text, NSPasteboardTypeString);
	si_array_free(array);
}

u16 RGFW_registerJoystick(RGFW_window* win, i32 jsNumber) {
	assert(win != NULL);
	
	return RGFW_registerJoystickF(win, (char*)"");
}

u16 RGFW_registerJoystickF(RGFW_window* win, char* file) {
	assert(win != NULL);
	
	return win->src.joystickCount - 1;
}

void RGFW_window_close(RGFW_window* win){
	assert(win != NULL);
	
	#ifdef RGFW_VULKAN
    for (int i = 0; i < win->src.image_count; i++) {
        vkDestroyFramebuffer(RGFW_vulkan_info.device, RGFW_vulkan_info.framebuffers[i], NULL);
    }

    for (int i = 0; i < win->src.image_count; i++) {
        vkDestroyImageView(RGFW_vulkan_info.device, win->src.swapchain_image_views[i], NULL);
    }

    vkDestroySwapchainKHR(RGFW_vulkan_info.device, win->src.swapchain, NULL);
    vkDestroySurfaceKHR(RGFW_vulkan_info.instance, win->src.rSurf, NULL);
    free(win->src.swapchain_image_views);
    free(win->src.swapchain_images);
	#endif
	
	release(win->src.view);

	if (win->src.cursor != NULL && win->src.cursor != NULL)
		release(win->src.cursor);

	#ifdef RGFW_ALLOC_DROPFILES
	{
		i32 i;
		for (i = 0; i < RGFW_MAX_DROPS; i++)
			RGFW_FREE(win->event.droppedFiles[i]);
    
	
		RGFW_FREE(win->event.droppedFiles);
	}
	#endif

	u32 i;
	for (i = 0; i < RGFW_windows_size; i++)
		if (RGFW_windows[i]->src.window == win->src.window){
			RGFW_windows[i] = NULL;
			break;
		}

	if (!i){
		RGFW_windows_size = 0;
		RGFW_FREE(RGFW_windows);
	}
	

	NSApplication_terminate(NSApp, (id)win->src.window);
}
#endif

#if defined(RGFW_X11) || defined(RGFW_MACOS)

#ifndef RGFW_NO_THREADS
#include <pthread.h>

RGFW_thread RGFW_createThread(void* (*function_ptr)(void*), void* args) {
	RGFW_thread t;
	pthread_create((pthread_t*)&t, NULL, *function_ptr, NULL);
	return t;
}
void RGFW_cancelThread(RGFW_thread thread) { pthread_cancel((pthread_t)thread); }
void RGFW_joinThread(RGFW_thread thread) { pthread_join((pthread_t)thread, NULL); }
#ifdef __linux__
void RGFW_setThreadPriority(RGFW_thread thread, u8 priority) { pthread_setschedprio(thread, priority); }
#endif
#endif
#endif

void RGFW_window_makeCurrent_OpenGL(RGFW_window* win) {
	assert(win != NULL);
	
	#ifdef RGFW_OPENGL
		#ifdef RGFW_X11
			glXMakeCurrent((Display *)win->src.display, (Drawable)win->src.window, (GLXContext)win->src.rSurf);
		#endif
		#ifdef RGFW_WINDOWS
			wglMakeCurrent((HDC)win->src.window, (HGLRC)win->src.rSurf);
		#endif
		#if defined(RGFW_MACOS)
		NSOpenGLContext_makeCurrentContext(win->src.rSurf);
		#endif
	#else
	#ifdef RGFW_EGL
	eglMakeCurrent(win->src.EGL_display, win->src.EGL_surface, win->src.EGL_surface, win->src.rSurf);
	#endif
	#endif

}

void RGFW_window_makeCurrent(RGFW_window* win) {
	assert(win != NULL);
	
	#if defined(RGFW_WINDOWS) && defined(RGFW_DIRECTX)
	RGFW_dxInfo.pDeviceContext->lpVtbl->OMSetRenderTargets(RGFW_dxInfo.pDeviceContext, 1, &win->src.renderTargetView, NULL);
	#endif

 	RGFW_window_makeCurrent_OpenGL(win);
}

void RGFW_window_swapInterval(RGFW_window* win, i32 swapInterval) { 
	assert(win != NULL);
	
	#ifdef RGFW_OPENGL
	#ifdef RGFW_X11
	((PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((GLubyte*)"glXSwapIntervalEXT"))((Display*)win->src.display, (Window)win->src.window, swapInterval); 
	#endif
	#ifdef RGFW_WINDOWS

	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALEXTPROC)(int interval);
	static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	static void* loadSwapFunc = (void*)1;

	if (loadSwapFunc == NULL) {
		fprintf(stderr, "wglSwapIntervalEXT not supported\n");
		win->fpsCap = (swapInterval == 1) ? 0 : swapInterval;
		return;
	}

	if (wglSwapIntervalEXT == NULL) {
		loadSwapFunc = (void*)wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)loadSwapFunc;
	}

	if (wglSwapIntervalEXT(swapInterval) == FALSE)
		fprintf(stderr, "Failed to set swap interval\n");

	#endif
	#if defined(RGFW_MACOS)
	win->src.rSurf = NSOpenGLView_openGLContext(win->src.view);
	NSOpenGLContext_setValues(win->src.rSurf, &swapInterval, NSOpenGLContextParameterSwapInterval);
	#endif
	#endif

	#ifdef RGFW_EGL
	eglSwapInterval(win->src.EGL_display, swapInterval);
	#endif

	win->fpsCap = (swapInterval == 1) ? 0 : swapInterval;

}

void RGFW_window_swapBuffers(RGFW_window* win) { 
	assert(win != NULL);
	
	win->event.frames++;
	RGFW_window_checkFPS(win);
	
	RGFW_window_makeCurrent(win);

	#ifdef RGFW_OPENGL
	#ifdef RGFW_EGL
	eglSwapBuffers(win->src.EGL_display, win->src.EGL_surface);
	#else
	#if defined(RGFW_X11) && defined(RGFW_OPENGL)
	glXSwapBuffers((Display*)win->src.display, (Window)win->src.window);
	#endif
	#ifdef RGFW_WINDOWS
	SwapBuffers((HDC)win->src.window);
	#endif
	#if defined(RGFW_MACOS)
	NSOpenGLContext_flushBuffer(win->src.rSurf);
	#endif
	#endif
	#endif


	#if defined(RGFW_WINDOWS) && defined(RGFW_DIRECTX)
	win->src.swapchain->lpVtbl->Present(win->src.swapchain, 0, 0);
	#endif
	
	/* clear the window*/


	#if defined(RGFW_OSMESA) || defined(RGFW_BUFFER)
	#ifdef RGFW_OSMESA
	u8* row = (u8*) RGFW_MALLOC(win->r.w * 3);

	i32 half_height = win->r.h / 2;
	i32 stride = win->r.w * 3;

	i32 y;
	for (y = 0; y < half_height; ++y) {
		i32 top_offset = y * stride;
		i32 bottom_offset = (win->r.h - y - 1) * stride;
		memcpy(row, win->buffer + top_offset, stride);
		memcpy(win->buffer + top_offset, win->buffer + bottom_offset, stride);
		memcpy(win->buffer + bottom_offset, row, stride);
	}

	RGFW_FREE(row);
	#endif
	
	#ifdef RGFW_X11
		win->src.bitmap->data = (char*)win->buffer;
		
		XPutImage(win->src.display, (Window)win->src.window, XDefaultGC(win->src.display, XDefaultScreen(win->src.display)), win->src.bitmap, 0, 0, 0, 0, win->r.w, win->r.h);
	#endif
	#ifdef RGFW_WINDOWS
		HGDIOBJ oldbmp = SelectObject(win->src.hdcMem, win->src.bitmap);
		BitBlt(win->src.window, 0, 0, win->r.w, win->r.h, win->src.hdcMem, 0, 0, SRCCOPY);
		SelectObject(win->src.hdcMem, oldbmp);
	#endif
	#if defined(RGFW_MACOS)
	struct CGColorSpace* colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);

	CGContextRef bitmapContext = CGBitmapContextCreate (win->buffer,
									win->r.w,
									win->r.h,
									8,
									(win->r.w * 4 * sizeof(u8)),
									colorSpace,
									kCGImageAlphaPremultipliedLast);


	NSGraphicsContext* old = NSGraphicsContext_currentContext(win->src.rSurf);

	struct CGImage* myImage = CGBitmapContextCreateImage(bitmapContext);
	
	CGContextDrawImage(win->src.rSurf, CGRectMake(0, 0, win->r.w, win->r.h), myImage);

	NSGraphicsContext_currentContext(old);
	
	CGColorSpaceRelease(colorSpace);
	CGContextRelease(bitmapContext);
	CGImageRelease(myImage);
	#endif
	#endif

	#ifdef RGFW_VULKAN
	#ifdef RGFW_PRINT_ERRORS
	fprintf(stderr, "RGFW_window_swapBuffers %s\n", "RGFW_window_swapBuffers is not yet supported for Vulkan");
	RGFW_error = 1;
	#endif
	#endif
}

void RGFW_window_maximize(RGFW_window* win) {
	assert(win != NULL);
	
	RGFW_area screen = RGFW_getScreenSize();

	RGFW_window_move(win, RGFW_VECTOR(0, 0));
	RGFW_window_resize(win, screen);
}

u8 RGFW_window_shouldClose(RGFW_window* win) {
	assert(win != NULL);
	
	/* || RGFW_isPressedI(win, RGFW_Escape) */
	return (win->event.type == RGFW_quit || RGFW_isPressedI(win, RGFW_OS_BASED_VALUE(0xff1b, 0x1B, 53)));
}

void RGFW_sleep(u32 microsecond) {
   struct timespec time;
   time.tv_sec = 0;
   time.tv_nsec = microsecond * 1000;

   nanosleep(&time , NULL);
}

void RGFW_window_checkFPS(RGFW_window* win) {
	assert(win != NULL);
	
	static float currentFrame = 0;

	win->event.fps = RGFW_getFPS();

	if (win->fpsCap == 0)
		return;

    double targetFrameTime = 1.0 / win->fpsCap;
    double elapsedTime = RGFW_getTime() - currentFrame;

    if (elapsedTime < targetFrameTime) {
		u32 sleepTime = (u32)((targetFrameTime - elapsedTime) * 1e6);	
		RGFW_sleep(sleepTime);
    }
    
    currentFrame = RGFW_getTime();
} 

u32 RGFW_getTimeNS(void) {
    struct timespec ts = { 0 };
    clock_gettime(1, &ts);
	
	return ts.tv_nsec;
}

u32 RGFW_getTime(void) {
    struct timespec ts = { 0 };
    clock_gettime(1, &ts);
	
	return ts.tv_sec;
}

u32 RGFW_getFPS(void) {
    static double previousSeconds = 0.0;
	if (previousSeconds == 0.0)
		previousSeconds = (double)RGFW_getTime();//glfwGetTime();
   
    static i16 frameCount;
    double currentSeconds = (double)RGFW_getTime();//glfwGetTime();
    double elapsedSeconds = currentSeconds - previousSeconds;

    static double fps = 0;

    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        fps = (double)frameCount / elapsedSeconds;
        frameCount = 0;
    }

    frameCount++;
    
    return (u32)fps;
}

#endif /*RGFW_IMPLEMENTATION*/

#define RGFW_Escape RGFW_OS_BASED_VALUE(0xff1b, 0x1B, 53)
#define RGFW_F1 RGFW_OS_BASED_VALUE(0xffbe, 0x70, 127)
#define RGFW_F2 RGFW_OS_BASED_VALUE(0xffbf, 0x71, 121)
#define RGFW_F3 RGFW_OS_BASED_VALUE(0xffc0, 0x72, 100)
#define RGFW_F4 RGFW_OS_BASED_VALUE(0xffc1, 0x73, 119)
#define RGFW_F5 RGFW_OS_BASED_VALUE(0xffc2 0x74, 97)
#define RGFW_F6 RGFW_OS_BASED_VALUE(0xffc3, 0x75, 98)
#define RGFW_F7 RGFW_OS_BASED_VALUE(0xffc4, 0x76, 99)
#define RGFW_F8 RGFW_OS_BASED_VALUE(0xffc5, 0x77, 101)
#define RGFW_F9 RGFW_OS_BASED_VALUE(0xffc6, 0x78, 102)
#define RGFW_F10 RGFW_OS_BASED_VALUE(0xffc7, 0x79, 110)
#define RGFW_F11 RGFW_OS_BASED_VALUE(0xffc8, 0x7A, 104)
#define RGFW_F12 RGFW_OS_BASED_VALUE(0xffc9, 0x7B, 112)
#define RGFW_F13 RGFW_OS_BASED_VALUE(0xffca 0x7C, 106)
#define RGFW_F14 RGFW_OS_BASED_VALUE(0xffcb, 0x7D, 108)
#define RGFW_F15 RGFW_OS_BASED_VALUE(0xffcc, 0x7E, 114)

#define RGFW_Backtick RGFW_OS_BASED_VALUE(96 , 192, 50)

#define RGFW_0 RGFW_OS_BASED_VALUE(0x0030, 0x30, 29)
#define RGFW_1 RGFW_OS_BASED_VALUE(0x0031, 0x31, 18)
#define RGFW_2 RGFW_OS_BASED_VALUE(0x0032, 0x32, 19)
#define RGFW_3 RGFW_OS_BASED_VALUE(0x0033, 0x33, 20)
#define RGFW_4 RGFW_OS_BASED_VALUE(0x0034, 0x34, 21)
#define RGFW_5 RGFW_OS_BASED_VALUE(0x0035, 0x35, 23)
#define RGFW_6 RGFW_OS_BASED_VALUE(0x0036, 0x36, 22)
#define RGFW_7 RGFW_OS_BASED_VALUE(0x0037, 0x37, 26)
#define RGFW_8 RGFW_OS_BASED_VALUE(0x0038, 0x38, 28)
#define RGFW_9 RGFW_OS_BASED_VALUE(0x0039, 0x39, 25)

#define RGFW_Minus RGFW_OS_BASED_VALUE(0x002d, 189, 27)
#define RGFW_Equals RGFW_OS_BASED_VALUE(0x003d, 187, 24)
#define RGFW_BackSpace RGFW_OS_BASED_VALUE(0xff08, 322, 51)
#define RGFW_Tab RGFW_OS_BASED_VALUE(0xff89, 0x09, 48)
#define RGFW_CapsLock RGFW_OS_BASED_VALUE(0xffe5, 20, 57)
#define RGFW_ShiftL RGFW_OS_BASED_VALUE(0xffe1, 0xA0, 56)
#define RGFW_ControlL RGFW_OS_BASED_VALUE(0xffe3, 0xA2, 59)
#define RGFW_AltL RGFW_OS_BASED_VALUE(0xffe9, 164, 58)
#define RGFW_SuperL RGFW_OS_BASED_VALUE(0xffeb, 0x5B, 55) 
#define RGFW_ShiftR RGFW_OS_BASED_VALUE(0xffe2, 0x5C, 56)
#define RGFW_ControlR RGFW_OS_BASED_VALUE(0xffe4, 0xA3, 59)
#define RGFW_AltR RGFW_OS_BASED_VALUE(0xffea, 165, 58)
#define RGFW_SuperR RGFW_OS_BASED_VALUE(0xffec, 0xA4, 55)
#define RGFW_Space RGFW_OS_BASED_VALUE(0x0020,  0x20, 49)

#define RGFW_A RGFW_OS_BASED_VALUE(0x0041, 0x41, 0)
#define RGFW_B RGFW_OS_BASED_VALUE(0x0042, 0x42, 11)
#define RGFW_C RGFW_OS_BASED_VALUE(0x0043, 0x43, 8)
#define RGFW_D RGFW_OS_BASED_VALUE(0x0044, 0x44, 2)
#define RGFW_E RGFW_OS_BASED_VALUE(0x0045, 0x45, 14)
#define RGFW_F RGFW_OS_BASED_VALUE(0x0046, 0x46, 3)
#define RGFW_G RGFW_OS_BASED_VALUE(0x0047, 0x47, 5)
#define RGFW_H RGFW_OS_BASED_VALUE(0x0048, 0x48, 4) 
#define RGFW_I RGFW_OS_BASED_VALUE(0x0049, 0x49, 34)
#define RGFW_J RGFW_OS_BASED_VALUE(0x004a, 0x4A, 38)
#define RGFW_K RGFW_OS_BASED_VALUE(0x004b, 0x4B, 40)
#define RGFW_L RGFW_OS_BASED_VALUE(0x004c, 0x4C, 37)
#define RGFW_M RGFW_OS_BASED_VALUE(0x004d, 0x4D, 46)
#define RGFW_N RGFW_OS_BASED_VALUE(0x004e, 0x4E, 45)
#define RGFW_O RGFW_OS_BASED_VALUE(0x004f, 0x4F, 31)
#define RGFW_P RGFW_OS_BASED_VALUE(0x0050, 0x50, 35)
#define RGFW_Q RGFW_OS_BASED_VALUE(0x0051, 0x51, 12)
#define RGFW_R RGFW_OS_BASED_VALUE(0x0052, 0x52, 15)
#define RGFW_S RGFW_OS_BASED_VALUE(0x0053, 0x53, 1)
#define RGFW_T RGFW_OS_BASED_VALUE(0x0054, 0x54, 17)
#define RGFW_U RGFW_OS_BASED_VALUE(0x0055, 0x55, 32)
#define RGFW_V RGFW_OS_BASED_VALUE(0x0056, 0x56, 9)
#define RGFW_W RGFW_OS_BASED_VALUE(0x0057, 0x57, 13)
#define RGFW_X RGFW_OS_BASED_VALUE(0x0058, 0x58, 7)
#define RGFW_Y RGFW_OS_BASED_VALUE(0x0059, 0x59, 16)
#define RGFW_Z RGFW_OS_BASED_VALUE(0x005a, 0x5A, 6)

#define RGFW_a RGFW_OS_BASED_VALUE(0x0061, 0x41, 0)
#define RGFW_b RGFW_OS_BASED_VALUE(0x0062, 0x42, 11)
#define RGFW_c RGFW_OS_BASED_VALUE(0x0063, 0x43, 8)
#define RGFW_d RGFW_OS_BASED_VALUE(0x0064, 0x44, 2)
#define RGFW_e RGFW_OS_BASED_VALUE(0x0065, 0x45, 14)
#define RGFW_f RGFW_OS_BASED_VALUE(0x0066, 0x46, 3)
#define RGFW_g RGFW_OS_BASED_VALUE(0x0067, 0x47, 5)
#define RGFW_h RGFW_OS_BASED_VALUE(0x0068, 0x48, 4)
#define RGFW_i RGFW_OS_BASED_VALUE(0x0069, 0x49, 34)
#define RGFW_j RGFW_OS_BASED_VALUE(0x006a, 0x4a, 38)
#define RGFW_k RGFW_OS_BASED_VALUE(0x006b, 0x4b, 40)
#define RGFW_l RGFW_OS_BASED_VALUE(0x006c, 0x4c, 37)
#define RGFW_m RGFW_OS_BASED_VALUE(0x006d, 0x4d, 46)
#define RGFW_n RGFW_OS_BASED_VALUE(0x006e, 0x4e, 45)
#define RGFW_o RGFW_OS_BASED_VALUE(0x006f, 0x4f, 31)
#define RGFW_p RGFW_OS_BASED_VALUE(0x0070, 0x50, 35)
#define RGFW_q RGFW_OS_BASED_VALUE(0x0071, 0x51, 12)
#define RGFW_r RGFW_OS_BASED_VALUE(0x0072, 0x52, 15)
#define RGFW_s RGFW_OS_BASED_VALUE(0x0073, 0x53, 1)
#define RGFW_t RGFW_OS_BASED_VALUE(0x0074, 0x54, 17)
#define RGFW_u RGFW_OS_BASED_VALUE(0x0075, 0x55, 32)
#define RGFW_v RGFW_OS_BASED_VALUE(0x0076, 0x56, 9)
#define RGFW_w RGFW_OS_BASED_VALUE(0x0077, 0x57, 13)
#define RGFW_x RGFW_OS_BASED_VALUE(0x0078, 0x58, 7) 
#define RGFW_y RGFW_OS_BASED_VALUE(0x0079, 0x59, 16)
#define RGFW_z RGFW_OS_BASED_VALUE(0x007a, 0x5A, 6)

#define RGFW_Period RGFW_OS_BASED_VALUE(0x002e, 190, 47)
#define RGFW_Comma RGFW_OS_BASED_VALUE(0x002c, 188, 43)
#define RGFW_Slash RGFW_OS_BASED_VALUE(0x002f, 191, 44)
#define RGFW_Bracket RGFW_OS_BASED_VALUE(0x005b, 219, 33)
#define RGFW_CloseBracket RGFW_OS_BASED_VALUE(0x005d, 221, 30) 
#define RGFW_Semicolon RGFW_OS_BASED_VALUE(0x003b, 186, 41)
#define RGFW_Return RGFW_OS_BASED_VALUE(0xff0d, 0x0D, 36) 
#define RGFW_Quote RGFW_OS_BASED_VALUE(0x0022, 222, 39)
#define RGFW_BackSlash RGFW_OS_BASED_VALUE(0x005c, 322, 42)

#define RGFW_Up RGFW_OS_BASED_VALUE(0xff52, 0x26, 126)
#define RGFW_Down RGFW_OS_BASED_VALUE(0xff54, 0x28, 125)
#define RGFW_Left RGFW_OS_BASED_VALUE(0xff51, 0x25, 123)
#define RGFW_Right RGFW_OS_BASED_VALUE(0xff53, 0x27, 124)

#define RGFW_Delete RGFW_OS_BASED_VALUE(0xffff, 0x2E, 118)
#define RGFW_Insert RGFW_OS_BASED_VALUE(0xff63, 0x2D, 115)
#define RGFW_End RGFW_OS_BASED_VALUE(0xff57, 0x23, 120)
#define RGFW_Home RGFW_OS_BASED_VALUE(0xff50, 0x24, 116) 
#define RGFW_PageUp RGFW_OS_BASED_VALUE(0xff55, 336, 117)
#define RGFW_PageDown RGFW_OS_BASED_VALUE(0xff56, 325, 122)

#define RGFW_Numlock RGFW_OS_BASED_VALUE(0xff7f, 0x90, 72)
#define RGFW_KP_Slash RGFW_OS_BASED_VALUE(0xffaf, 0x6F, 82)
#define RGFW_Multiply RGFW_OS_BASED_VALUE(0xffaa, 0x6A, 76)
#define RGFW_KP_Minus RGFW_OS_BASED_VALUE(0xffad, 0x6D, 67)
#define RGFW_KP_1 RGFW_OS_BASED_VALUE(0xffb1, 0x61, 84)
#define RGFW_KP_2 RGFW_OS_BASED_VALUE(0xffb2, 0x62, 85)
#define RGFW_KP_3 RGFW_OS_BASED_VALUE(0xffb3, 0x63, 86)
#define RGFW_KP_4 RGFW_OS_BASED_VALUE(0xffb4, 0x64, 87)
#define RGFW_KP_5 RGFW_OS_BASED_VALUE(0xffb5, 0x65, 88)
#define RGFW_KP_6 RGFW_OS_BASED_VALUE(0xffb6, 0x66, 89)
#define RGFW_KP_7 RGFW_OS_BASED_VALUE(0xffb7, 0x67, 90)
#define RGFW_KP_8 RGFW_OS_BASED_VALUE(0xffb8, 0x68, 92)
#define RGFW_KP_9 RGFW_OS_BASED_VALUE(0xffb9, 0x619, 93)
#define RGFW_KP_0 RGFW_OS_BASED_VALUE(0xffb0, 0x60, 83)
#define RGFW_KP_Period RGFW_OS_BASED_VALUE(0xffae, 0x6E, 65)
#define RGFW_KP_Return RGFW_OS_BASED_VALUE(0xff8d, 0x92, 77)

#ifdef __cplusplus
}
#endif