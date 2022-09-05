#pragma once 
#include <string> // standard library string std::string
#include <vector> // standard library vector struct std::vector<>
#include <array> // standard library array struct std::array<type,size>

// def struct defines so they can be pre-compiled in
struct dtx_font;
typedef struct _XDisplay Display;
typedef struct __GLXcontextRec *GLXContext; 

#define RSGLRGBTOHEX(r, g, b) ((r << 16) + (g << 8) + b) 

#define RSGLH 1 // for checking if the header is loaded

namespace RSGL{
    // Event constants
    const int keyPressed = 2; // a key has been pressed
    const int keyReleased = 3; // a key has been released
    const int mouseButtonPressed = 4; // a mouse button has been pressed (left,middle,right)
    const int mouseButtonReleased = 5; // a mouse button has been released (left,middle,right)
    const int mousePosChanged = 6; // the position of the mouse has been changed
    const int quit = 33; // the user clicked the quit button
    const int dnd = 34; // a file has been dropped into the window
    
    // mouse constants : win.event.button
    const int mouseLeft = 1; 
    const int mouseMiddle = 2; 
    const int mouseRight = 3;
    const int mouseScrollUp = 4;
    const int mouseScrollDown = 5;

    // shape/draw structures 
    struct point{ int x, y; }; // a single point
    struct rect { int x, y , w, h, rotationAngle = 0; }; // a rectangle
    struct circle{ int x, y, radius; }; // a circle
    struct triangle { std::array<RSGL::point, 3> points; };  /*Triangle structure.*/
    struct area{ int w, h; }; // an area (width/heigth) without a specific x/y
    struct color{ int r, g, b, a = 255; }; // color structure
    enum shape{ RECT, LINE, CIRCLE, IMAGE, TRIANGLE, TEXT, EMPTY }; // shape constants

    // structure for images
    struct image{
      std::string file; // the source file a image is drawn from
      RSGL::rect r; // the area/x/y of the image stored in RSGL::rect
      unsigned int tex; // the openGL texture for the image
    }; 

    // font stucutre for font info
	  struct Font{
        std::string file; // font file
        int size = 0; // size of the font
        dtx_font* font; // source dtx_font
        
        Font(){}
        // functions for loading a font, with a string/data
        Font(std::string File, int Size); 
        Font(std::string File) { file = File; }  
        Font(const char* File){ file = File; } 
    };

    struct winArgs{ 
        int gpu = 1; /*what to use for graphics?
                     1 - openGL, that's it*/ 
        bool resize = false; /* can the user resize the window?*/
        bool autoResize = false; // Should everything resize if the window size changes?
    }; // args for creating a window

    //Window structure
    class window {
        //hidden data structs
        struct Event{ 
          int type; // which even has been sent?
          int button; // which mouse button has been clicked (0) left (1) middle (2) right
          RSGL::point mouse; // the x/y of the mouse

          int ledState; // 0 : numlock, 1 : caps lock, 3 : small lock
          int keysym; // the source keycode of which key has been pressed
          std::string key; // a string of the key that has been pressed
        }; // event structure to handle events sent to a specific drawable
        
        struct Debug{
            int fps;
        }; // struct for debug info per window
        
        bool areesize, swapFlag = true, isRoot; // private flags
        char keyboard[32]; // 32 bit char of the keyboard statuss
        void* vInfo; // visual info for the window
        
      public:
        std::string name; // the name of the window
        Event event; // the source event struct
        Debug debug; // debug info for the window
        RSGL::color color; // the background color
        RSGL::rect r; // the size/x/y of the window
        
        //sources
        Display* display; // xlib display
        unsigned long d; // xlib drawable/window
        unsigned long glxWin; // source GLXwindow
        GLXContext context; //source GLXcontext

        int GPU; // the GPU option selection

        void setColor(RSGL::color c); // sets the background color to c
        void checkEvents(); // checks if any events have been sent (is required to get events)
        bool isPressed(unsigned long key); // checks if a key has been pressed (with key code)
        bool isPressed(std::string key);  // checks if a key has been pressed (with string of a key)
        void close(); // closes the window
        void clear(); // clears the display
        void moveTo(RSGL::point p); // moves the window's x/y to the points
        bool isOpen(bool checkEvents = true); // returns true if the window hasn't been closed

        window(){}
        window(
              std::string /*the name*/,
              RSGL::rect /*the rect/size/x/y of the window*/,
              RSGL::color /*the background color*/,
              winArgs args = {} /*other args*/ 
        ); // inits the window with these values
    };
    
    RSGL::window* root(); // the first window created

    // functional optional gui system, I don't like this kinda system
    namespace func{
        
        void start( 
            bool running, // run var 
            void (*gameLoop)(RSGL::window), // a gameLoop function 
            void (*eventLoop)(int, RSGL::window), // a function for checking events
            RSGL::window win = *root() // which window is this running on?
        );

        // start, but with running as an pointer so you can control it outside 
        void start( bool* running , void (*gameLoop)(RSGL::window), void (*eventLoop)(int,RSGL::window), RSGL::window win = *root() );
    };
    

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
    struct drawArgs{
        // general
        bool fill = true; // not for text
        RSGL::window win = *root();
        // rect/circle only
        // rect-only
        bool dotted = false; // is it a dotted rectangle? (rect only)
        bool rounded = false;
        // text only
        int scrollSpeed = 0; // scroll text speed, 0 for no scroll text 
        int stopY = -1; // scroll text stop y (-1, the end of the window)
        int typeSpeed = 0; // speed of the "typing" out of the text, 0 : no typing 
    }; // general args for drawing
    
    void drawText(
            std::string text, // the text to draw
            RSGL::circle c, // the x, y and size the text
            RSGL::Font font, // the font info
            RSGL::color col, // the color
            drawArgs args = {} // extra args
    );

    void drawPoint(
          RSGL::point p, // the point to be draw
          RSGL::color c, // the color of the point
          RSGL::window win = *root() // the window to draw on
    ); // draws a point on the screen
    
    // point drawing function, draw based on one color or a list of colors
    void drawPoints(
        std::vector<RSGL::point> p, // list of points
        RSGL::color c, // single color
        RSGL::window win = *root() // window to draw on
    );
    
    void drawPoints( 
            std::vector<RSGL::point> p, // list of points to draw
            std::vector<RSGL::color> c, // multi colors ( must be the same size as p ) 
            RSGL::window win = *root() // window to draw on
    );

    void drawRect(
        RSGL::rect r /*the rect to draw*/,
        color c /*the color rect*/, 
        drawArgs args = {} // extra args
    ); // draws a rect on the window
      
    void drawLine(
        RSGL::point p1 /* the first point to draw */, 
        RSGL::point p2 /* the second point (where to stop)*/, 
        RSGL::color c,
        RSGL::window win = *root() /*the window to draw on*/
    ); // draws a cirlce on the screen

    void drawCircle(  
        RSGL::circle c /*the circle to draw*/, 
        RSGL::color col /* the color of the circle*/, 
        drawArgs args = {} // extra args
    ); // draws a cirlce on the screen

    void drawTriangle(
        RSGL::triangle t, // triangle struct 
        RSGL::color c,  // color of the triangle
        drawArgs args = {} // extra args
    );
    
    void drawImage(
          std::string fileName /*the file to draw*/, 
          RSGL::rect r /*the width/heigth/x/y to put the image*/,
          RSGL::window d = *root() /*the window to draw on*/); // draws a image on the screen
    void drawImage(RSGL::image r, RSGL::window win = *root()); 
    
    std::vector<std::string> fileDialog(
        std::string title /*the title of the sub window*/,
        bool multiple = false /*one or more files?*/,
        bool save = false /*to save a file?*/, 
        bool directory = false /*should it accept folders or files*/
    ); // sends the file dialog

    // widgets 

    struct button{
        shape s; // the shape of the button
        int event = 0; // the current event
        void draw(RSGL::window win = *root()); // draw the button
        void checkEvents(RSGL::window win = *root()); // check events based on win
        void checkAndDraw(RSGL::window win = *root()); // check events... then draw the button
        
        // more function programming for those who are into that, I only judge a little
        std::vector<void(*)(void)> functions = {NULL, NULL, NULL, NULL}; // functions to run based on the event
        void setOnIdle( void(*OnIdle)(void) ){ functions.at(0) = OnIdle; } // set the function for when the button idle
        void setOnHover( void(*OnHover)(void) ){ functions.at(1) = OnHover; } // set the function for the button is hovered
        void setOnClick( void(*OnClick)(void) ){ functions.at(2) = OnClick; } // set the function for the button is clicked
        void setOnHold( void(*OnHold)(void) ){ functions.at(3) = OnHold; } // set the function for the button is held
        // bit function to set them all at once
        void setOnAll( void(*OnIdle)(void), void(*OnHover)(void), void(*OnClick)(void), void(*OnHold)(void) ){ functions.at(0)=OnIdle; functions.at(1)=OnHover; functions.at(2)=OnClick; functions.at(3)=OnHold;  }
        void run(RSGL::window win = *root()); // run the functional button system
        
        // shape data ( don't write to these directly until after the init )
        RSGL::rect rect; 
        std::string File, Text; 
        RSGL::color color; 
        RSGL::color rectColor;

        private: 
            // private data for the button backend
            RSGL::rect imageRect, buttonRect;  RSGL::circle cir, textCir;
            RSGL::Font font;  
            std::vector<button> states;
            bool Draw = true; bool pressed = false;

            bool collideWithMouse(RSGL::window win);
            void switchTo(int);

        public:
            button(){} 
            // init functions

            // set button based on a shape/color 
            button(RSGL::rect r, RSGL::color col){ s = RECT; rect = r; color = col; }  // rect
            button(RSGL::circle c, RSGL::color col){ s = CIRCLE; cir = c; color = col; } // cirlce

            // set button based on an image and rect/color [optional]
            button(const char* file, RSGL::rect imageR, RSGL::rect buttonR = {0, 0, 0, 0}, RSGL::color rColor = {}){ rectColor = rColor; s = IMAGE; imageRect = imageR; rect = buttonR; }
            // set button based on text and rect/color
            button(
                const char* text,  // text itself
                RSGL::Font f, // font of the text 
                RSGL::circle textC,  // size of the text
                RSGL::rect buttonR, // rect to draw
                RSGL::color textColor, // color of the text
                RSGL::color rColor = {0, 0, 0, 0} // color of the rect none by default
            ){ color = textColor; rectColor = rColor; s = TEXT; textCir = textC; font = f; rect = buttonR; Text = text; }
            
            // init a button with a vector of buttons as the states [for changing automatically based on the event]
            button(std::vector<button> buttons){ states = buttons; }
    }; // button widget struct

    struct checkBox{
        RSGL::rect rect; // rect size of the checkbox

        void draw(RSGL::window win = *root());  // draw the checkBox on win
        void checkEvents(RSGL::window win = *root()); // check the events based on win
        void checkAndDraw(RSGL::window win = *root()); // check the events and draw based on win
        bool isPressed(); // if the checkBox checked or not

        private:
            // private datas
            RSGL::color c1, c2; 
            std::string File = "", File2 = "";
            bool pressed = false;

        public:
            checkBox(){}
            // init functions
            checkBox(
                    RSGL::rect r, // rect size
                    RSGL::color color1,  // not-pressed color
                    RSGL::color color2 // pressed color
            ){ rect = r; c1 = color1; c2 = color2; }
            checkBox(
                const char* file1, // pressed image 
                std::string file2, // not-pressed image
                RSGL::rect r // rect size
                ){ rect = r; File = file1; File2 = file2; }
    }; // check box widget struct 
    
    // splash screen widget
    void splashScreen(
        std::string file = "res/images/logo.png", // file to use
        std::string font = "res/fonts/SansPosterBold.ttf", // font to use
        int t = 100, // how long it should last
        RSGL::window d = *root() // window to draw on
    ); 

    // progressBar widgets
    void progressBar(
            RSGL::rect r, // size of the bar
            RSGL::color c, // color of the bar
            int pos, // pos of progress %
            int dir = 1, // direction of progress
            RSGL::color c2 = {0, 0, 0, 0}); // color of progress 

    // automatically moving version
    void progressBarAuto(RSGL::rect r, RSGL::color c, 
            int speed = 1, // how far it moves per tick
            int ticks = 0, // how long each tick is
            int dir = 1, RSGL::color c2 = {0, 0, 0, 0}
    );
    
    struct menu{    
        std::string text, font , image; 
        RSGL::color c, // color of the text
        bgcolor, // bg color
        c2 = {0, 0, 0, 0}; // outline color
    }; // basic menu widget stuct

    struct mouseMenu{
        int draw(RSGL::window win = *root()); // draw the mouseMenu on the window
        mouseMenu(){}
        mouseMenu(
            RSGL::rect r, // rect size of the menu
            RSGL::color c, // color of the rect
            std::vector<menu> m, // menus
            RSGL::window win = *root() // window to draw on
        );
        
        private:
            // private datas for processing
            RSGL::rect rect; 
            RSGL::color color;
            std::vector<menu> menus;
            std::vector<RSGL::button> bs;
            bool pressed = false;
            RSGL::point p; 
            int tick = 0;
    }; // mouseMenu widget struct

    struct sideMenu{
      int draw(RSGL::window win = *root()); // draw the slide menu (returns the slide pos) on win
      sideMenu(){}
      // init slide menu based on menus
      sideMenu(
            RSGL::rect r, // rect area
            RSGL::color c, // color of the rect
            std::vector<menu> m, // menus
            RSGL::window win = *root() // window
      );
      private:
          //private data
          RSGL::color color; RSGL::rect rect;
          std::vector<menu> menus;
          std::vector<RSGL::button> bs;
          int tick = 0;
    };

    struct listEvent{ bool event; int pressed; }; // event data to return ( event = 0 == not an event)
    struct list{
      std::vector<std::string> list; // backend list string
      std::string font; // font of the text
      RSGL::rect r; // rect of where the list draws
      RSGL::color c, // color of the text
                bg, // background color
                oc, // outline color
                fc; // button color
      int pressed = 0; // which item has been pressed (0 none) ( list[pressed-1] )
      RSGL::listEvent draw(RSGL::window win = *root()); // draw the list
    };

    bool timer(int ticks); // return true after ticks amount of time has passed
 
    // other
    void notifiy(
        std::string title, /*the title of the notifcation*/ 
        std::string content, /*what's in the notification*/
        std::string image = "" /*an image file to add to it*/
    ); // sends notifcation

    void messageBox(std::string message, /*the message*/
        bool question = false, /*is it a question?*/
        bool error = false /*is it an error?*/
    ); // sends a message box errors/questions
    
    // other ( mostly used by RSGL )

    // image loading
    RSGL::image loadImage(RSGL::image r); 
    RSGL::image loadSVG(RSGL::image r); 

    RSGL::rect fullscreen(); // returns the size of the screen
    
    // loads a char* buffer into the tex vector, so you can load an image statically
    void loadTex(std::vector<std::vector<unsigned char*>> tex);
    
    // weird bounded circle thing, mostly used for drawing other shapes
    void drawBoundCircle(RSGL::circle c, RSGL::color col, RSGL::rect boundary = {-1, -1, -1, -1},RSGL::rect noHIT = {-1, -1, -1, -1},bool fill = true,RSGL::window win = *root());

#ifndef RSGLNAMESPACEEXTENTION
};
#endif
