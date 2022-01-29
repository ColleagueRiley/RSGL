#include "RSGL.hpp"
bool running = true;
RSGL::window window("",{500,500,500,500},{0,0,0,500});
#include <math.h>

void RSGL::window::checkEvents(){
  XEvent E;
  XNextEvent(display, &E);
  event.type = E.type;
  std::cout << event.type << std::endl;
  if (event.type == 33 && E.xclient.data.l[0] == (long int)XInternAtom(display, "WM_DELETE_WINDOW", true)){} 
  else if  (event.type == 33 ) {event.type = 0;} 
  if (event.type == 4 || event.type == 5){event.button = E.xbutton.button;}
  if (event.type == 4 || event.type == 5 || event.type == 6){ std::cout << "mouse move"; event.x=E.xbutton.x;event.y=E.xbutton.y;}
  if (event.type == 2 || event.type == 3){XQueryKeymap(display,keyboard);}
  if (event.type == 2 || event.type == 3){ event.keycode = XKeycodeToKeysym(display,E.xkey.keycode,1); event.key=XKeysymToString(event.keycode);}
  else { event.keycode = 0; event.key="";}
  XKeyboardState keystate;
  XGetKeyboardControl(display,&keystate); event.ledState= keystate.led_mask;
}



int main(int argc, char **argv){
    int x = 190;  int frames=0;
    while (running){
        window.checkEvents(); window.clear();
        if (window.event.type == RSGL::quit) running=false;
        if (window.event.type == RSGL::KeyPressed) x+=5;
        RSGL::drawCircle({200,200,20},{200,0,0});
        RSGL::drawRect({window.event.x,window.event.y,20,20},{0,0,200});
    
    } window.close();
} 