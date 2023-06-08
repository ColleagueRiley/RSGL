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
DEFINES

VERSIONING (RSGL::setGLVersion should also be run to load that version with your opengl loader (GLX/WGL/NGL))
GRAPHICS_API_OPENGL_11 (not supported)
GRAPHICS_API_OPENGL_21
GRAPHICS_API_OPENGL_33
GRAPHICS_API_OPENGL_43
GRAPHICS_API_OPENGL_ES2

RSGL_IMPLEMENTATION - includes the source headers at the end of the file
*/

#ifndef RSGL_H
struct RGFW_window;
struct FONScontext;

namespace RSGL  {
    //! Event constants
    /** \addtogroup Events
     *  @{
     */
    enum events {
        keyPressed = 2, /*!< a key has been pressed*/
        keyReleased = 3, /*!< a key has been released*/
        mouseButtonPressed = 4, /*!< a mouse button has been pressed (left,middle,right)*/
        mouseButtonReleased = 5, /*!< a mouse button has been released (left,middle,right)*/
        mousePosChanged = 6, /*!< the position of the mouse has been changed*/
        jsButtonPressed = 7, /*!< a joystick button was pressed */
        jsButtonReleased = 8, /*!< a joystick button was released */
        jsAxisMove = 9, /*!< an axis of a joystick was moved*/
        quit = 33, /*!< the user clicked the quit button*/
        dnd = 34 /*!< a file has been dropped into the window*/
    };
    /** @}*/

    //! mouse constants : win.event.button
    /** \addtogroup mouse-constants
     *  @{
     */
    enum mouse_buttons {
        mouseLeft = 1,       //!< left mouse button is pressed
        mouseMiddle = 2,     //!< mouse-wheel-button is pressed
        mouseRight = 3,      //!< right mouse button is pressed
        mouseScrollUp = 4,   //!< mouse wheel is scrolling up
        mouseScrollDown = 5 //!< mouse wheel is scrolling down
    };
/** @}*/

    /*! joystick button codes (based on xbox/playstation), you may need to change these values per controller */
    enum joystick_constants {
        JS_A = 0, /* or PS X button */
        JS_B = 1, /* or PS circle button */
        JS_Y = 2, /* or PS triangle button */
        JS_X = 3, /* or PS square button */
        JS_START = 9, /* start button */
        JS_SELECT = 8, /* select button */
        JS_HOME = 10, /* home button */
        JS_UP = 13, /* dpad up */
        JS_DOWN = 14, /* dpad down*/
        JS_LEFT = 15, /* dpad left */
        JS_RIGHT = 16, /* dpad right */
        JS_L1 = 4, /* left bump */
        JS_L2 = 5, /* left trigger*/
        JS_R1 = 6, /* right bumper */
        JS_R2 = 7 /* right trigger */
    };

    //! shape/draw structures
    /** \addtogroup shape-structs
    *  @{
    */

    struct point { int x, y; };
    struct area { int w, h; };
    struct rect { int x, y, w, h; };
    struct circle { int x, y, d; }; //!< d = diameter of the circle (radius * 2)

    struct triangle {
        RSGL::point p1, p2, p3;

        RSGL::point& operator[](int index) { return (index == 0) ? p1 : (index == 1) ? p2 : p3; }
    };

    typedef rect oval; //!< oval is the same thing as a rect struct

    struct color {  
        unsigned char r, g, b, a; 

        /* mainly for c++98, allow alpha to be 255 by default */
        color(){ } 
        color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) { this->r = r;      this->g = g;     this->b = b;      this->a = a;    }
    }; //!< color structure

    struct bitmap {
        char* file;          //!< source file (Images only)
        RSGL::rect r;              //!< drawing rect size
        unsigned char* data;        //!< bitmap data
        unsigned int tex; //!< opengl texture and oval texture for adding textures to smooth ovals
        RSGL::area size;           //!< bitmap size
        char channels;                     // bitmap channels

        bitmap() { data = 0; tex = -1; } /* for c++98 */
        ~bitmap(); /* auto free bitmap */
    }; //!< structure for bitmaps and images

    typedef bitmap image; //!< the image struct is the same thing as a bitmap struct

    //!< font stucutre for font info
	struct font{
        char* file; //!< font file

        int size; //!< size of the font
        int fonsFont; //!< source fons font
        
        FONScontext* ctx = (FONScontext*)0; /* fons font context */ 

        font(){ size = 0; ctx = (FONScontext*)0; } /* for c++98 */
        //!< functions for loading a font, with a string/data
        font(const char* File, int Size);  //!< load font
        font(const char* File){ file = (char*)File; font(); }  //!< set font with char*
    
        ~font(); /* free fonsFont */
    }; //!< font structure that can generally be used for a place-holder for std::string
/** @}*/

//!< window data structs/functions
/** \addtogroup Window_Data_and_Managment
 *  @{
 */
    struct event {
        unsigned char type; /*!< which event has been sen t?*/
        unsigned char button; /*!< which mouse button has been clicked (0) left (1) middle (2) right OR which joystick button was pressed OR which joystick axis was moved*/
        point mouse; /*!< mouse x, y of event or Drop point*/

        unsigned char ledState; /*!< 0 : numlock, 1 : caps lock, 3 : small lock*/

        unsigned keyCode; /*!< keycode of event*/

        char* keyName; /*!< key name of event */

        /*! drag and drop data */
        int droppedFilesCount; /*!< house many files were dropped */
        const char** droppedFiles; /*!< dropped files*/

        /*! joystick*/
        unsigned short joystick; /* which joystick this event applies to (if applicable to any) */
        char axis[4][16]; /*[4][2] array x, y of axises (-100 to 100) */
    };                                                       //!< event structure to handle events sent to a specific drawable

    //! Window structure

    /* this must be run before the window is created or the window will use whatever the last-set (or default) version is */
    void setGLVersion(int major, int minor);

    /* extra window arguments */
    typedef unsigned int winArgs;

    enum winArgConsts {
        noBorder		= (1L<<3), /*!< If the window doesn't have border*/
        noResize		= (1L<<4), /*!< If the window cannot be resized  by the user*/
        allowDnd     = (1L<<5), /*!< if the window supports drag and drop*/
        hideMouse = (1L<<6), /* if the window should hide the mouse or not (can be toggled later on)*/
        fullscreen = (1L<<8), /* if the window should be fullscreen by default or not */
        center = (1L<<10),
        autoResize = (1L<<11),
        noSwapInterval = (1L<<12)
    };

    struct window {
        char* name;  //!< the name of the window
        RSGL::event event; //!< the source event struct
        RSGL::color color; //!< the background color
        RSGL::rect r;      //!< the size/x/y of the window

        RSGL::area areaSize; //!< for autoresize

        RGFW_window* source; //!< source window obj
        int tag;

        int GPU;                //!< the GPU option selection
        int fpsCap;
        unsigned int fps; 
        unsigned int sourceVBO; //!< source vbo object
        bool vbo;               //!< if vbo is supported or not
        bool focused;   //!< if the window is focused or not (updated by check events)

        RSGL::event checkEvents(bool setcurrentWindowSize = true); //!< checks if any events have been sent (is required to get events), returns an optional event struct as an alternative to win.event, this is mainly to pass the function to another function
        bool isPressed(unsigned int keyCode, bool onFocus = false);                              //!< checks if a key has been pressed (works with key code or const char*)
        bool isPressed(const char* keyName, bool onFocus = false);                      //!< checks if a key has been pressed (works with key code or const char*)
        
        void registerJoystick(int joyStickNum);
        bool isPressedJS(unsigned short controller, unsigned char button);
                
        void close();                                                                   //!< closes the window
        void clear();                                                                   //!< clears the display
        bool isOpen(bool autoClear = true, bool autoClose = true);    //!< clears the window (toggleable), returns true if the window hasn't been closed, if the window is cloesd (and autoClose is true), RSGL::window::close() is called
        void changeIcon(const char* icon);
        void changeIcon(unsigned char* icon, int w, int h, int channels);

        const char* readClipboard();
        void writeClipboard(const char*);

        /* global data */
        RSGL::point globalMouse(); //!< returns mouse x/y in the context of the screen
        RSGL::rect fullscreen();   //!< returns the size of the screen

        window() { }
        window(
            const char* /*the name*/,
            RSGL::rect /*the rect/size/x/y of the window*/,
            RSGL::color /*the background color*/,
            winArgs args = 0 /* other args [from winArgConsts] */
        );                    //!< inits the window with these values

        ~window();
    };

    RSGL::area* currentWindowSize(); //!< the current drawing context

    /* 
        converts a key string to a char (if the keyStr is only a char by default it just returns keyName[0]) 
        returns 0 if the key is invalid or unhandled
        "BackSpace" and "Tab" are both unhandled
    */
    char keyStrToChar(const char* keyName, bool enterNL = true /*if "Return" should be handled as \n*/);

/** @}*/

    //!< collition functions
    /** \addtogroup collision-functions
     *  @{
     */
    int circleCollidePoint(RSGL::circle c, RSGL::point p);                 //!< if a circle collides with a point
    int circleCollideRect(RSGL::circle c, RSGL::rect r);                   //!< if a circle collides with a rect
    int circleCollide(RSGL::circle cir1, RSGL::circle cir2);               //!< if a circle collides with another circle
    int circleCollideLine(RSGL::circle c, RSGL::point p1, RSGL::point p2); //!< if a circle collides with a line
    int rectCollidePoint(RSGL::rect r, RSGL::point p);                     //!< if a rect collides with a point
    int rectCollideRect(RSGL::rect r, RSGL::rect r2);                      //!< if a rect collides with another rect
    int pointCollide(RSGL::point p, RSGL::point p2);                       //!< if a point collides with another point
    /** @}*/

    void freeDraw();

    struct gradient {
        RSGL::color color0, color1, color2, color3;

        int size;
        
        gradient() { size = 0; }
        gradient(RSGL::color c) { color0 = c; size = 1;}
        gradient(RSGL::color c, RSGL::color c1) { color0 = c; color1 = c1; size = 2;}
        gradient(RSGL::color c, RSGL::color c1, RSGL::color c2) { color0 = c; color1 = c1; color2 = c2; size = 3;}
        gradient(RSGL::color c, RSGL::color c1, RSGL::color c2, RSGL::color c3) { color0 = c; color1 = c1; color2 = c2; color3 = c3; size = 3;}
    };

    //!< drawing functions

    struct drawArgs {
        #if __cplusplus >= 201103L  /* C++98 REALLY doesn't like this  */
        //! general
        bool fill = true;                            //!< not for text
        float rotationAngle = 0;                     //!< the angle it's rotated to
        float flipHorizontal = 0;                    //!< how much it's flipped horizontally
        float flipVertical = 0;                      //!< how much it's flipped vertically
        float lineWidth = 0;                         //!< the width of the line
        int texture = -1;                            //!< draw a texture on it (from load/draw image/bitmap functions)
        int glTexture = -1; /* actual opengl texture, (not RSGL texture from RSGL::tex) */
        bool preLoad = true; /* load bitmap from RSGL::tex if it's already loaded (might cause issues with simular bitmaps, toggleable for if you want to manage it yourself) */
        float texOffX = 0; 
        float texOffY = 0; /* texture offset */
        RSGL::gradient gradient = (RSGL::gradient){ };      //!< gradient to draw on the shapes
        RSGL::area windowSize = *currentWindowSize(); //!< window to write on
        //!< rect-only
        bool rounded = false;
        RSGL::point roundPoint = (RSGL::point){10, 10}; //!< where the rounding is : 10, 10 is good for a round rect, 24, 24 is a bean shape, 204, 204 is an oval
        //!< polygon / circle only
        RSGL::point arc = (RSGL::point){0, -1}; /* arc open/close (0 - 360 (circles)), 0, -1 = default */
        /*!< text only */
        float spacing = 0; /* oh far letters are spaced out */
        float blur = 0; /* blur effect on text */ 
        #else /* c++98 doesn't mind this */
        bool fill, preLoad, rounded;
        float rotationAngle, flipHorizontal, flipVertical,lineWidth, spacing, blur, texOffX, texOffY;
        int texture, glTexture;
        RSGL::gradient gradient;
        RSGL::area windowSize;
        RSGL::point roundPoint, arc;

        drawArgs() { /* this is compressed because this information is already included + it just adds extra text */
            fill = true;          rotationAngle = 0;            flipHorizontal = 0;          flipVertical = 0;         lineWidth = 0;       texture = -1;          glTexture = -1;              preLoad = true;     texOffX = 0;   texOffY = 0;         gradient = RSGL::gradient();            windowSize = *currentWindowSize();        rounded = false;          roundPoint = (RSGL::point){10, 10};              arc =  (RSGL::point){0, -1};          spacing = 0;           blur = 0;
        }
        #endif
    };                       //!< general args for drawing

    /** \addtogroup drawing-functions
     *  @{
     */

    void loadFont(RSGL::font font, int size);

    void drawText(
        const char* text,  //!< the text to draw
        RSGL::circle c,    //!< the x, y and size the text
        RSGL::color col,   //!< the color
        RSGL::font font,   //!< the font info
        drawArgs args = drawArgs() //!< extra args
    );                     //!< function for drawing text

    int textWidth(const char* text, RSGL::font font, int size);

    void drawPoint(
        RSGL::point p,     //!< the point to be draw
        RSGL::color c,     //!< the color of the point
        drawArgs args = drawArgs() //!< extra args
    );                     //!< draws a point on the screen

    void drawRect(
        RSGL::rect r /*the rect to draw*/,
        color c /*the color rect*/,
        drawArgs args = drawArgs() //!< extra args
    );                     //!< draws a rect on the window

    void drawLine(
        RSGL::point p1,    //!< the first point to draw
        RSGL::point p2,    //!< the second point (where to stop)
        RSGL::color c,     //!< the color of the line
        drawArgs args = drawArgs() //!< extra args
    );                     //!< draws a cirlce on the screen

    void drawCircle(
        RSGL::circle c,    //!< the circle to draw
        RSGL::color col,   //!< the color of the circle
        drawArgs args = drawArgs() //!< extra args
    );                     //!< draws a cirlce on the screen

    void drawTriangle(
        RSGL::triangle t,  //!< triangle struct
        RSGL::color c,     //!< color of the triangle
        drawArgs args = drawArgs() //!< extra args
    );                     //!< draw triangle

    void drawOval(
        RSGL::oval o, //!< the rect shape to draw
        color c /*the color rect*/,
        drawArgs args = drawArgs() //!< extra args
    );                     //!< draw oval

    void drawPolygon(
        RSGL::oval o,            //!< x/y/size to draw
        RSGL::color color,       //!< color to draw
        int sides,               //!< amount of sides in the polygon
        RSGL::drawArgs args = drawArgs() //!< extra argss
    );                           //!< draw polygon basd on given side count

    int drawImage(
        const char* fileName /*the file to draw*/,
        RSGL::rect r /*the width/heigth/x/y to put the image*/,
        drawArgs args = drawArgs()                            //!< extra args
    );                                                //!< draws a image on the screen (returns the index texture in the texture buffer)
    int drawImage(RSGL::image r, drawArgs args = drawArgs()); //!< draw image on screen based on image struct (returns the index texture in the texture buffer)
    
    int loadImage(const char* file);
    int loadBitmap(unsigned char *data, int channels, RSGL::area memsize); //!< load a bitmap into the texture buffer (no drawing) (returns the index texture in the texture buffer)

    int drawBitmap(
        unsigned char *data,                      //!< 1D bitmap
        RSGL::rect r,                            //!< size to draw
        char channels,                            //!< channels in data
        RSGL::area memsize,                      //!< size of data
        RSGL::drawArgs args = drawArgs(),                //!< optional args for drawing
        RSGL::color color = RSGL::color(255, 255, 255, 255) //!< optional forced color
    );                                           //!< draw a bitmap on the screen (returns the index texture in the texture buffer)
/** @}*/

#ifndef RSGL_NO_AUDIO

    struct audioData;

    struct audio {
        char* file;
        bool loop;
        
        audioData* data;

        void play(const char* file);
        void play();
        void pause();
        void stop();
        void seek(int pos);
        void setVolume(int vol);
        int length();

        int volume();
        unsigned int position();
        bool isPlaying();

        audio() {
            file = 0;
            loop = false;
        } /* for c++98 */
        ~audio(); /* free audioData's init malloc */
    };
#endif
    /** @}*/

#ifndef RSGL_NO_WIDGETS
    enum buttonEvent { hovered, held, released, none };

    struct button {
        RSGL::rect r;
        buttonEvent event;

        button(){ event = none; }
        button(RSGL::rect rect){ r = rect;          event = none; }

        buttonEvent& checkEvent(RSGL::event e);
        void draw(RSGL::color c, drawArgs d = drawArgs());
    };

    struct slider {
        RSGL::rect bar;
        RSGL::button tick;

        int checkEvent(RSGL::event e);
        void draw(RSGL::color c, RSGL::color c1, drawArgs d = drawArgs(), drawArgs d2 = drawArgs()); 
    };

    struct textbox {
        RSGL::button box; /* text box */
        char* text; /* source text */
        unsigned long textSize;
        
        RSGL::point p;
        bool inUse;

        textbox();
        textbox(RSGL::rect rect) { this->textSize = 0; box.r = rect; inUse = false; }
        ~textbox();

        void addChar(char ch, __SIZE_TYPE__ index);
        void eraseChar(__SIZE_TYPE__ index);
        void checkEvent(
                RSGL::event e, 
                bool tab = true, /* if tabs are handled (3 spaces) */
                bool enterNL = true /* if enter/return should be handled as a newline */
        ); /* fills in `text` when a key is pressed */

        /*  cir.x/cir.y offsets box.x/box.y, 
            if (cir.d == 0) font size is based on the box size + text length 
        */
        void draw(RSGL::circle cir, RSGL::font font, RSGL::color col, drawArgs args = drawArgs()); /* draws text */
    };
#endif

    /* 
        menus : just use an array of RSGL::button(s) 
    */

    bool timerT(int ticks);       //!< return true after tick amount of time has passed
    bool timerM(int miliseconds); //!< return true after milisecond amount of time has passed

    typedef unsigned long thread;

    thread createThread(void* (*function_ptr)(void*), void* args);
    void cancelThread(thread thread);
    void joinThread(thread thread);
    void setThreadPriority(thread thread, unsigned char priority);
};

#endif /*RSGL_H*/

#define RSGL_H
#define RSGL_SILICON

#ifdef RSGL_IMPLEMENTATION
#include "source/rsgl.cpp"
#include "source/draw.cpp"
#include "source/collide.cpp"
#ifndef RSGL_NO_WIDGETS
#include "source/widgets.cpp"
#endif

#ifndef RSGL_NO_AUDIO
#include "source/audio.cpp"
#endif

#endif