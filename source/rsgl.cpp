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

#ifndef RSGL_H
#include "RSGL.hpp"
#endif

#include "deps/rlgl.h"
#include "deps/RGFW.h"
#include "deps/stb_image.h"

namespace RSGL {
	RSGL::area REALcurrentWindowSize;
    
	RSGL::area* currentWindowSize() {   return &REALcurrentWindowSize;   }

    RSGL::point glVersion = {3, 3};

    int windowsOpen = 0;

    void setGLVersion(int major, int minor) { glVersion = (RSGL::point){major, minor}; }

    window::window(const char* title, RSGL::rect rect, RSGL::color c, winArgs args) {
        RGFW_setGLVersion(glVersion.x, glVersion.y);

        source = RGFW_createWindowPointer((char*)title, rect.x, rect.y, rect.w, rect.h, args);
        
        rect = (RSGL::rect){source->x, source->y, source->w, source->h}; 

        name = source->name;

        int* screenR = (int*)RGFW_getScreenSize(source);

        areaSize = (RSGL::area) {-1, -1};
        focused = false;

        if (center & args)
            rect = (RSGL::rect) {(screenR[0] - rect.w) / 2, (screenR[1] - rect.h) / 2, r.w, r.h };
        if (autoResize & args)
            areaSize = (RSGL::area) {r.w, r.h};
        if (noSwapInterval & args)
            RGFW_swapInterval(source, 0);

        this->r = rect;
        color = c;

        RGFW_makeCurrent(source);

        rlLoadExtensions((void*)RGFW_getProcAddress);
        
        rlglInit(r.w, r.h);

        rlClearColor(c.r, c.g, c.b, c.a);
        rlEnableDepthTest(); /* for 3D stuff */

        windowsOpen++;
    }

    void RSGL::window::changeIcon(const char* icon) {
        int w, h, n;
        unsigned char* data = stbi_load(icon, &w, &h, &n, 0);

        changeIcon(data, w, h, n);
        free(data);
    }

    void RSGL::window::changeIcon(unsigned char* icon, int w, int h, int channels) { RGFW_setIcon(source, icon, w, h, channels); }


    RSGL::event RSGL::window::checkEvents(bool setCurrentDrawingContext) {
        source->x = r.x;
        source->y = r.y;
        source->w = r.w;
        source->h = r.h;
        source->name = (char*)name;
        source->fpsCap = fpsCap;

        RGFW_checkEvents(source);

        fps = source->fps;
        focused = source->inFocus;
        r = (RSGL::rect) {source->x, source->y, source->w, source->h};
        
        event = *(RSGL::event*)&source->event; 

        if (source->event.droppedFilesCount)
            event.type = RSGL::dnd;

        if (setCurrentDrawingContext) {
            REALcurrentWindowSize = (area){r.w, r.h};
            // make this window the current glx context
            RGFW_makeCurrent(source);
        }

        if (areaSize.w != -1 || areaSize.h != -1)
            rlViewport(0, 0, r.w, r.h);

        return event;
    }

    void RSGL::window::clear() {
        rlClearScreenBuffers();
        rlDrawRenderBatchActive();
        
        RGFW_swapBuffers(source);

        rlClearColor(color.r, color.g, color.b, color.a);   
        glFlush();
    }

    bool RSGL::window::isPressed(const char* key, bool onFocus /*=false*/) { return RGFW_isPressedS(onFocus ? source : NULL, (char*)key); }
    bool RSGL::window::isPressed(unsigned int key, bool onFocus /*=false*/) { return RGFW_isPressedI(onFocus ? source : NULL, key); }

    void RSGL::window::registerJoystick(int joyStickNum) {  RGFW_registerJoystick(source, joyStickNum); }

    bool RSGL::window::isPressedJS(unsigned short controller, unsigned char button) { return RGFW_isPressedJS(source, controller, button); }

    RSGL::rect RSGL::window::fullscreen() {
        int* screenR = (int*)RGFW_getScreenSize(source);

        return (RSGL::rect) {0, 0, screenR[0], screenR[1]};
    }

    RSGL::point RSGL::window::globalMouse() {
        int* point = RGFW_getGlobalMousePoint(source);

        return (RSGL::point) {point[0], point[1]};
    }

    const char* RSGL::window::readClipboard() { return RGFW_readClipboard(source); }
    void RSGL::window::writeClipboard(const char* str) { RGFW_writeClipboard(source, (char*)str); }

    bool RSGL::window::isOpen(bool autoClear, bool autoClose) {
        if (autoClear)
            clear();

        return (checkEvents().type != RSGL::quit);
    }

    void RSGL::window::close() { this->~window(); }

    RSGL::window::~window() { 
        if (source != NULL) {
            if (windowsOpen == 1)
                rlglClose();
            RGFW_closeWindow(source);

            windowsOpen--;
        }

        source = NULL;
    }
    
    /* other */

    /*! NOTE! (for X11/linux) : if you define a window in a thread, it must be run after the original thread's window is created or else there will be a memory error */
    thread createThread(void* (*function_ptr)(void*), void* args) { return RGFW_createThread(function_ptr, args); } /*!< create a thread*/
    void cancelThread(thread thread) { RGFW_cancelThread(thread); }  /*!< cancels a thread*/
    void joinThread(thread thread) { RGFW_joinThread(thread); } /*!< join thread to current thread */
    void setThreadPriority(thread thread, unsigned char priority) { RGFW_setThreadPriority(thread, priority); } /*!< sets the priority priority  */


    bool timerT(int ticks){
        static int i = 0, tickLimit = 0;
        
        if (ticks > tickLimit)
            tickLimit = ticks;

        if (i > tickLimit)
            i = 0;

        i++;

        return !(i % ticks);
    }

    bool timerM(int milliseconds) {
        static int start_time = time(0), miliLimit = 0;

        if (milliseconds > miliLimit)
            miliLimit = milliseconds;

        int passed = time(0) - start_time;

        if (passed > miliLimit)
            start_time = time(0);

        return !(passed % milliseconds) && passed;
    }

    char keyStrToChar(const char* keyName, bool enterNL /* = true */) {
        if (strlen(keyName) <= 1)
            return  keyName[0];

        if (!strcmp(keyName, "asciitilde")) return '`';
        else if (!strcmp(keyName, "grave")) return '~';
        else if (!strcmp(keyName, "exclam")) return '!';
        else if (!strcmp(keyName, "at")) return '@';
        else if (!strcmp(keyName, "numbersign")) return '#';
        else if (!strcmp(keyName, "dollar")) return '$';
        else if (!strcmp(keyName, "percent")) return '%';
        else if (!strcmp(keyName, "asciicircum")) return '^';
        else if (!strcmp(keyName, "ampersand")) return '&';
        else if (!strcmp(keyName, "asterisk")) return '*';    
        else if (!strcmp(keyName, "parenleft")) return '(';
        else if (!strcmp(keyName, "parenright"))  return ')';

        else if (!strcmp(keyName, "underscore"))  return '_';
        else if (!strcmp(keyName, "minus"))  return '-';
        else if (!strcmp(keyName, "plus"))  return '+';
        else if (!strcmp(keyName, "equal"))  return '=';

        else if (!strcmp(keyName, "braceleft")) return '{';
        else if (!strcmp(keyName, "bracketleft")) return '[';
        else if (!strcmp(keyName, "bracketright")) return ']';
        else if (!strcmp(keyName, "braceright")) return '}';

        else if (!strcmp(keyName, "colon")) return ':';
        else if (!strcmp(keyName, "semicolon")) return ';';
        else if (!strcmp(keyName, "quotedbl")) return '"';
        else if (!strcmp(keyName, "apostrophe")) return '\'';
        else if (!strcmp(keyName, "bar")) return '|';
        else if (!strcmp(keyName, "backslash")) return '\\';

        else if (!strcmp(keyName, "less")) return '<';
        else if (!strcmp(keyName, "comma")) return ',';
        else if (!strcmp(keyName, "greater")) return '>';
        else if (!strcmp(keyName, "period")) return '.';
        else if (!strcmp(keyName, "question")) return '?';
        else if (!strcmp(keyName, "slash")) return '/';
        
        else if (!strcmp(keyName, "space")) return ' ';
        else if (!strcmp(keyName, "Return") && enterNL) return '\n';

        /* Tab and BackSpace are not handled */

        return '\0';
    }
};