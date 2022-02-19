
#include "RSGL.hpp"

bool running = true;
RSGL::window window("",{500,500,500,500},{0,0,0,500});
bool a =true; bool pressed=false;


int main(int argc, char **argv){
    int x = 190;  int frames=0;
    while (running){
        window.checkEvents();
        if (window.event.type == RSGL::MouseButtonPressed) a=true; 
        if (window.event.type == RSGL::MouseButtonReleased) a=false;
        RSGL::drawRect({window.event.x,window.event.y,20,20},{255,0,0},a,false,window);
        std::cout << window.event.x << std::endl;
        std::cout << window.event.y<< std::endl;
        // std::cout << window.debug.fps << std::endl;
        if (window.event.type == RSGL::quit) running=false; 
        window.clear();
    } window.close();
} 