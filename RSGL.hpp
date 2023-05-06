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

#if defined(RGFW_CPP_STD) || defined(_GLIBCXX_VECTOR) /* use std::vector if it's asked for or if it's already in use anyway */
#include <string>
#include <vector>
#else

#include <stdlib.h>

#ifndef _INITIALIZER_LIST /* don't use the slim version if the normal version is already in use*/
namespace std {
  template<class type>
    struct initializer_list {
      type* src;
      size_t srcSize;

      constexpr initializer_list(const type __a, size_t __l) : src(__a),  srcSize(__l) { }
      constexpr initializer_list() noexcept : src(0),  srcSize(0) { }
    };
}
#endif /* by default use (a super slim version of) initializer_list + our custom vector type to avoid c++ stl bloat */
#endif


#ifndef RSGL_H

struct RGFW_window;

namespace RSGL  {
    #ifndef RGFW_CPP_STD
    template<typename type>
    struct vector {
        type* src; /* source c-array */
        size_t srcSize; /* size of the c-array */

        void push_back(type data);

        void push_back(vector<type> data);

        void insert(int index, type data);

        void insert(int begin, vector<type> data);

        void erase(int index);

        /* for backwards compatibility */
        size_t size() { return srcSize; } 
        type& at(int index) { return src[index]; }
        type& back(){ return src[size() - 1]; }
        int begin(){ return 0; } /* filler */

        vector() {}

        vector(type* data, size_t size);

        vector(const char* string);

        type& operator[](int index){ return at(index); }

        ~vector() {  if (size())       free(src);  }

        vector(std::initializer_list<type> list);
        bool operator==(vector<type> vector2);
    };

    typedef vector<char> string;
    #else
    using std::string;
    using std::vector;
    #endif

    //! Event constants
    /** \addtogroup Events
     *  @{
     */
    unsigned char keyPressed = 2; /*!< a key has been pressed*/
    unsigned char keyReleased = 3; /*!< a key has been released*/
    unsigned char mouseButtonPressed = 4; /*!< a mouse button has been pressed (left,middle,right)*/
    unsigned char mouseButtonReleased = 5; /*!< a mouse button has been released (left,middle,right)*/
    unsigned char mousePosChanged = 6; /*!< the position of the mouse has been changed*/
    unsigned char jsButtonPressed = 7; /*!< a joystick button was pressed */
    unsigned char jsButtonReleased = 8; /*!< a joystick button was released */
    unsigned char jsAxisMove = 9; /*!< an axis of a joystick was moved*/
    unsigned char quit = 33; /*!< the user clicked the quit button*/
    unsigned char dnd = 34; /*!< a file has been dropped into the window*/
    /** @}*/

    //! mouse constants : win.event.button
    /** \addtogroup mouse-constants
     *  @{
     */
    const int mouseLeft = 1;       //!< left mouse button is pressed
    const int mouseMiddle = 2;     //!< mouse-wheel-button is pressed
    const int mouseRight = 3;      //!< right mouse button is pressed
    const int mouseScrollUp = 4;   //!< mouse wheel is scrolling up
    const int mouseScrollDown = 5; //!< mouse wheel is scrolling down
/** @}*/

    /*! joystick button codes (based on xbox/playstation), you may need to change these values per controller */
    unsigned char JS_A = 0; /* or PS X button */
    unsigned char JS_B = 1; /* or PS circle button */
    unsigned char JS_Y = 2; /* or PS triangle button */
    unsigned char JS_X = 3; /* or PS square button */
    unsigned char JS_START = 9; /* start button */
    unsigned char JS_SELECT = 8; /* select button */
    unsigned char JS_HOME = 10; /* home button */
    unsigned char JS_UP = 13; /* dpad up */
    unsigned char JS_DOWN = 14; /* dpad down*/
    unsigned char JS_LEFT = 15; /* dpad left */
    unsigned char JS_RIGHT = 16; /* dpad right */
    unsigned char JS_L1 = 4; /* left bump */
    unsigned char JS_L2 = 5; /* left trigger*/
    unsigned char JS_R1 = 6; /* right bumper */
    unsigned char JS_R2 = 7; /* right trigger */

    //! shape/draw structures
    /** \addtogroup shape-structs
    *  @{
    */

    struct point {
        bool operator==(point p) { return x == p.x && y == p.y; } //!< if one point == another point (same values)
        bool operator!=(point p) { return !(p == *this); } //!< if one point != another point (diff values)
        void operator+=(point p) {  x += p.x;   y += p.y;    }
        void operator-=(point p) {  x -= p.x;     y -= p.y;  }

        int x /*!< x*/, y /*!< y*/;
    }; //!< a single points

    struct area {
        bool operator==(area a) { return w == a.w && h == a.h; } //!< if one area == another area (same values)
        bool operator!=(area a) { return w != a.w && h != a.h; } //!< if one area != another area (!same values)
        void operator+=(area a){  w += a.w;      h += a.h; }
        void operator-=(area a){  w -= a.w;      h -= a.h; }

        int w /*!< w*/, h /*!< h*/;
    }; //!< an width / height area

    struct rect : point, area {
        bool operator==(rect r) { return x == r.x && y == r.y && w == r.w && h == r.h; } //!< if one rect == another rect (same values)
        bool operator!=(rect r) { return !(r == *this); }                                //!< if one rect != another rect (!same values)

        rect() {}
        rect(int X, int Y, int W, int H) {    x = X;     y = Y;     w = W;      h = H;     } //!< load with just ints
    };                 //!< Rect is RSGL::point combined with RSGL::area
    typedef rect oval; //!< oval is the same thing as a rect struct

    struct circle : point {                                                                          //!< circle as a child of point
        int d; //!< diameter of the circle (radius * 2)

        bool operator==(circle c) { return x == c.x && y == c.y && d == c.d; } //!< if one circle == another circle (same values)
        bool operator!=(circle c) { return !(c == *this); }                    //!< if one cir != another cir (!same values)
        void operator+=(circle c) {      x += c.x;      y += c.y;      d += c.d;     }
        void operator-=(circle c) {      x -= c.x;      y -= c.y;      d -= c.d;     }

        circle() {}
        circle(int X, int Y, int D) {  x = X;   y = Y;    d = D;  } //!< load circle with circle values
    };    //!< circle struct (made of x, y and d)

    typedef RSGL::point triangle[3]; //!< Triangle type
    struct color {  
        unsigned char r, g, b, a; 
        color(){} // mainly for c++98
        color(unsigned char r, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255) { this->r = r;      this->g = g;     this->b = b;      this->a = a;    }
    }; //!< color structure

    enum shape { RECT, LINE, CIRCLE, IMAGE, TRIANGLE, TEXT, EMPTY }; //!< shape constants

    struct bitmap {
        string file;          //!< source file (Images only)
        RSGL::rect r;              //!< drawing rect size
        unsigned char* data;        //!< bitmap data
        unsigned int tex, ovalTex; //!< opengl texture and oval texture for adding textures to smooth ovals
        RSGL::area size;           //!< bitmap size
        int n;                     // bitmap channels

        ~bitmap();
    }; //!< structure for bitmaps and images

    typedef bitmap image; //!< the image struct is the same thing as a bitmap struct

    //!< font stucutre for font info
    struct font {
        string file; //!< font file
        int size; //!< size of the font
        font(string file, int size = 0) { this->file = file; this->size = size; } /* for c++98*/
        font() {}
    };  //!< font structure that can generally be used for a place-holder for string

/** @}*/

//!< window data structs/functions
/** \addtogroup Window_Data_and_Managment
 *  @{
 */

    struct winArgs {
        bool resize = true;      /* can the user resize the window?*/
        bool autoResize = false; //!< Should everything resize if the window size changes?
        bool noBorder = false;   //!< if there is not a boardernn
        bool fullscreen = false; //!< if the window is full screen
        bool center = false;     //!< if the window x/y should be centered
        bool rgbaWindow = false; //!< enable this if you want your window background to have an alpha value OR if you want to make it so transparent shapes show behind the window if there's nothing behind it
        bitmap icon = {0};        //!< custom icon (bitmap or file)
    };                           //!< args for creating a window

    struct event {
        unsigned char type; /*!< which event has been sen t?*/
        unsigned char button; /*!< which mouse button has been clicked (0) left (1) middle (2) right OR which joystick button was pressed OR which joystick axis was moved*/
        point mouse; /*!< mouse x, y of event or Drop point*/

        unsigned char ledState; /*!< 0 : numlock, 1 : caps lock, 3 : small lock*/

        unsigned keyCode; /*!< keycode of event*/

        string keyName; /*!< key name of event */

        /*! drag and drop data */
        int droppedFilesCount; /*!< house many files were dropped */
        vector<string> droppedFiles; /*!< dropped files*/

        /*! joystick*/
        unsigned short joystick; /* which joystick this event applies to (if applicable to any) */
        unsigned char axisesCount; /* number of axises */
        vector<string> axis; /*[4][2] array x, y of axises (-100 to 100) */
    };                                                       //!< event structure to handle events sent to a specific drawable

    struct debug {
        int fps;
    }; //!< struct for debug info per window

    //! Window structure

    struct window {
        string name;  //!< the name of the window
        RSGL::event event; //!< the source event struct
        RSGL::debug debug; //!< debug info for the window
        RSGL::color color; //!< the background color
        RSGL::rect r;      //!< the size/x/y of the window

        RSGL::area areaSize = {-1, -1}; //!< for autoresize

        RGFW_window* source; //!< source window obj
        int tag;

        int GPU;                //!< the GPU option selection
        int fpsCap;
        unsigned int sourceVBO; //!< source vbo object
        bool vbo;               //!< if vbo is supported or not
        bool focused = false;   //!< if the window is focused or not (updated by check events)

        void setIcon(string file);                                                 //!< sets BMP! image to the icon
        RSGL::event checkEvents(bool setcurrentWindowSize = true); //!< checks if any events have been sent (is required to get events), returns an optional event struct as an alternative to win.event, this is mainly to pass the function to another function
        bool isPressed(unsigned int keyCode, bool onFocus = false);                              //!< checks if a key has been pressed (works with key code or string)
        bool isPressed(string keyName, bool onFocus = false);                      //!< checks if a key has been pressed (works with key code or string)
        bool isPressed(vector<unsigned int> keys, bool onFocus = false, bool OR = false);                    //!< checks if a key has been pressed (works with key code or string)
        bool isPressed(vector<string> keys, bool onFocus = false, bool OR = false);            //!< checks if a key has been pressed (works with key code or string)
        void close();                                                                   //!< closes the window
        void clear();                                                                   //!< clears the display
        bool isOpen(bool autoClear = true, bool autoClose = true);    //!< clears the window (toggleable), returns true if the window hasn't been closed, if the window is cloesd (and autoClose is true), RSGL::window::close() is called
        void changeIcon(string icon);
        void changeIcon(unsigned char* icon, int w, int h, int channels);

        /* global data */
        RSGL::point globalMouse(); //!< returns mouse x/y in the context of the screen
        RSGL::rect fullscreen();   //!< returns the size of the screen

        window() {}
        window(
            string /*the name*/,
            RSGL::rect /*the rect/size/x/y of the window*/,
            RSGL::color /*the background color*/,
            winArgs args = winArgs() /*other args*/
        );                    //!< inits the window with these values

        ~window();
    };

    RSGL::area* currentWindowSize(); //!< the current drawing context

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
    int imageCollideRect(RSGL::image img, RSGL::rect r);                   //!< if a image collides with a rect
    int imageCollideCircle(RSGL::image img, RSGL::circle c);               //!< if a image collides with a circle
    int imageCollidePoint(RSGL::image img, RSGL::point p);                 //!< if a image collides with a point
    int imageCollideImage(RSGL::image img, RSGL::image img2);              //!< if a image collides with another image
    /** @}*/

    extern bool vboON;

    void freeDraw(RSGL::area size = *RSGL::currentWindowSize());

    /* The first index of drawArgs.gradient must hold the size of the array at the .r value (excluding the index itself)
        ex. {{2}, {255, 0, 0, 255}, {0, 0, 255, 255}}
    */

    //!< drawing functions
    struct drawArgs {
        //! general
        bool fill = true;                            //!< not for text
        float rotationAngle = 0;                     //!< the angle it's rotated to
        float flipHorizontal = 0;                    //!< how much it's flipped horizontally
        float flipVertical = 0;                      //!< how much it's flipped vertically
        float lineWidth = 0;                         //!< the width of the line
        int texture = -1;                            //!< draw a texture on it (from load/draw image/bitmap functions)
        vector<RSGL::color> gradient = {};      //!< gradient to draw on the shapes
        bool vectorGraphics = true;                  // use vector graphics (textures) for circles, rounded rects or ovals
        RSGL::area windowSize = *currentWindowSize(); //!< window to write on
        //! rect/circle only
        //!< rect-only
        bool dotted = false; //!< is it a dotted rectangle? (rect only)
        bool rounded = false;
        RSGL::point roundPoint = {4, 4}; //!< where the rounding is : 4, 4 is good for a round rect, 24, 24 is a bean shape, 204, 204 is an oval
        //! circle only
        int segments = 30; //!< the amount of segments in a circle
        bool ovalTex = false;
        //! text only
        int scrollSpeed = 0; //!< scroll text speed, 0 for no scroll text
        int stopY = -1;      //!< scroll text stop y (-1, the end of the window)
        int typeSpeed = 0;   //!< speed of the "typing" out of the text, 0 : no typing
    };                       //!< general args for drawing

    /** \addtogroup drawing-functions
     *  @{
     */

    void loadFont(RSGL::font font, int size);

    void drawText(
        string text,  //!< the text to draw
        RSGL::circle c,    //!< the x, y and size the text
        RSGL::color col,   //!< the color
        RSGL::font font,   //!< the font info
        drawArgs args = drawArgs() //!< extra args
    );                     //!< function for drawing text

    int textWidth(string text, RSGL::font font, int size);

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
        string fileName /*the file to draw*/,
        RSGL::rect r /*the width/heigth/x/y to put the image*/,
        drawArgs args = drawArgs()                            //!< extra args
    );                                                //!< draws a image on the screen (returns the index texture in the texture buffer)
    int drawImage(RSGL::image r, drawArgs args = drawArgs()); //!< draw image on screen based on image struct (returns the index texture in the texture buffer)

    int loadImage(string file);                                      //!< loads a image into the texture buffer without drawing (returns the index texture in the texture buffer)
    int loadBitmap(unsigned char *data, int channels, RSGL::area memsize); //!< load a bitmap into the texture buffer (no drawing) (returns the index texture in the texture buffer)

    void freeBitmap(unsigned int tex);
    void freeImage(unsigned int tex);

    void freeBitmap(unsigned char *data);
    void freeImage(string file);

    int drawBitmap(
        unsigned char *data,                      //!< 1D bitmap
        RSGL::rect r,                            //!< size to draw
        unsigned char channels,                            //!< channels in data
        RSGL::area memsize,                      //!< size of data
        RSGL::drawArgs args = drawArgs(),                //!< optional args for drawing
        RSGL::color color = (RSGL::color){255, 255, 255, 255} //!< optional forced color
    );                                           //!< draw a bitmap on the screen (returns the index texture in the texture buffer)
/** @}*/

//!< widgets
/** \addtogroup widgets
 *  @{
 */
#ifndef RSGL_NO_WIDGETS

    /* widget rewrite later */



#endif

#ifndef RSGL_NO_AUDIO
    struct audio {
        string file;
        bool inited = false;
        bool loop;

        void play(string file);
        void play();
        void pause();
        void stop();
        void seek(int pos);
        void setVolume(int vol);
        int length();

        int volume();
        unsigned int position();
        bool isPlaying();
    };
#endif
    /** @}*/

    bool timerT(int ticks);       //!< return true after tick amount of time has passed
    bool timerM(int miliseconds); //!< return true after milisecond amount of time has passed
};

#endif /*RSGL_H*/

#define RSGL_H


#ifdef RSGL_IMPLEMENTATION
#include "source/vector.cpp"


#include "source/rsgl.cpp"
#include "source/draw.cpp"
#include "source/collide.cpp"
#include "source/other.cpp"

#ifndef RSGL_NO_AUDIO
#include "source/audio.cpp"
#endif

#endif