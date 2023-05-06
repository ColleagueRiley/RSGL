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
 * appreciated but is not require((RGFW_window*)d)->
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#define RGFW_IMPLEMENTATION

#ifndef RSGL_IMPLEMENTATION
#endif

#define GL_GLEXT_PROTOTYPES

#ifdef _WIN32
#ifdef __linux__
#undef __linux__ // for cross compiling
#endif
#endif

#define RGFW_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define RLGL_IMPLEMENTATION


#include "deps/rlgl.h"

#if defined(RSGL_PRELOAD_X11) && (!defined(__unix__) || defined(_WIN32))
#undef RSGL_PRELOAD_X11
#endif

#ifdef RSGL_PRELOAD_X11

#define XDL_IMPLEMENTATION
#define XDL_NO_DEALLOCATE

#include "deps/XDL.h"

XDLModule RSGL_X11module = NULL;
#endif

#ifndef RSGL_H
#include "../RSGL.hpp"
#endif

#include "deps/stb_image.h"
#include "deps/RGFW.h"

namespace RSGL {
	// the root grab function

	int frames = 0;
	int prevTime = time(0);

	RSGL::area REALcurrentWindowSize;
    
	RSGL::area* currentWindowSize() {  
        return &REALcurrentWindowSize; 
    }
};

int windowsOpen = 0;

RSGL::window::window(RSGL::string name, RSGL::rect rect, RSGL::color c, winArgs args) {
	#ifdef RSGL_PRELOAD_X11
	if (RSGL_X11module == NULL) 
		RSGL_X11module = XDL_init();
	#endif

	unsigned long RGFWargs = 0;

    if (!args.resize)
        RGFWargs |= RGFW_NO_RESIZE;
    if (args.noBorder)
        RGFWargs |= RGFW_NO_BORDER;
    if (args.rgbaWindow)
        RGFWargs |= RGFW_TRANSPARENT_WINDOW;

    RGFW_setGLVersion(3, 3);

    source = RGFW_createWindowPointer(name.src, rect.x, rect.y, rect.w, rect.h, RGFWargs);

    int* screenR = (int*)RGFW_getScreenSize(source);

    if (args.fullscreen)
        r = (RSGL::rect) {0, 0, screenR[0], screenR[1] };
    if (args.center)
        r = (RSGL::rect) {((r.x + screenR[0]) / 2), ((r.y + screenR[0]) / 2), r.w, r.h };

    if (args.icon.data != NULL)
        RGFW_setIcon(source, args.icon.data, args.icon.size.w, args.icon.size.h, args.icon.n);

    if (args.icon.file.size())
        changeIcon(args.icon.file);

    if (args.autoResize)
        areaSize = (RSGL::area) {r.w, r.h};

    this->r = rect;
    color = c;
    name = name;

    RGFW_makeCurrent(source);

    rlLoadExtensions((void*)RGFW_getProcAddress);
    
    rlglInit(r.w, r.h);

    rlClearColor(245, 245, 245, 255);
    rlEnableDepthTest();

	windowsOpen++;
}

void RSGL::window::changeIcon(string icon) {
    int w, h, n;
    unsigned char* data = stbi_load(icon.src, &w, &h, &n, 0);

    changeIcon(data, w, h, n);

    free(data);
}

void RSGL::window::changeIcon(unsigned char* icon, int w, int h, int channels) { RGFW_setIcon(source, icon, w, h, channels); }


RSGL::event RSGL::window::checkEvents(bool setCurrentDrawingContext) {
    source->x = r.x;
    source->y = r.y;
    source->w = r.w;
    source->h = r.h;
	source->fpsCap = fpsCap;

    RGFW_checkEvents(source);

    r = (RSGL::rect) {source->x, source->y, source->w, source->h};
    
    event = *(RSGL::event*)&source->event; 

	if (setCurrentDrawingContext) {
		REALcurrentWindowSize = (area){r.w, r.h};
		// make this window the current glx context
		RGFW_makeCurrent(source);
	}

	//rlViewport(0, 0, r.w, r.h);

	return event;
}

void RSGL::window::clear() {
	rlClearScreenBuffers();
	rlDrawRenderBatchActive();
	
	RGFW_swapBuffers(source);

    rlClearColor(color.r, color.g, color.b, color.a);   
	glFlush();
}

bool RSGL::window::isPressed(RSGL::string key, bool onFocus /*=false*/) { return RGFW_isPressedS(onFocus ? source : NULL, key.src); }
bool RSGL::window::isPressed(unsigned int key, bool onFocus /*=false*/) { return RGFW_isPressedI(onFocus ? source : NULL, key); }

bool RSGL::window::isPressed(vector<unsigned int> key, bool onFocus /*=false*/, bool OR /*=false*/) { 
	size_t size = sizeof(key)/sizeof(unsigned int);

	for (unsigned int i = 0; i < size; i++) {
		if (OR && isPressed(key[i], onFocus))
			return true;
		else if (!OR && !isPressed(key[i], onFocus)) /* AND */
			return false;
	}

	return !OR; /* if it's OR, nothing passsed, if it's AND, everything passed */
}

bool RSGL::window::isPressed(RSGL::vector<string> key, bool onFocus /*=false*/, bool OR /*=false*/) { 
	size_t size = sizeof(key)/sizeof(char*);
	vector<unsigned int> keyS;

	for (unsigned int i = 0; i < size; i++)
		keyS.push_back(RGFW_keyStrToKeyCode(key[i].src));  

	return isPressed(keyS, onFocus, OR);
}

RSGL::rect RSGL::window::fullscreen() {
    int* screenR = (int*)RGFW_getScreenSize(source);

	return (RSGL::rect) {0, 0, screenR[0], screenR[1]};
}

RSGL::point RSGL::window::globalMouse() {
	int* point = RGFW_getGlobalMousePoint(source);

	return (RSGL::point) {point[0], point[1]};
}

bool RSGL::window::isOpen(bool autoClear, bool autoClose) {
	if (autoClear)
		clear();

	if (checkEvents().type != RSGL::quit)
		return true;

	else {
		if (autoClose)
			close();
		return false;
	}
}

void RSGL::window::close() {
    rlglClose();
    RGFW_closeWindow(source);

	#ifdef RSGL_PRELOAD_X11
	if (!windowsOpen)
		XDL_close(RSGL_X11module);
	#endif
}

RSGL::window::~window() { close(); }