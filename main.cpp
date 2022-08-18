#include "RSGL.hpp"
bool running = true;


RSGL::window win("name",{500,500,500,500},{255,255,255});

bool isIn(std::string c, std::string s){
    return s.find_first_of(c) > s.size();
}

//std::vector<RSGL::Font> fonts;
struct output {bool returned=false; std::string output; };
output textBox(RSGL::rect box,int textSize,RSGL::Font font,RSGL::color col,std::string placeHolder="",bool initText=false,RSGL::window win=RSGL::root){
    static std::string text=placeHolder;
    static bool focused=false;
    static int onLine=0;
    std::string keys = "`1234567890-=\\qwertyuiop[]asdfghjkl;'zxcvbnm,./~!@#$%^&*()_+|{}:\"<>?QWERTYUIOPASDFGHJKLZXCVBNM";
    int max = (box.width*1.5)/textSize;
    if (win.event.type == RSGL::MouseButtonPressed && RSGL::RectCollidePoint(box,win.event.mouse)) focused=true;  
    if (win.event.type == RSGL::MouseButtonPressed && !RSGL::RectCollidePoint(box,win.event.mouse)) focused=false;
       
    if (focused && win.event.type == RSGL::KeyPressed){
        std::string key=win.event.key;
        if (text==placeHolder && !initText) text="";
        if (key == "Return" && win.isPressed("Shift")){ key = "\n"; onLine=-1; }
        else if (key == "Return") return {true,text};
        if (key == "Tab") key = "      ";
        if (key == "\n" || key == "      " || isIn(key,keys)){
            if (!win.isPressed("Shift") && isIn(key,"ABCDEFGHIJKLMNOPQRSTUVWXYZ")) key = std::tolower(key.at(0));
            else if (isIn(key,"ABCDEFGHIJKLMNOPQRSTUVWXYZ") || (!win.isPressed("Shift") && isIn(key,keys))) key = key.at(0) ;
            if (onLine < max){ text+=key; onLine++; }
        }
    }
    RSGL::drawRect(box,{0,0,0},false);
    RSGL::drawText(text,{box.x,box.y+(textSize/4),textSize},font,col);
    return {false,""};
}

struct Menu{std::string text, font, image; RSGL::color c, bgcolor, c2={0,0,0,0};};


struct topMenu{
    topMenu(){}
    topMenu(RSGL::rect r,RSGL::color c, std::vector<Menu> m, RSGL::window win=RSGL::root);
    RSGL::listEvent draw(RSGL::window win=RSGL::root);
    private:
        RSGL::color color; RSGL::rect rect;
        std::vector<Menu> menus;
        std::vector<RSGL::Button> bs;
        int tick=0, pressed=0;
};


topMenu::topMenu(RSGL::rect r,RSGL::color c, std::vector<Menu> ms,RSGL::window win){
    for (int i=0; i < ms.size(); i++){
        int yb=-(0.15*r.length);
        rect=r; color = c; menus=ms;
        RSGL::Button b={ms.at(i).text.c_str(), ms.at(i).font, {r.x+10+(r.length/ms.size()/1.25)*i,r.y,r.length/ms.size()/1.5}, {r.x+10+(r.length/ms.size()/1.25)*i,r.y,r.length/ms.size()/1.5}, ms.at(i).c, {0,0,0,0} }; 
        bs.insert(bs.end(), b);
    }
}   

RSGL::listEvent topMenu::draw(RSGL::window win){
    RSGL::rect r = rect;  std::vector<Menu> list = menus;
    RSGL::drawRect(rect,color);   for (int i=0; i < menus.size(); i++){ 
        RSGL::Button b({r.x+i*(r.length/(menus.size()*2)+2), r.y+4 ,r.length,r.length/(menus.size()*2)-2},{0,0,0,0});
        b.checkAndDraw();
        if (b.event || (pressed && pressed-1 == i)){ b = {{r.x+i*(r.length/(menus.size()*2)+2), r.y+4 ,r.length,r.length/(menus.size()*2)-2},menus.at(i).c2}; b.draw();}
        if (RSGL::RectCollidePoint({r.x+i*(r.length/(menus.size()*2)+2), r.y+4 ,r.length,r.length/(menus.size()*2)-2},win.event.mouse) && win.event.type==RSGL::MouseButtonReleased) pressed=i+1; 
        else if (win.event.type==RSGL::MouseButtonReleased && i==pressed-1) pressed=0;
        RSGL::drawText(list.at(i).text,{r.x,r.y+i*(r.length/(list.size()*2)+2), r.length/(list.size()*2) },menus.at(i).font,menus.at(i).c,win);
     
        RSGL::rect c = {r.x+i*(r.length/(menus.size()*2)+2), r.y+4 ,list.at(i).text.size()*r.length/(menus.size()*2) ,r.length/(menus.size()*2)-2};
        RSGL::drawRect(c,{255,0,0});
    }
    return {(pressed > 0),pressed-1};
}

int main(){
    topMenu mm = {{50,50,200,55},{100,100,100},{{"hi","/usr/share/fonts/TTF/DejaVuSans.ttf","",{0,0,0},{255,0,0}}, {"bye","/usr/share/fonts/TTF/DejaVuSans.ttf","",{0,0,0},{255,0,0}} }};
    while (running){
        win.checkEvents();
        RSGL::listEvent event = mm.draw();
        win.clear(); 
    } win.close();
}