#include "RSGL.hpp"
bool running = true;
RSGL::window window("",{500,500,500,500},{0,0,0});

struct textBox{
    RSGL::rect r; int size; const char* font; RSGL::color c; std::string text; RSGL::drawable d=RSGL::root;
    void draw();
    bool keyInit=false; int len=0,ylen=0;
    void listen();
    bool pressed=false; bool clicked=false;
    bool isClicked();
};

void textBox::listen(){
    int maxlen = r.length/size-2;
    if (window.event.type == RSGL::KeyPressed && len >= maxlen){len=0; text+='\n'; ylen++;}
    if (window.event.type == RSGL::KeyPressed && !keyInit) keyInit=true;
    if (ylen >= r.width/size-(size/4)){}
    else if (keyInit && window.event.type == RSGL::KeyReleased && window.event.key == "Return"){ text+="\n"; len=0; ylen++;}
    else if (keyInit && window.event.type == RSGL::KeyReleased && window.event.key == "Return"){ text+="   "; len+=3;}
    else if (keyInit && window.event.type == RSGL::KeyReleased && window.event.key == "Space"){ text+=" "; len++;}
    else if (keyInit && window.event.type == RSGL::KeyReleased && window.event.key == "BackSpace" && text.size() > 0){ if (text.at(text.size()-1) == '\n'){ len=maxlen;ylen--;} text.erase(text.end()-1); len--;}
    else if (keyInit && window.event.type == RSGL::KeyReleased && window.event.key != "Control_L" && window.event.key != "Control_R" &&  window.event.key != "Caps_Lock"){ 
        if (window.isPressed("Shift_L") || window.isPressed("Shift_R") || window.event.ledState == 3)text+=window.event.key;
        else  text+=std::tolower(window.event.key.at(0)); len++; 
    }
}

void textBox::draw(){
    if (text.size() > 0){ RSGL::drawText(text,{r.x,r.y,size},font,c,d); XFlush(d.display);}
}

bool textBox::isClicked(){
    if (window.event.type == RSGL::MouseButtonPressed && RSGL::RectCollidePoint(r,{window.event.x,window.event.y})) pressed=true;
    if (window.event.type == RSGL::MouseButtonReleased  && RSGL::RectCollidePoint(r,{window.event.x,window.event.y}) && pressed) clicked=true;
    if (window.event.type == RSGL::MouseButtonReleased  && !RSGL::RectCollidePoint(r,{window.event.x,window.event.y})) clicked=false;
    if (clicked) return true;
    return false;
}

int main(int argc, char **argv){
    textBox box = {{100,100,400,400},25,"a.ttf",{255,255,255}}; 

    while (running){
        window.checkEvents(); window.clear();
        if (window.event.type == RSGL::quit) running=false;
        if (box.isClicked()) box.listen();
        RSGL::drawRect({100,100,400,400},{255,0,0},false);
        
        box.draw();
    } window.close();
}