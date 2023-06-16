#ifndef RSGL_H
#include "RSGL.hpp"
#endif

#include <stdio.h>
#include <cstdlib>
#include <cstring>
 
namespace RSGL {
    buttonEvent& button::checkEvent(RSGL::event e) {
        switch (e.type) {
            case RSGL::mouseButtonPressed:
                if (RSGL::rectCollidePoint(r, e.mouse)) 
                    event = held;
                break;
            case RSGL::mouseButtonReleased:
                if (event == held)
                    event = released;
                break;
            case RSGL::mousePosChanged:
                if ((event == none || event == released) && event != held && RSGL::rectCollidePoint(r, e.mouse)) 
                    event = hovered;
                else if (!RSGL::rectCollidePoint(r, e.mouse))
                    event = none;
                break;
            default: break;
        }

        return event;
    }

    void button::draw(RSGL::color c, drawArgs d) { RSGL::drawRect(r, c, d); }

    int slider::checkEvent(RSGL::event e) {
        tick.checkEvent(e);
        
        if (bar.w > bar.h)
            tick.r.y = bar.y;
        else if (bar.w < bar.h)
            tick.r.x = bar.x;

        static bool isHeld = false;

        if (tick.event == held)
            isHeld = true;
        else if (tick.event == released)
            isHeld = false;

        if (isHeld) {
            if (bar.w > bar.h) {
                tick.r.x = e.mouse.x;       
            }

            else if (bar.w < bar.h) {
                tick.r.y = e.mouse.y;    
            }
        }
        
        if (tick.r.x < bar.x)
            tick.r.x = bar.x;
        if (tick.r.x > bar.x + bar.w - tick.r.w)
            tick.r.x = bar.x + bar.w - tick.r.w;

        if (tick.r.y < bar.y)
            tick.r.y = bar.y;
        if (tick.r.y > bar.y + bar.h)
            tick.r.y = bar.y + bar.h - tick.r.h;

        if (bar.w > bar.h)
            return (((float)(tick.r.x - bar.x)/(float)(bar.w  - tick.r.w))) * 100;
        else if (bar.w < bar.h)
            return (((float)(tick.r.y - bar.y)/(float)(bar.h  - tick.r.h))) * 100;
        
        return 0;
    }
    
    void slider::draw(RSGL::color c, RSGL::color c1, drawArgs d1, drawArgs d2) {
        RSGL::drawRect(bar, c, d1);
        tick.draw(c1, d2);
    }

    void textbox::addChar(char ch, size_t index)  {
        if (textSize == index) {
            int xx = textSize++;
        
            text = (char*)realloc(text, (textSize + 1) * sizeof(char));

            text[textSize] = '\0';
            text[textSize - 1] = ch;
        } 
        else {
            char* oText = (char*)malloc((textSize + 1) * sizeof(char));
            oText[textSize + 1] = 0;
            
            size_t size = 0;

            for (int i = 0; i < textSize; i++, size++) {
                if (i == index) { 
                    oText[size] = ch;

                    size++;
                } 
                
                oText[size] = text[i];
            }  

            free(text);
            text = strdup(oText);
            
            free(oText);
        }

        if (ch != '\n')
            p.x++;
        else {
            p.y++;
            p.x = 0;
        }
    }

    void textbox::eraseChar(size_t index) {
        if (textSize) {
            textSize--;
        
            if (text[textSize] != '\n')
                p.x--;
            else
                p.y--;

            if (index == textSize) {
                text[textSize] = 0;
                text = (char*)realloc(text, (textSize + 1) * sizeof(char));
            } else { 
                char* nText = strdup(text);
                
                free(text);
                text = (char*)malloc(0);
                
                size_t size = textSize;
                textSize = 0;

                for (int i = 0; i < (size + (size != (textSize - 1))); i++) {
                    if (i == index) i++;

                    addChar(nText[i], textSize);
                }

                free(nText);
            }
        }
    }

    void textbox::checkEvent(RSGL::event e, bool tab /*= true*/, bool enterNL /*= true*/) {
        box.checkEvent(e);

        if (box.event == held)
            inUse = true;
        else if (e.type == RSGL::mouseButtonPressed)
            inUse = false;
        
        if (e.type == RSGL::keyPressed && inUse) { 
            bool wrote = false;
            char ch = keyStrToChar((const char*)e.keyName, true);


            if (ch == '\0') {
                if (!strcmp(e.keyName, "Tab") && tab) 
                    for (int i = 0; i < 3; i++) addChar(' ', textSize);

                if (!strcmp(e.keyName, "BackSpace"))
                    eraseChar(textSize - 1);
            } 

            else addChar(ch, textSize); 
        }
    }

    struct textList { size_t size; char** text; };
    textList getTextList(char* text);

    void textbox::draw(RSGL::circle cir, RSGL::font font, RSGL::color col, drawArgs args) {
        int w = 0;
        
        if (textSize) {
            RSGL::drawText(text, cir, col, font, args);

            if (p.x) {
                textList texts = getTextList(text);

                char* lineText = strdup(texts.text[p.y]);

                w = RSGL::textWidth(lineText, font, cir.d);
                
                if (w > box.r.w)
                    eraseChar(textSize - 1);

                free(lineText);
            }
        }
        
        if (inUse)
            RSGL::drawRect((RSGL::rect){cir.x + w, cir.x + (p.y * cir.d), 2, cir.d}, {255, 255, 255});
    }

    textbox::textbox() { text = (char*)malloc(0); textSize = 0; inUse = false; }
    textbox::~textbox(){ free(text); }

    textList getTextList(char* text) {
        size_t textsSize = 1;
        for (int i = 0; i < strlen(text); i++)
            if (text[i] == '\n')
                textsSize++;

        char* texts[textsSize];

        for (int i = 0; i < textsSize; i++) {
            texts[i] = (char*)malloc(strlen(text) * sizeof(char*));

            memset(texts[i], 0, strlen(text) * sizeof(char));
        }

        if (textsSize == 1) 
            texts[0] = (char*)text;

        else {
            int x = 0, y = 0;

            for (int i = 0; i < strlen(text); i++) {
                if (text[i] == '\n') {
                    x++;
                    y = 0;
                }
                else {
                    texts[x][y] = text[i];
                    y++;
                }
            }
        }
    
        return {textsSize, texts};
    }
}