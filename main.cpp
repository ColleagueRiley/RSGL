
#include "RSGL.hpp"

bool running = true;
RSGL::window window("",{500,500,500,500},{0,0,0,500});
bool a =true;
    

bool pressed=false;
void RSGL::window::checkEvents(){
  XEvent E;
  if (XEventsQueued(display,QueuedAlready) + XEventsQueued(display,QueuedAfterReading)) XNextEvent(display, &E);
  event.type = E.type;
  if (event.type == 33 && E.xclient.data.l[0] == (long int)XInternAtom(display, "WM_DELETE_WINDOW", true)){} 
  else if(event.type == 33){event.type = 0;} 
  if (event.type == 4 || event.type == 5){event.button = E.xbutton.button; 
    if (event.type==4) pressed=true;  else pressed=false; }
  if (event.type == 4 || event.type == 5 || event.type == 6){
    int x, y,i; unsigned m; unsigned m2; Window w;

    if (XQueryPointer(display, DefaultRootWindow(display), &DefaultRootWindow(display), &w, &x, &y, &i, &i, &m)){event.x=x-(r.width+210); event.y=y-(r.length-200);}
  }
  if (event.type == 2 || event.type == 3){XQueryKeymap(display,keyboard);}
  if (event.type == 2 || event.type == 3){ event.keycode = XKeycodeToKeysym(display,E.xkey.keycode,1); event.key=XKeysymToString(event.keycode);}
  else { 
        event.keycode = 0; event.key="";    
        int x, y,i; unsigned m; unsigned m2; Window w;
        if (XQueryPointer(display, DefaultRootWindow(display), &DefaultRootWindow(display), &w, &x, &y, &i, &i, &m)){
            if (m){
                if (m == 256) event.button=1;
                else if (m == 212) event.button=2;
                else if (m == 1024) event.button=3;
                event.type=RSGL::MouseButtonReleased;
                if (pressed) event.type=RSGL::MouseButtonPressed;
                pressed=!pressed;
            } 
        } 
        else{
            XFlush(display);
        }
  }  
  XWindowAttributes a;
  XGetWindowAttributes(display,d,&a); 
  if (r.width != a.width || r.length != a.height  && areesize){ glViewport(0,0,a.width,a.height); } //std::cout << E.xresizerequest.width << " , " << E.xresizerequest.height << std::endl;}
  r.width=a.width; r.length=a.height; r.x=a.x; r.y=a.y;
  XKeyboardState keystate;
  XGetKeyboardControl(display,&keystate); event.ledState= keystate.led_mask;
}

int main(int argc, char **argv){
    int x = 190;  int frames=0;
    while (running){
        window.checkEvents();
        if (window.event.type == RSGL::MouseButtonPressed) a=true;
        if (window.event.type == RSGL::MouseButtonReleased) a=false;
        RSGL::drawRect({window.event.x,window.event.y,200,200},{255,0,0},a);
       // std::cout << window.debug.fps << std::endl;
        if (window.event.type == RSGL::quit) running=false;
         window.clear();
    } window.close();
} 