#include "RSGL.hpp"
bool running = true;
RSGL::window window("",{500,500,500,500},{0,0,0,500});



int main(int argc, char **argv){
    int x = 100;  int frames=0;
    while (running){
        window.checkEvents(); window.clear();
        if (window.event.type == RSGL::quit) running=false;
        if (window.event.type == RSGL::KeyPressed) x+=5;
        RSGL::drawLine({200,200},{400,400},{200,0,0});
        //RSGL::drawImage("logo.png",{x,200,200,200},false);
    } window.close();
} 