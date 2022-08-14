#pragma once 
#include <iostream> // standard library std::string, std::cout... ect
#include <vector> // standard library vector struct std::vector<>
#include <array> // standard library array struct std::array<type,size>
#include <map> // standard library map struct std::map<>
#include <X11/Xlib.h>
#include "keys.hpp"
#include "deps/drawtext.h"
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
    
    const int mouseLeft =1;
    const int mouseMiddle= 2;
    const int mouseRight =3;
    const int mouseScrollUp =4;
    const int mouseScrollDown =5;

    // shape/draw structures 
    struct point{int x, y;};  // a single point
    struct rect {int x, y , width, length; int rotationAngle=0; }; // a rectangle
    struct circle{int x, y; int radius;}; // a circle
    struct triangle {  std::array<RSGL::point, 3> points; };  /*Triangle structure.*/
    struct area{int width,length;}; // an area (width/length) without a specific x/y
    struct color{int r,g,b; /* red, green blue and float alpha (not required)*/ float a=255;}; // color structure
    struct stroke{ int size; RSGL::color color; bool fill=true;}; // info about the stroke of a shape

    // structure for images (.pngs)
    struct image{
      RSGL::rect r; // the area/x/y of the image stored in RSGL::rect
      std::string file; // the source file a image is drawn from
      std::vector<unsigned> cords; // every point in a image
      GLuint tex;
      image(){};
    }; 
	  struct Font{
        dtx_font* font; int size=0; std::string file;
        Font(){}
        Font(std::string File, int Size);
        Font(std::string File) { file=File; }  Font(const char* File){ file=File; }
    };

    // the structure for a surface that can be drawn upon (window/pixmap ect)
    struct drawable{
      public:
        bool isRoot=false;
        RSGL::rect r; // the rectangular surface and x,y placement of a drawable
        Display* display; // the source display 
        Drawable d; // the source Xlib drawable structure
        RSGL::color color; // the background color of the drawable
        int GPU; //what  gpu system is being used
        #ifdef RSGLOPENGL
        GLXWindow glxWin;
        GLXContext context;
        #endif
        void loadArea(drawable& dsrc, RSGL::rect r, RSGL::point p); // loads a drawable onto the drawable
    };

    //pixmap stucture
    struct pixmap : drawable{
        pixmap(){} 
        pixmap(
              RSGL::drawable dr /*the source drawable*/,
              RSGL::area a /*how big you ant the pixmap to be (in area)*/); // init fuction
    };

    struct winArgs{int gpu=1; bool resize=false, autoResize=false;};
    //Window structure
    struct window : drawable{
      private:
        bool areesize;
        struct Event{ 
          int type; // which even has been sent?
          int button; // which mouse button has been clicked (0) left (1) middle (2) right
          RSGL::point mouse; // the x/y of the mouse
          int keycode; // the source keycode of which key has been pressed
          std::string key; // a string of the key that has been pressed
          int ledState; // 0 : numlock, 1 : caps lock, 3 : small lock
        }; // event structure to handle events sent to a specific drawable
        struct Debug{
              int fps;
        };
        char keyboard[32]; // 32 bit char of the keyboard status
        #ifndef RSGLNOOPENGL
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
        GLXContext context;
        #endif
        int setColor(RSGL::color c); // changes the background color to c
        window(){}
        window(const char*){isRoot=true;};
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
        bool isOpen(bool checkEvents=true);
    };
    
    RSGL::window* root();

    void test(RSGL::drawable d=*root());
    // collition functions
    int CircleCollidePoint(RSGL::circle c, RSGL::point p); // if a circle collides with a point
    int CircleCollideRect(RSGL::circle c, RSGL::rect r); // if a circle collides with a rect
    int CircleCollide(RSGL::circle cir1,RSGL::circle cir2); // if a circle collides with another circle
    int CircleCollideLine(RSGL::circle c,RSGL::point p1, RSGL::point p2);
    int RectCollidePoint(RSGL::rect r, RSGL::point p); // if a rect collides with a point
    int RectCollideRect(RSGL::rect r, RSGL::rect r2); // if a rect collides with another rect
    int PointCollide(RSGL::point p, RSGL::point p2); // if a point collides with another point
    int ImageCollideRect(RSGL::image img, RSGL::rect r); // if a image collides with a rect
    int ImageCollideCircle(RSGL::image img, RSGL::circle c); // if a image collides with a circle
    int ImageCollidePoint(RSGL::image img, RSGL::point p); // if a image collides with a point
    int ImageCollideImage(RSGL::image img, RSGL::image img2); // if a image collides with another image



      //drawing functions
    void drawText(
                  std::string text,
                  RSGL::circle c,
                  RSGL::Font font,
                  RSGL::color col,
                  RSGL::window win=*root());

    int drawPoint(
          RSGL::point p /*the point to be draw*/, 
          color c /*the color of the point*/,
          RSGL::drawable win=*root() /*the window to draw on*/); // draws a point on the screen
      
      void drawPoints(std::vector<RSGL::point> p, RSGL::color c, RSGL::drawable win=*root());
      void drawPoints(std::vector<RSGL::point> p,std::vector<RSGL::color> c, RSGL::drawable win=*root());
      
      void drawRect(
          RSGL::rect r /*the rect to draw*/,
          color c /*the color rect*/, 
          bool fill=True /* fill or just outlines?*/, 
          bool dotted=false, /*is it a dotted rectangle?*/
          RSGL::drawable win=*root() /*the window to draw on*/); // draws a rect on the screen
      struct rectArgs{bool fill=false, dotted=false; RSGL::drawable win=*root();};      
      void drawRecta(RSGL::rect r, RSGL::color c, rectArgs args);
      
    void drawLine(
            RSGL::point p1 /* the first point to draw */, 
            RSGL::point p2 /* the second point (where to stop)*/, 
            RSGL::color c,
            RSGL::drawable win=*root() /*the window to draw on*/); // draws a cirlce on the screen

    int drawCircle(
          RSGL::circle c /*the circle to draw*/, 
          color col /* the color of the circle*/, 
          bool fill=True /* fill or just outlines?*/, 
          RSGL::drawable win=*root() /*the window to draw on*/); // draws a cirlce on the screen
    struct circleArgs{bool fill=false; RSGL::drawable win=*root();};      
    void drawCirclea(RSGL::circle c, RSGL::color col, circleArgs args);

    int drawTriangle(RSGL::triangle t, RSGL::color c, bool solid=true, RSGL::drawable win=*root());
    struct TriArgs{bool solid=false; RSGL::drawable win=*root();};      
    int drawTrianglea(RSGL::triangle t, RSGL::color c, TriArgs args);
    
    void drawImage(
          std::string fileName /*the file to draw*/, 
          RSGL::rect r /*the width/length/x/y to put the image*/,
          RSGL::window d=*root() /*the window to draw on*/); // draws a image on the screen
    int drawImage(RSGL::image r, RSGL::window win=*root());

    void drawSVG(
          std::string fileName /*the file to draw*/, 
          RSGL::rect r /*the width/length/x/y to put the image*/,
          RSGL::window d=*root() /*the window to draw on*/); // draws a image on the screen
    int drawSVG(RSGL::image r, RSGL::window win=*root());
    
    //draw with strokes 
    void drawCircle(RSGL::circle c, RSGL::color col, stroke s,bool fill=true, RSGL::window d=*root());
    void drawRect(RSGL::rect r, RSGL::color col, stroke s,bool fill=true, RSGL::window d=*root());

    void drawBoundCircle(RSGL::circle c, RSGL::color col, RSGL::rect boundary={-1,-1,-1,-1},RSGL::rect noHIT={-1,-1,-1,-1},bool fill=true,RSGL::window win=*root());
    
    void drawRoundRect(RSGL::rect r,RSGL::color c,bool fill=true,RSGL::window win =*root()); 

    std::vector<std::string> fileDialog(
        std::string title /*the title of the sub window*/,
        bool multiple=false /*one or more files?*/,
        bool save=false /*to save a file?*/, 
        bool directory=false /*should it accept folders or files*/); // sends the file dialog

    void notifiy(
        std::string title /*the title of the notifcation*/, 
        std::string content /*what's in the notification*/,
        std::string image="" /*an image file to add to it*/); // sends notifcation

    void messageBox(std::string message /*the message*/, 
        bool question=false /*is it a question?*/,
        bool error=false /*is it an error?*/); // sends a message box errors/questions
    
    namespace func{
        void start( bool running , void (*gameLoop)(RSGL::window), void (*eventLoop)(int,RSGL::window), RSGL::window win = *root() );
        void start( bool* running , void (*gameLoop)(RSGL::window), void (*eventLoop)(int,RSGL::window), RSGL::window win = *root() );
    };
    
    enum Shape{ RECT, LINE, CIRCLE, IMAGE, TRIANGLE, TEXT, EMPTY };
    struct Button{
        Shape s; int event=0;
        Button(){}
        void draw(RSGL::window win = *root());
        void checkEvents(RSGL::window win = *root());
        void checkAndDraw(RSGL::window win = *root());
        
        std::vector<void(*)(void)> functions = {NULL,NULL,NULL,NULL};
        void setOnIdle( void(*OnIdle)(void) ){ functions.at(0)=OnIdle; }
        void setOnHover( void(*OnHover)(void) ){ functions.at(1)=OnHover; }
        void setOnClick( void(*OnClick)(void) ){ functions.at(2)=OnClick; }
        void setOnHold( void(*OnHold)(void) ){ functions.at(3)=OnHold; }
        void setOnAll( void(*OnIdle)(void), void(*OnHover)(void), void(*OnClick)(void), void(*OnHold)(void) ){ functions.at(0)=OnIdle; functions.at(1)=OnHover; functions.at(2)=OnClick; functions.at(3)=OnHold;  }
        void run(RSGL::window win = *root());
        
        RSGL::rect rect; std::string File, Text; RSGL::color color; RSGL::color rectColor;
        private:
            RSGL::rect imageRect, buttonRect;  RSGL::circle cir, textCir;
            RSGL::Font font;  
            std::vector<Button> states;
            bool Draw=true; bool pressed=false;

            bool collideWithMouse(RSGL::window win);
            void switchTo(int);

        public:
            Button(RSGL::rect r,RSGL::color col){ s=RECT; rect=r; color=col; }
            Button(RSGL::circle c,RSGL::color col){ s=CIRCLE; cir=c; color=col; }
            Button(const char* file, RSGL::rect imageR, RSGL::rect buttonR={0,0,0,0}, RSGL::color rColor={}){ rectColor=rColor; s=IMAGE; imageRect=imageR; rect=buttonR; }
            Button(const char* text, RSGL::Font f, RSGL::circle textC, RSGL::rect buttonR,RSGL::color textColor,RSGL::color rColor={0,0,0,0}){ color=textColor; rectColor=rColor; s=TEXT; textCir=textC; font=f; rect=buttonR; Text=text; }
            Button(std::vector<Button> buttons){ states=buttons; }
    };

    struct CheckBox{
        CheckBox(){}
        void draw(RSGL::window win = *root());
        void checkEvents(RSGL::window win = *root());
        void checkAndDraw(RSGL::window win = *root());
        bool isPressed();

        RSGL::rect rect;
        private:
            RSGL::color c1, c2;
            std::string File=""; std::string File2="";
            bool pressed=false;

        public:
            CheckBox(RSGL::rect r,RSGL::color color1, RSGL::color color2){ rect=r; c1=color1; c2=color2; }
            CheckBox(const char* file1, std::string file2, RSGL::rect r){ rect=r; File=file1; File2=file2; }
    };
    void splashScreen(std::string file="res/images/logo.png",std::string font="res/fonts/SansPosterBold.ttf",int t=100,RSGL::window d=*root()); 

    void scrollText(std::string text, RSGL::circle r, RSGL::Font font, RSGL::color col, int speed, int stopY=-1, RSGL::window win=*root());
    void progressBar(RSGL::rect r, RSGL::color c, int pos,int dir=1, RSGL::color c2={0,0,0,0});
    void progressBarAuto(RSGL::rect r, RSGL::color c, int speed=1, int ticks=0, int dir=1, RSGL::color c2={0,0,0,0});
    void drawLink(std::string url,RSGL::circle c,RSGL::Font font,RSGL::color col,std::string text="",bool underLine=true,RSGL::window win=*root());
    void drawTypingText(std::string text,RSGL::circle c,RSGL::Font font,RSGL::color col,int speed=3,RSGL::window win=*root());
    
    struct Menu{std::string text, font, image; RSGL::color c, bgcolor, c2={0,0,0,0};};
    struct mouseMenu{
        mouseMenu(){}
        mouseMenu(RSGL::rect r,RSGL::color c, std::vector<Menu> m, RSGL::window win=*root());
        int draw(RSGL::window win=*root());
        private:
            RSGL::color color; RSGL::rect rect;
            std::vector<Menu> menus;
            std::vector<RSGL::Button> bs;
            bool pressed=false;
            RSGL::point p; int tick=0;
    };
    struct sideMenu{
      sideMenu(){}
      sideMenu(RSGL::rect r,RSGL::color c, std::vector<Menu> m, RSGL::window win=*root());
      int draw(RSGL::window win=*root());
      private:
          RSGL::color color; RSGL::rect rect;
          std::vector<Menu> menus;
          std::vector<RSGL::Button> bs;
          int tick=0;
    };

    bool timer(int ticks);

    struct listEvent{bool event; int pressed; };
    struct List{
      std::vector<std::string> list;
      std::string font;
      RSGL::rect r;
      RSGL::color c,bg,oc,fc;
      int pressed=0;
      RSGL::listEvent draw(RSGL::window win=*root());
    };

    RSGL::rect fullscreen();

#ifndef RSGLNAMESPACEEXTENTION
};
#endif
