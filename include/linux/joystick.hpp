#pragma once
#ifdef RSGLBROKENJOY
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <iostream>
#include <vector>

namespace RSGL{
    #ifndef RSGLRGBTOHEX
    struct point{int x,y;};
    #endif
    const int joystickUnplugged=0;
    const int joystickButtonPressed=1;
    const int joystickButtonReleased=2;
    const int joystickAxisChanged=3;

    class joystick{
        struct Event{
            int type,button;
            struct RSGL::point axes[3] = {0};
            size_t axis;
        };

        size_t get_axis_state(struct js_event *event, struct RSGL::point axes[3]);

        public:        
            int js;
            Event event;

            joystick(std::string Device, int global=0);
            joystick(int jsx,int global=0);
            joystick();

            void close();
            void checkEvents();
    };  
}

#ifndef RSGLJOYSTICKNOSRC
RSGL::joystick::joystick(int jsx,int global){ joystick("/dev/input/js"+std::to_string(jsx),global); }
RSGL::joystick::joystick(){}
void RSGL::joystick::close(){ ::close(js); }

size_t RSGL::joystick::get_axis_state(struct js_event *event, struct RSGL::point axes[3]){
    size_t axis = event->number / 2;

    if (axis < 3)
    {
        if (event->number % 2 == 0)
            axes[axis].x = event->value;
        else
            axes[axis].y = event->value;
    }

    return axis;
}

void RSGL::joystick::checkEvents(){
    struct js_event e; 
    if (read(js, &e, sizeof(e)) != sizeof(e)) perror("Could not read controller events");
    switch(e.type){
        case 0: event.type = 0;
        case JS_EVENT_BUTTON: 
            event.type = e.value ? 1 : 2; event.button=e.number; break;
        case JS_EVENT_AXIS: event.type=3; event.axis = get_axis_state(&e, event.axes); break;
        default: break; event.type = -1; break;
    }
}
#else
RSGL::joystick::joystick(std::string Device, int global){
    js = open(Device.c_str(), O_RDONLY);
    if (js == -1) perror("Could not open joystick");
//    if (global) joysticks.insert(joysticks.end(),this);
}
#endif
#endif