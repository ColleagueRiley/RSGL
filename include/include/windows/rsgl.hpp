#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "../linux/deps/png++/image.hpp"                     


#define RSGLRGBTOHEX(r, g, b) ((r << 16) + (g << 8) + b)

namespace RSGL{
    #ifdef OPENGL
      cairo_surface_t *sfc;
      cairo_t *ctx;
    #endif
    const int KeyPressed=2;
    const int KeyReleased=3;
    const int MouseButtonPressed=4;
    const int MouseButtonReleased=5;
    const int MousePosChanged=6;
    const int quit = 33;
    const int dnd = 34;

    struct point{int x, y;};
    struct rect {int x, y; int length,width; };
    struct circle{int x, y; int radius;};
    struct area{int width,length;};
    struct color{int r,g,b; float a=1.0;};

    int value(RSGL::rect r){return r.x+r.y+r.width+r.length;};
    int value(RSGL::circle c){return c.x+c.y+c.radius;};

    struct image{
      RSGL::rect r;
      RSGL::rect srcr = r; 
      const char* file; 
      std::vector<std::vector<int>> cords;
      png::image< png::rgba_pixel> img;
      bool loaded = false;
    };

    struct text{
      RSGL::rect r; 
      RSGL::rect srcr = r; 
      RSGL::color c;
      RSGL::color sc;
      std::string text;
      std::string stext;
      bool loaded = false;
    };

    struct drawable{
      private:
        struct Event{
          int type;
          int button;
          int x,y;
        };
      public:
        RSGL::rect r;
        Event event;
        
        void loadArea(drawable& dsrc, RSGL::rect r, RSGL::point p);
    };

    struct pixmap : drawable{
        pixmap(){}
        pixmap(RSGL::drawable dr,RSGL::area a);
    };

    struct window : drawable{
      private:
        struct Event{
          int type;
          int button;
          int x,y;
        };
        char keyboard[32];
      public:
        std::string name;
        RSGL::color color;
        RSGL::pixmap dbuffer; //double buffer
        Event event;
        SDL_Window* d;
        SDL_Renderer* r;
        int setColor(RSGL::color c);
        window(){}
        window(std::string,RSGL::rect, RSGL::color, bool resize = false);
        void checkEvents();
        bool isPressed(unsigned long key);
        void close();
        void clear();
    };
    
    window root;
    
    int clear(RSGL::rect r = {0,0,0,0},RSGL::drawable win=root); //clears everything and redraws things defined below it

    int CircleCollidePoint(RSGL::circle c, RSGL::point p);
    int CircleCollideRect(RSGL::circle c, RSGL::rect r);
    int CircleCollide(RSGL::circle cir1,RSGL::circle cir2);
    int RectCollidePoint(RSGL::rect r, RSGL::point p);
    int RectCollideRect(RSGL::rect r, RSGL::rect r2);
    int PointCollide(RSGL::point p, RSGL::point p2);
    int ImageCollideRect(RSGL::image img, RSGL::rect r);
    int ImageCollideCircle(RSGL::image img, RSGL::circle c);
    int ImageCollidePoint(RSGL::image img, RSGL::point p);
    int ImageCollideImage(RSGL::image img, RSGL::image img2);
  
    image loadImage(const char* file, RSGL::rect r, RSGL::drawable win=root);

    int drawImage(RSGL::image image,RSGL::drawable win=root);

    text loadText(std::string word, RSGL::rect r, std::string font, RSGL::color c,RSGL::drawable win=root);

    int drawText(RSGL::text,RSGL::drawable win=root);
    
    bool isPressed(unsigned long key);
    int drawPoint(RSGL::point p, color c,RSGL::drawable win=root);

    
    void drawRect(RSGL::rect r,color c, bool fill=true,int stroke=1, int lineColor = 0, RSGL::color lineCol = {}, RSGL::drawable win=root);

    int drawCircle(RSGL::circle c, color col,bool fill=true,int stroke=1, int lineColor = 0, RSGL::color lineCol = {},RSGL::drawable win=root);


    struct rectButton{
      rect r;
      color c;
      text t;
      RSGL::drawable d;
      void draw(RSGL::drawable win=root){drawRect(r,c);  d=win;}
      bool isClicked(){return d.event.type == MouseButtonReleased && RectCollidePoint(r,{d.event.x,d.event.y});}
      bool isHovered(){return RectCollidePoint(r,{d.event.x,d.event.y});}
      bool isPressed(){return d.event.type == MouseButtonPressed && RectCollidePoint(r,{d.event.x,d.event.y});}
    };

    struct circleButton{
      circle c;
      color col;
      text t;
      RSGL::drawable d;
      void draw(RSGL::drawable win=root){drawCircle(c,col);  d=win;}
      bool isClicked(){return d.event.type == MouseButtonReleased && CircleCollidePoint(c,{d.event.x,d.event.y});}
      bool isHovered(){return CircleCollidePoint(c,{d.event.x,d.event.y});}
      bool isPressed(){return d.event.type == MouseButtonPressed && CircleCollidePoint(c,{d.event.x,d.event.y});}
    };

    struct imgButton{
      image img;
      text t;
      RSGL::drawable d;
      void draw(RSGL::drawable win=root){drawImage(img);  d=win;}
      bool isClicked(){return d.event.type == MouseButtonReleased && ImageCollidePoint(img,{d.event.x,d.event.y});}
      bool isHovered(){return ImageCollidePoint(img,{d.event.x,d.event.y});}
      bool isPressed(){return d.event.type == MouseButtonPressed && ImageCollidePoint(img,{d.event.x,d.event.y});}
    };

    std::vector<std::string> fileDialog(std::string title,bool multiple=false,bool save=false, bool directory=false);
    void notifiy(std::string title, std::string content,std::string image="");
    void messageBox(std::string message, bool question=false,bool error=false);

#ifndef RSGLNAMESPACEEXTENTION
};
#endif