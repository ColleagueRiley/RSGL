#include "RSGL.hpp"
bool running = true;

RSGL::window window("name",{500,500,500,500},{255,255,255});
bool pressed=false;
std::vector<RSGL::circle> dots;
int main(){ 
    bool pressed=false;
    while (running){
        window.checkEvents(); 
        window.clear();
        switch (window.event.type){
            case RSGL::quit:  running = false; break;
            case RSGL::MouseButtonPressed: pressed = true;  break;
            case RSGL::MouseButtonReleased: pressed = false; break;
            default: break;
        }
        if (pressed) dots.insert(dots.end(),{window.event.x,window.event.y,25});
        for (int i=0;  i < dots.size(); i++) RSGL::drawCircle(dots.at(i),{0,0,0});
    } window.close();
}