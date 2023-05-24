#ifndef RSGL_H
#include "RSGL.hpp"
#endif


namespace RSGL {
    buttonEvent& button::checkEvent(RSGL::event e) {
        switch (e.type) {
            case RSGL::mouseButtonPressed:
                if (RSGL::rectCollidePoint(r, e.mouse)) 
                    event = held;
                break;
            case RSGL::mouseButtonReleased:
                if (RSGL::rectCollidePoint(r, e.mouse)) 
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
            
        if (tick.event == held) { 
            if (bar.w > bar.h) {
                if (e.type == mousePosChanged)
                    tick.r.x = e.mouse.x;       
            }

            else if (bar.w < bar.h) {
                if (e.type == mousePosChanged)
                    tick.r.x = e.mouse.y;    
            }
        }
    }

    void slider::draw(RSGL::color c, RSGL::color c1, drawArgs d1, drawArgs d2) {
        RSGL::drawRect(bar, c, d1);
        tick.draw(c1, d2);
    }

    void textbox::checkEvent(RSGL::event e) {

    }
    
    void textbox::draw(RSGL::circle cir, RSGL::font font, RSGL::color col, drawArgs args) {

    }
}