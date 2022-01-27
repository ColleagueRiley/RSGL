#include "../RSGL.hpp"
#include <iostream>
bool running = true;

int main(){
    RSGL::window window("name",{500,500,500,500},{255,255,255});
    RSGL::color c = {255,0,0};
    RSGL::rect r = {100,100,50,50};
    while (running){
        window.checkEvents();
        if (window.event.type == RSGL::quit) running = false;
        RSGL::drawRect({200,200,25,25},{0,0,0},false);
        RSGL::drawRect(r,c);
    } window.close();
} 