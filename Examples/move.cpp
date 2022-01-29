#define RSGLNOOPENGL
#include "../RSGL.hpp"
#include <iostream>

#define left                        0xff51  /* Move left, left arrow */
#define right                         0xff53  /* Move right, right arrow */

bool running = true;

RSGL::window win("name",{500,500,500,500},{255,255,255},0);

int main(){
    RSGL::color c = {255,0,0};
    RSGL::rect r = {100,100,50,50};
    while (running){
        win.checkEvents();
        switch(win.event.type){
            case RSGL::quit: running = false; break;
            case RSGL::KeyPressed: if (win.isPressed(right)) r.x++; else if (win.isPressed(left)) r.x--; break;
            default: break;
        }
        win.clear();
        RSGL::drawRect({200,200,25,25},{0,0,0},false);
        RSGL::drawRect(r,c);
    } win.close();
} 