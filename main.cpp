
#include "RSGL.hpp"
bool running = true;
RSGL::window window("",{500,500,500,500},{0,0,0,500});

int main(int argc, char **argv){
    int x = 190;  int frames=0;
    while (running){
        window.checkEvents();
        RSGL::drawRect({200,200,200,200},{255,0,0},false,true);
       // std::cout << window.debug.fps << std::endl;
        if (window.event.type == RSGL::quit) running=false;
         window.clear();
    } window.close();
} 