#include "../../include/linux/rsgl.hpp" // source headers

void RSGL::splashScreen(std::string file,std::string font,int t,RSGL::window win){
    static bool running=true;
    RSGL::color c = win.color; if (t < 100) t=100;
    int cl=0;
    while (running){
        win.checkEvents();  win.setColor({0,0,0}); cl++;
        if (cl==t) running=false;
        switch(win.event.type){
            case RSGL::quit: running=false; win.close(); break;
            default: break;
        }
        RSGL::drawImage(file,(RSGL::rect){win.r.width/6,win.r.length/6,win.r.width/1.5,win.r.length/1.5});
        RSGL::drawText("Powered With",{win.r.width/6,win.r.length/9,win.r.width/20},font.data(),{255,0,0});
        win.clear();
    } win.setColor(c); running=false;
}


void RSGL::Button::draw(RSGL::window win){
    switch (s){
        case RECT: RSGL::drawRect(rect,color,true,false,win); break;
        case CIRCLE: RSGL::drawCircle(cir,color,true,win); break;
        case IMAGE:
            RSGL::drawImage(File,imageRect,win);
            if (buttonRect.x+buttonRect.y+buttonRect.length+buttonRect.width && Draw) RSGL::drawRect(rect,rectColor,true,false,win);
            else{ Draw=false; buttonRect=imageRect; } break;
        case TEXT: RSGL::drawRect(rect,rectColor,true,false,win); RSGL::drawText(Text,textCir,font,color); break;
        default: break;
    }
}

bool RSGL::Button::collideWithMouse(RSGL::window win){
    if ( s == RECT || s == TEXT || s == IMAGE ){ 
        if (!rect.x && imageRect.x ) rect=imageRect; if (!rect.x && buttonRect.x) rect=buttonRect;
        imageRect=rect; buttonRect=rect;
        return RSGL::RectCollidePoint(buttonRect,win.event.mouse); 
    }
    if ( s == CIRCLE ) return RSGL::CircleCollidePoint(cir,win.event.mouse); 
}

void RSGL::Button::switchTo(int next){
    if (!(next < 0 || next > 3 || states.size() < next || states.empty()) ){
        Button Next = states.at(next);
        s=Next.s; 
        rect=Next.rect; imageRect=Next.imageRect; buttonRect=Next.buttonRect;  
        cir=Next.cir; textCir=Next.textCir;
        font=Next.font;  
        File=Next.File, Text=Next.Text;
        color=Next.color, rectColor=Next.rectColor;
        color=Next.color; Draw=Next.Draw;
    }
}

void RSGL::Button::checkEvents(RSGL::window win){
    switch (win.event.type){
        case RSGL::MouseButtonPressed: 
            if (collideWithMouse(win) && win.event.button == 1){  event = 1; pressed=true; switchTo(1); } break;
        case RSGL::MouseButtonReleased: 
            if (collideWithMouse(win) && win.event.button == 1){ event = 2; pressed=false; switchTo(2); } break;
        case RSGL::MousePosChanged: 
            if (collideWithMouse(win) && !pressed){  event = 3; switchTo(3); } break;
        default: 
            if (!collideWithMouse(win)){event = 0; switchTo(0);} break;
    }
}

void RSGL::Button::run(RSGL::window win){
    checkEvents(win);
    switch(event){
        case 1: functions.at(2)(); break;
        case 2: functions.at(3)(); break;
        case 3: functions.at(1)(); break;
        default: functions.at(0)(); break;
    }
    draw(win); 
}

void RSGL::Button::checkAndDraw(RSGL::window win){ checkEvents(win); draw(win); }

void RSGL::CheckBox::draw(RSGL::window win){
    if (File.empty()) RSGL::drawRect(rect, pressed ? c2 : c1 );
    else RSGL::drawImage(pressed ? File2 : File,rect);
}

void RSGL::CheckBox::checkEvents(RSGL::window win){
    if (win.event.type == RSGL::MouseButtonPressed && RSGL::RectCollidePoint(rect,win.event.mouse)  && win.event.button == 1) pressed=!pressed;
}


void RSGL::CheckBox::checkAndDraw(RSGL::window win){ checkEvents(win); draw(win); }

void RSGL::scrollText(std::string text, RSGL::circle r, RSGL::Font font, RSGL::color col, int speed, int stopY, RSGL::window win){
    int nl=0;
    for (int i=0; i < text.size(); i++) if (text.at(i) == '\n') nl++; 
    static RSGL::circle c = r; 
    if (stopY == -1) stopY=nl;
    if ( abs(abs(c.y-r.y)/nl) <= stopY ) c.y--;
    RSGL::drawText(text,{c.x,c.y,c.radius},font,{255,0,0});
}

void RSGL::progressBarAuto(RSGL::rect r, RSGL::color c, int speed, int ticks, int dir, RSGL::color c2){
    static int tick=0;
    static int pos=0; 
    RSGL::drawRect(r,c);
    if (dir==-1) RSGL::drawRect({r.x+r.width-pos,r.y,1+pos,r.length},c2);
    if (dir==1) RSGL::drawRect({r.x,r.y,1+pos,r.length},c2);
    if(r.x+r.width-pos > r.x && ticks == tick) pos+=speed; 
    if (ticks != tick) tick++; else tick=0; 
}

void RSGL::progressBar(RSGL::rect r, RSGL::color c, int pos,int dir, RSGL::color c2){
    RSGL::drawRect(r,c);
    while (r.x+r.width-pos < r.x ) pos--;
    if (dir==-1) RSGL::drawRect({r.x+r.width-pos,r.y,1+pos,r.length},c2);
    if (dir==1) RSGL::drawRect({r.x,r.y,1+pos,r.length},c2);
}

void RSGL::drawLink(std::string url,RSGL::circle c,RSGL::Font font,RSGL::color col,std::string text,bool underLine,RSGL::window win){
    if (text.empty()) text=url;
    RSGL::drawText(text,c,font,col,win);
    if (underLine) RSGL::drawRect({c.x,c.y+c.radius,text.size()*(c.radius/1.87),1},col);
    if (win.event.type == RSGL::MouseButtonPressed && RSGL::RectCollidePoint({c.x,c.y,text.size()*(c.radius/1.87),c.radius},win.event.mouse)){
        std::string cmd = "xdg-open " + url; system(cmd.c_str());
    }
}

void RSGL::drawTypingText(std::string text,RSGL::circle c,RSGL::Font font,RSGL::color col,int speed,RSGL::window win){
      static int addTick=0; static std::string outputText="";
      if (addTick >= speed && text.size() != outputText.size()){ addTick=0; outputText += text.at(outputText.size());}
      RSGL::drawText(outputText,c,font,col);
      addTick++;
}

RSGL::mouseMenu::mouseMenu(RSGL::rect r,RSGL::color c, std::vector<Menu> ms,RSGL::window win){
    for (int i=0; i < ms.size(); i++){
        int yb=-(0.15*r.length);
        rect=r; color = c; menus=ms;
        RSGL::Button b={ms.at(i).text.c_str(), ms.at(i).font, {r.x+10,r.y+(r.length/ms.size()/1.25)*(i+1)+yb,r.length/ms.size()/1.5}, {r.x,r.y+(r.length/ms.size()/1.25)*i,r.width,(r.length/ms.size()/1.25)}, ms.at(i).c, {0,0,0,0} }; 
        bs.insert(bs.end(), b);
    }
}   

int RSGL::mouseMenu::draw(RSGL::window win){
    if (win.event.type == RSGL::MouseButtonPressed && win.event.button==RSGL::mouseRight && tick >= 6 && !pressed ){ tick=0; pressed=true; p=win.event.mouse; }
    if (pressed){
        rect = {p.x,p.y,rect.width,rect.length};
        RSGL::drawRect(rect,color);
        for (int i=0; i < bs.size(); i++){ 
            std::vector<Menu> ms=menus; RSGL::rect r=rect; 
            int yb=-(0.15*r.length);
            bs.at(i).checkAndDraw();
            if (bs.at(i).event == 2 && tick >= 6){ pressed=false; return i+1;}
            if (bs.at(i).event == 3) menus.at(i).c2 = ms.at(i).bgcolor;
            else if (bs.at(i).event || !RSGL::RectCollidePoint({r.x,r.y+(r.length/ms.size()/1.25)*(i+1)+yb,r.width,(r.length/ms.size()/1.25)},win.event.mouse)) menus.at(i).c2 = {0,0,0,0};
            bs.at(i) = {ms.at(i).text.c_str(), ms.at(i).font, {r.x+10,r.y+(r.length/ms.size()/1.25)*(i+1)+yb,r.length/ms.size()/1.5}, {r.x,r.y+(r.length/ms.size()/1.25)*(i+1)+yb,r.width,(r.length/ms.size()/1.25)}, ms.at(i).c, ms.at(i).c2 }; 
        }
    } 
    if (win.event.type == RSGL::MouseButtonReleased && tick >= 12 && pressed ){ tick=0; pressed=false; }
    
    tick++; return 0;
}

RSGL::sideMenu::sideMenu(RSGL::rect r,RSGL::color c, std::vector<Menu> ms,RSGL::window win){
    for (int i=0; i < ms.size(); i++){
        int yb=-(0.15*r.length);
        rect=r; color = c; menus=ms;
        RSGL::Button b={ms.at(i).text.c_str(), ms.at(i).font, {r.x+10,r.y+(r.length/ms.size()/1.25)*(i+1)+yb,r.length/ms.size()/1.5}, {r.x,r.y+(r.length/ms.size()/1.25)*i,r.width,(r.length/ms.size()/1.25)}, ms.at(i).c, {0,0,0,0} }; 
        bs.insert(bs.end(), b);
    }
}   

int RSGL::sideMenu::draw(RSGL::window win){
    RSGL::drawRect(rect,color);
    for (int i=0; i < bs.size(); i++){ 
        std::vector<Menu> ms=menus; RSGL::rect r=rect; 
        int yb=-(0.15*r.length);
        bs.at(i).checkAndDraw();
        if (bs.at(i).event == 2 && tick >= 6) return i+1;
        if (bs.at(i).event == 3) menus.at(i).c2 = ms.at(i).bgcolor;
        else if (bs.at(i).event || !RSGL::RectCollidePoint({r.x,r.y+(r.length/ms.size()/1.25)*(i+1)+yb,r.width,(r.length/ms.size()/1.25)},win.event.mouse)) menus.at(i).c2 = {0,0,0,0};
        bs.at(i) = {ms.at(i).text.c_str(), ms.at(i).font, {r.x+10,r.y+(r.length/ms.size()/1.25)*(i+1)+yb,r.length/ms.size()/1.5}, {r.x,r.y+(r.length/ms.size()/1.25)*(i+1)+yb,r.width,(r.length/ms.size()/1.25)}, ms.at(i).c, ms.at(i).c2 }; 
    }
    tick++; return 0;
}

bool RSGL::timer(int ticks){
    static int tick=0;
    if (tick >= ticks){ tick=0; return true; }
    tick++; return false;
}

RSGL::listEvent RSGL::List::draw(RSGL::window win){
    RSGL::drawRect(r,bg);
    for (int i=0; i < list.size(); i++){ 
        RSGL::Button b({r.x,4+r.y+i*(r.length/(list.size()*2)+2),r.length,r.length/(list.size()*2)-2},{0,0,0,0});
        b.checkAndDraw();
        if (b.event || (pressed && pressed-1 == i)){ b = {{r.x,4+r.y+i*(r.length/(list.size()*2)+2),r.length,r.length/(list.size()*2)-2},fc}; b.draw();}
        if (RSGL::RectCollidePoint({r.x,4+r.y+i*(r.length/(list.size()*2)+2),r.length,r.length/(list.size()*2)-2},win.event.mouse) && win.event.type==RSGL::MouseButtonReleased) pressed=i+1; 
        else if (win.event.type==RSGL::MouseButtonReleased && i==pressed-1) pressed=0;
        if (i == pressed-1 && win.isPressed("Up") && RSGL::timer(6) ){ if (pressed > 1) pressed--; else pressed = list.size(); }
        if (i == pressed-1 && win.isPressed("Down") && RSGL::timer(6)){ if (pressed < list.size()) pressed++; else pressed=1;  }
        RSGL::drawText(list.at(i),{r.x,r.y+i*(r.length/(list.size()*2)+2), r.length/(list.size()*2) },font,c,win);
    }
    RSGL::drawRect(r,oc,false);
    return {(pressed > 0),pressed-1};
}