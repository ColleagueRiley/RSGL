#include "RSGL.hpp"
#include <math.h>
bool running = true;
RSGL::window window("",{100,400,500,500},{0,0,0});


int main(int argc, char **argv){
    while (running){
        window.checkEvents(); 
        if (window.event.type == RSGL::quit) running=false;
        for (int y=0; y < outline.size(); y++){
            RSGL::drawPoint({outline.at(y).x+200,outline.at(y).y+200},{255,0,0});
        }
        RSGL::drawImage("Rand.png",imR);
        window.clear();
    } window.close();
}