#include "../RSGL.hpp"
bool running = true;

int main(){
    RSGL::window window("name",{500,500,500,500},{255,255,255});
    RSGL::color c = {255,0,0};
    RSGL::circle cir = {100,100,50};
    while (running){
        window.checkEvents();
        switch(window.event.type){
            case RSGL::quit: running=false;
            default: break;
        }
        RSGL::drawCircle({200,200,25},{0,0,0},false);
        RSGL::drawCircle(cir,c);
    } window.close();
} 