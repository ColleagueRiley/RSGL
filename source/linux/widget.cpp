#include "../../include/linux/rsgl.hpp" // source headers

/* 
    these functions should all be RSGL on RSGL, 
    and therefor (hopefully) should work on all systems 
    assuming the basic functions stay the same

    - Colleague Riley 
*/

void RSGL::splashScreen(std::string file,std::string font,int t,RSGL::window win){
    bool running = true; // if the program is still running
    RSGL::color c = win.color; // save the current background color
    
    if (t < 100) t = 100; // time be at least 100
    
    win.setColor({0, 0, 0}); // change the background to black
    
    while (running){
        win.checkEvents();  
        if (RSGL::timer(t)) 
            running = false; // end when the time is up

        if (win.event.type == RSGL::quit){
            // if the window is closed, close the window/display and kill the program so there aren't any issues
            win.close();
            exit(0); 
        }
        
        RSGL::drawText("Powered With", {win.r.w/6, win.r.h/9, win.r.w/20}, font.data(), {255,0,0}); // draw "powered with" right above RSGL 
        RSGL::drawImage(file, {win.r.w/6, win.r.h/6, (int)(win.r.w/1.5), (int)(win.r.h/1.5)}); // draw the image in the right area on the screen

        win.clear();
    } 
    
    win.setColor(c); // set the color back to it's original color
}


void RSGL::button::draw(RSGL::window win){
    switch (s){
        case RECT: 
            RSGL::drawRect(rect, color, {.win=win}); 
            break;
        
        case CIRCLE: 
            RSGL::drawCircle(cir, color, {.win=win}); 
            break;
        
        case IMAGE:
            RSGL::drawImage(File, imageRect, win);

            // if there is a background rect, draw it
            if (Draw && buttonRect.x+buttonRect.y+buttonRect.h+buttonRect.w) 
                RSGL::drawRect(rect, rectColor, {.win=win});
            
            // else make sure, tell set draw to false, so it doesn't check that again
            else{ 
                Draw = false; 
                buttonRect = imageRect; // and make sure the rect is right
            } 

            break;
        
        case TEXT: 
            RSGL::drawRect(rect, rectColor, {.win=win}); 
            RSGL::drawText(Text, textCir, font, color); 
            break;
        default: break;
    }
}

bool RSGL::button::collideWithMouse(RSGL::window win){
    // if it has a rect area, use rect collision
    if ( s == RECT || s == TEXT || s == IMAGE ){ 
        // make sure the rects are right
        if (!rect.x && imageRect.x ) 
            rect = imageRect; 
        
        if (!rect.x && buttonRect.x) rect = buttonRect;

        imageRect=rect; 
        buttonRect=rect;
        
        // check 
        return RSGL::RectCollidePoint(buttonRect, win.event.mouse); 
    }
    
    // if it has a circle area, use circle collision
    if (s == CIRCLE) 
        return RSGL::CircleCollidePoint(cir, win.event.mouse); 
    
    // if it has neither :(
    return false;
}

void RSGL::button::switchTo(int next){
    // make sure it can be switched
    if ( !(next < 0 || next > 3 || states.size() < next || states.empty()) ){
        button Next = states.at(next); // grab the next button

        // switch over the data ( using this = Next wouldn't switch everything over right )
        s = Next.s;

        // rights
        rect = Next.rect; 
        imageRect = Next.imageRect; 
        buttonRect = Next.buttonRect;  
        
        // cirs
        cir = Next.cir; 
        textCir = Next.textCir;
        
        // font / file info
        font = Next.font;  
        File = Next.File, 
        Text = Next.Text;
        
        // colors
        color = Next.color, rectColor = Next.rectColor;
        color = Next.color; 
        
        Draw = Next.Draw;
    }
}

void RSGL::button::checkEvents(RSGL::window win){
    switch (win.event.type){
        case RSGL::mouseButtonPressed: 
            if (collideWithMouse(win) && win.event.button == 1){  
                event = 1; // send the event 
                pressed=true;  // say it's pressed 
                switchTo(1); // switch to that event's button
            } 
            break;
        case RSGL::mouseButtonReleased: 
            if (collideWithMouse(win) && win.event.button == 1){ 
                event = 2; 
                pressed=false; // say it's not pressed
                switchTo(2); 
            } 
            break;
        case RSGL::mousePosChanged: 
            if (collideWithMouse(win) && !pressed){  
                event = 3; 
                switchTo(3); 
            } break;
        default:  
            // switch to idle if there's no event
            if (!collideWithMouse(win)){
                event = 0; 
                switchTo(0);
            } 
            break;
    }
}

// functional version for the hippies
void RSGL::button::run(RSGL::window win){
    checkEvents(win); // check the event
    switch(event){
        case 1: 
            functions.at(2)(); // run the proper function per event
            break;
        case 2: 
            functions.at(3)(); 
            break;
        case 3: 
            functions.at(1)(); 
            break;
        default: 
            functions.at(0)(); 
            break;
    }

    draw(win); // draw on the proper window
}

void RSGL::button::checkAndDraw(RSGL::window win){ 
    checkEvents(win); // check ..
    draw(win);  // and draw
}

void RSGL::checkBox::draw(RSGL::window win){
    if (File.empty()) 
        RSGL::drawRect(rect, pressed ? c2 : c1, {.win=win}); // draw rect with the color based on if it's pressed or not
    else 
        RSGL::drawImage(pressed ? File2 : File, rect, win); // draw the image based on if it's pressed or not
}

void RSGL::checkBox::checkEvents(RSGL::window win){
    // if it's pressed, switch
    if (win.event.type == RSGL::mouseButtonPressed && RSGL::RectCollidePoint(rect, win.event.mouse)  && win.event.button == 1)
        pressed = !pressed;
}

void RSGL::checkBox::checkAndDraw(RSGL::window win){ 
    checkEvents(win); 
    draw(win); 
}

void RSGL::progressBarAuto(RSGL::rect r, RSGL::color c, int speed, int ticks, int dir, RSGL::color c2){
    static int pos = 0; // static pos for adding to

    RSGL::drawRect(r, c); // draw rect 1
    
    // draw rect 2 based on the pos and dir 
    if (dir == -1) 
        RSGL::drawRect({r.x + (r.w - pos), r.y, pos+1, r.h}, c2);

    else if (dir == 1) 
        RSGL::drawRect({r.x, r.y, pos+1, r.h}, c2);
    
    if((r.x + r.w-pos) > r.x && RSGL::timer(ticks)) 
        pos += speed; // make the pos go up by the speed per tick
}

void RSGL::progressBar(RSGL::rect r, RSGL::color c, int pos,int dir, RSGL::color c2){
    RSGL::drawRect(r, c);

    // get the actual pos if it is too little
    while ((r.x + (r.w - pos)) < r.x) 
        pos--;
    
    // deja vu?
    if (dir == -1) 
        RSGL::drawRect({r.x + r.w - pos, r.y, pos+1, r.h}, c2);
    
    else if (dir == 1) 
        RSGL::drawRect({r.x, r.y, pos+1, r.h}, c2);
}

RSGL::mouseMenu::mouseMenu(RSGL::rect r,RSGL::color c, std::vector<menu> ms,RSGL::window win){
    // add the data
    int yb = -(0.15 * r.h);
    rect = r; 
    color = c; 
    menus = ms;

    for (int i=0; i < ms.size(); i++){
        // edit the button and add it to the list
        RSGL::button b = { 
                ms.at(i).text.c_str(), 
                ms.at(i).font, 
                // ajust rect sizes
                {r.x + 10, r.y + ( r.h / (int)(ms.size()/1.25) ) * (i + 1) + yb, r.h / (int)(ms.size() / 1.5)}, 
                {r.x, r.y + r.h / (int)(ms.size()/1.25) * i, r.w, r.h/ (int)(ms.size()/1.25)},  
                ms.at(i).c, 
                {0, 0, 0, 0} 
        }; 

        bs.insert(bs.end(), b);
    }
}   

int RSGL::mouseMenu::draw(RSGL::window win){

    // if it's pressed and hasn't been logged yet, say it's pressed and log the current mouse point
    if ( !pressed && win.event.type == RSGL::mouseButtonPressed && win.event.button == RSGL::mouseRight && RSGL::timer(6)){ 
        pressed= true; 
        p = win.event.mouse; 
    }
    
    else if (pressed){
        // change the rect x/y to the mouse x/y
        rect = {p.x, p.y, rect.w, rect.h};

        RSGL::drawRect(rect,color); // draw the rect
        
        for (int i=0; i < bs.size(); i++){ 
            std::vector<menu> ms=menus; // the menus
            int yb = -(0.15 * rect.h); // the button y

            bs.at(i).checkAndDraw(); // check&draw the current button

            if (bs.at(i).event == 2 && RSGL::timer(6)){ 
                pressed=false; // flip pressed
                return i+1; // return that bs[i] was pressed
            }
            if (bs.at(i).event == 3)  // change the color if it's hovered
                menus.at(i).c2 = ms.at(i).bgcolor; 

            else if (
                bs.at(i).event ||  
                !RSGL::RectCollidePoint(
                        {rect.x, rect.y + rect.h/(int)(ms.size()/1.25) * (i + 1) + yb, rect.w, rect.h / (int)(ms.size() / 1.25)}
                        ,win.event.mouse)
            ) 
                menus.at(i).c2 = {0,0,0,0};
            
            // update the button
            bs.at(i) = {
                    ms.at(i).text.c_str(), ms.at(i).font, 
                    {rect.x + 10, rect.y + (int)( rect.h / ms.size() / 1.25) * (i + 1) + yb, rect.h / (int)(ms.size() / 1.5)}, 
                    {rect.x, rect.y + rect.h/(int)(ms.size()/1.25) * (i + 1) + yb, rect.w, rect.h / (int)(ms.size() / 1.25)},
                    ms.at(i).c, ms.at(i).c2 
            };

        }

        if (win.event.type == RSGL::mouseButtonReleased && RSGL::timer(12)) 
            pressed=false; // if the button is released, unpress
    }

    return 0;
}

RSGL::sideMenu::sideMenu(RSGL::rect r,RSGL::color c, std::vector<menu> ms,RSGL::window win){
    for (int i=0; i < ms.size(); i++){
        int yb=-(0.15*r.h);
        rect=r; color = c; menus=ms;
        RSGL::button b={ms.at(i).text.c_str(), ms.at(i).font, {r.x+10,r.y+ (int)(r.h/ms.size()/1.25)*(i+1)+yb,r.h/ (int)(ms.size()/1.5)}, {r.x,r.y+ (int)(r.h/ms.size()/1.25)*i,r.w, (int)(r.h/ms.size()/1.25)}, ms.at(i).c, {0,0,0,0} }; 
        bs.insert(bs.end(), b);
    }
}   

int RSGL::sideMenu::draw(RSGL::window win){
    RSGL::drawRect(rect, color);
    for (int i=0; i < bs.size(); i++){ 
        std::vector<menu> ms=menus; // vector of menus 
        int yb = -(0.15 * rect.h); // the button y

        bs.at(i).checkAndDraw(); // check&draw the current button

        if (bs.at(i).event == 2 && RSGL::timer(6)){ 
            return i+1; // return that bs[i] was pressed
        }
        if (bs.at(i).event == 3)  // change the color if it's hovered
            menus.at(i).c2 = ms.at(i).bgcolor; 

        else if (
            bs.at(i).event ||  
            !RSGL::RectCollidePoint(
                    {rect.x, rect.y + rect.h/(int)(ms.size()/1.25) * (i + 1) + yb, rect.w, rect.h / (int)(ms.size() / 1.25)}
                    ,win.event.mouse)
        ) 
            menus.at(i).c2 = {0,0,0,0};
        
        // update the button
        bs.at(i) = {
                ms.at(i).text.c_str(), ms.at(i).font, 
                {rect.x + 10, rect.y + (int)( rect.h / ms.size() / 1.25) * (i + 1) + yb, rect.h / (int)(ms.size() / 1.5)}, 
                {rect.x, rect.y + rect.h/(int)(ms.size()/1.25) * (i + 1) + yb, rect.w, rect.h / (int)(ms.size() / 1.25)},
                ms.at(i).c, ms.at(i).c2 
        };
    }
    
    return 0;
}

bool RSGL::timer(int ticks){
    static int tick = 0; // static tick for waiting 
    
    if (tick >= ticks){
        // if the ticks have been passed, return true and 1 out the ticks 
        tick=0; 
        return true; 
    }

    // else add to the ticks and return false
    tick++; 
    return false;
}

RSGL::listEvent RSGL::list::draw(RSGL::window win){
    RSGL::drawRect(r, bg);

    for (int i=0; i < list.size(); i++){ 
        // create the button and check/draw it
        RSGL::button b({r.x, 4 + r.y + i * (int)(r.h/(list.size()*2) + 2) ,r.h , (int)(r.h/(list.size()) * 2) - 2},{0, 0, 0, 0});
        b.checkAndDraw();

        if (b.event || (pressed && pressed-1 == i)) { 
            // update the button
            b = {{r.x, 4 + r.y + i * (int)(r.h / (list.size()*2) + 2), r.h, (int)(r.h / (list.size() * 2)) - 2}, fc}; 
            b.draw();
        }

        if (RSGL::RectCollidePoint({r.x, 4 + r.y + i * (int)(r.h/ (list.size() * 2) +2), r.h, (int)(r.h / (list.size() * 2)) -2},win.event.mouse) 
        && win.event.type == RSGL::mouseButtonReleased)    
            pressed=i+1; 
    
        else if (win.event.type == RSGL::mouseButtonReleased && i == pressed-1) 
            pressed=0;
        
        if (i == pressed-1 && win.isPressed("Up") && RSGL::timer(6) ){ 
            if (pressed > 1) 
                pressed--; 
 
            else 
                pressed = list.size(); 
        }
        
        if (i == pressed-1 && win.isPressed("Down") && RSGL::timer(6)){ 
            if (pressed < list.size()) 
                pressed++; 
 
            else 
                pressed=1;  
        }
        
        RSGL::drawText(list.at(i),{r.x, r.y + i* (int)(r.h / (list.size()*2)+ 2), (int)(r.h / (list.size()*2)) }, font, c, {.win = win});
    }
    
    RSGL::drawRect(r, oc, {.fill = false, .win = win});
    return {(pressed > 0), pressed - 1};
}