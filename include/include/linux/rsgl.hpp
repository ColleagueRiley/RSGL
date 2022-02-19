#pragma once 
#include <iostream> // standard library std::string, std::cout... ect
#include <vector> // standard library vector struct std::vector<>
#include <map> // standard library map struct std::map<>
#include "deps/X11/Xlib.h"  // xlib library for interacting with X11 servers
#include "deps/X11/Xatom.h" //Xlib atom defs
#include "deps/X11/cursorfont.h" //Xlib defs
#include "deps/png++/image.hpp" // for loading .png files                
#include "keys.hpp"
#ifndef RSGLOPEGNL
  #ifndef RSGLNOOPENGL
      #define RSGLOPENGL 0x98342
  #endif
#endif

#ifdef RSGLOPENGL
#include <GL/gl.h> //openGL
#include <GL/glx.h> //openGL
#endif

#define RSGLRGBTOHEX(r, g, b) ((r << 16) + (g << 8) + b)

namespace RSGL{
    // Event vars
    const int KeyPressed=2; // a key has been pressed
    const int KeyReleased=3; // a key has been released
    const int MouseButtonPressed=4; // a mouse button has been pressed (left,middle,right)
    const int MouseButtonReleased=5; // a mouse button has been released (left,middle,right)
    const int MousePosChanged=6; // the position of the mouse has been changed
    const int quit = 33; // the user clicked the quit button
    const int dnd = 34; // a file has been dropped into the window

    // shape/draw structures 
    struct point{int x, y;};  // a single point
    struct rect {int x, y , width, length; }; // a rectangle
    struct circle{int x, y; int radius;}; // a circle
    struct triangle {  int x, y; int width, length; }; /*Triangle structure.*/
    struct area{int width,length;}; // an area (width/length) without a specific x/y
    struct color{int r,g,b; /* red, green blue and float alpha (not required)*/ float a=255;}; // color structure

    // structure for images (.pngs)
    struct image{
      RSGL::rect r; // the area/x/y of the image stored in RSGL::rect
      const char* file; // the source file a image is drawn from
      std::vector<std::vector<int>> cords; // every point in a image
      png::image< png::rgba_pixel> img; // the og pnglib++ image structure
    }; 

    // the structure for a surface that can be drawn upon (window/pixmap ect)
    struct drawable{
      public:
        RSGL::rect r; // the rectangular surface and x,y placement of a drawable
        Display* display; // the source display 
        Drawable d; // the source Xlib drawable structure
        RSGL::color color; // the background color of the drawable
        int GPU; //what  gpu system is being used
        #ifdef RSGLOPENGL
        GLXWindow glxWin;
        #endif
        void loadArea(drawable& dsrc, RSGL::rect r, RSGL::point p); // loads a drawable onto the drawable
    };

    //pixmap stucture
    struct pixmap : drawable{
        pixmap(){} 
        pixmap(
              RSGL::drawable dr /*the source drawable*/,
              RSGL::area a /*how big you ant the pixmap to be (in area)*/); // init fuction
        private:
          XImage* data; // source Image struct
    };

    //Window structure
    struct window : drawable{
      private:
        bool areesize;
        struct Event{ 
          int type; // which even has been sent?
          int button; // which mouse button has been clicked (0) left (1) middle (2) right
          int x,y; // the x/y of the mouse
          int keycode; // the source keycode of which key has been pressed
          std::string key; // a string of the key that has been pressed
          int ledState; // 0 : numlock, 1 : caps lock, 3 : small lock
        }; // event structure to handle events sent to a specific drawable
        struct Debug{
              int fps;
        };
        char keyboard[32]; // 32 bit char of the keyboard status
        #ifndef RSGLNOOPENGL
        GLXContext context;
        XVisualInfo* vInfo;
        #endif
        bool swapFlag=true;
        //static void *event_helper(void *context){return ((RSGL::window *)context)->mouseEvent();}
      public:
        std::string name; // the name of the window
        RSGL::pixmap dbuffer; //double buffer
        Event event; // the source event struct
        Debug debug;
        RSGL::color color;
        #ifdef RSGLOPENGL
        GLXWindow glxWin;
        #endif
        int setColor(RSGL::color c); // changes the background color to c
        window(){}
        window(
              std::string /*the name*/,
              RSGL::rect /*the rect/size/x/y of the window*/,
              RSGL::color /*the background color*/,
              int gpu = 1 /*what to use for graphics?
                    0 - None (xlib/processor only) [ALSO ADD "#ifndef RSGLNOOPENGL" before including RSGL]
                    1 - openGL*/,
              bool resize = false, /* can the user resize the window?*/
              bool autoResize = false /* Should everything resize if the window size changes?*/); //inits the window with these values
        
        void checkEvents(); // checks if any events have been sent (is required to get events)
        bool isPressed(unsigned long key); // checks if a key has been pressed (with key code)
        bool isPressed(std::string key);  // checks if a key has been pressed (with string of a key)
        void close(); // closes the window
        void clear(); // clears the display
    };
    
    extern window root; // the first window made
    void test(RSGL::drawable d=root);
    // collition functions
    int CircleCollidePoint(RSGL::circle c, RSGL::point p); // if a circle collides with a point
    int CircleCollideRect(RSGL::circle c, RSGL::rect r); // if a circle collides with a rect
    int CircleCollide(RSGL::circle cir1,RSGL::circle cir2); // if a circle collides with another circle
    int RectCollidePoint(RSGL::rect r, RSGL::point p); // if a rect collides with a point
    int RectCollideRect(RSGL::rect r, RSGL::rect r2); // if a rect collides with another rect
    int PointCollide(RSGL::point p, RSGL::point p2); // if a point collides with another point
    int ImageCollideRect(RSGL::image img, RSGL::rect r); // if a image collides with a rect
    int ImageCollideCircle(RSGL::image img, RSGL::circle c); // if a image collides with a circle
    int ImageCollidePoint(RSGL::image img, RSGL::point p); // if a image collides with a point
    int ImageCollideImage(RSGL::image img, RSGL::image img2); // if a image collides with another image
  
    // structure for text
    struct Text{
      RSGL::circle rect; // the rectangular area/x/y of the text
      RSGL::color c; // the color of the text
      std::string text; // the text the text displays
      const char* f; // the text's font
      RSGL::window d=RSGL::root; // the window the text draws to
      Text(std::string txt /*the text*/, RSGL::circle r /*the source x/y/size of the text*/, const char* font /*the font of the text*/, RSGL::color col /*the color of the text*/, bool draw=true /**should it draw or not*/, RSGL::drawable d=RSGL::root /*what should it draw on*/); // init the text struct
      void draw(); // draws text using the text struct
      Text(){}
    };

    //drawing functions
    void drawText(
          std::string text /*the text*/, 
          RSGL::circle r /*the source x/y/size of the text*/, 
          const char* font /*the font of the text*/, 
          RSGL::color c /*the color of the text*/, 
          RSGL::drawable d=RSGL::root  /*what should it draw on*/); // draw text based on args

    int drawPoint(
          RSGL::point p /*the point to be draw*/, 
          color c /*the color of the point*/,
          RSGL::drawable win=root /*the window to draw on*/); // draws a point on the screen
    
    void drawRect(
          RSGL::rect r /*the rect to draw*/,
          color c /*the color rect*/, 
          bool fill=True /* fill or just outlines?*/, 
          bool dotted=false, /*is it a dotted rectangle?*/
          RSGL::drawable win=root /*the window to draw on*/); // draws a rect on the screen

    void drawLine(
            RSGL::point p1 /* the first point to draw */, 
            RSGL::point p2 /* the second point (where to stop)*/, 
            RSGL::color c,
            RSGL::drawable win=root /*the window to draw on*/); // draws a cirlce on the screen

    int drawCircle(
          RSGL::circle c /*the circle to draw*/, 
          color col /* the color of the circle*/, 
          bool fill=True /* fill or just outlines?*/, 
          RSGL::drawable win=root /*the window to draw on*/); // draws a cirlce on the screen

    void drawImage(
          std::string fileName /*the file to draw*/, 
          RSGL::rect r /*the width/length/x/y to put the image*/,
          bool resize=true /*should the image resize if it needs to?*/,
          RSGL::drawable d=RSGL::root /*the window to draw on*/); // draws a image on the screen

    std::vector<std::vector<RSGL::color>> resizeImage(std::vector<std::vector<RSGL::color>> image, RSGL::rect newSize, RSGL::rect ogsize); // resizes an image (.png) file to a resized 2d vector


    std::vector<std::string> fileDialog(
        std::string title /*the title of the sub window*/,
        bool multiple=false /*one or more files?*/,
        bool save=false /*to save a file?*/, 
        bool directory=false /*should it accept folders or files*/); // semds the file dialog

    void notifiy(
        std::string title /*the title of the notifcation*/, 
        std::string content /*what's in the notification*/,
        std::string image="" /*an image file to add to it*/); // sends notifcation

    void messageBox(std::string message /*the message*/, 
        bool question=false /*is it a question?*/,
        bool error=false /*is it an error?*/); // sends a message box errors/questions
#ifndef RSGLNAMESPACEEXTENTION
};
#endif