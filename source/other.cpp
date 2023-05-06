/*
* Copyright (c) 2021-23 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/

#ifndef RSGL_H
#include "../include/RSGL.hpp" 
#endif


bool RSGL::timerT(int ticks){
    static int i = 0, tickLimit = 0;
    
    if (ticks > tickLimit)
        tickLimit = ticks;

    if (i > tickLimit)
        i = 0;

    i++;

    return !(i % ticks);
}

bool RSGL::timerM(int milliseconds) {
    static int start_time = time(0), miliLimit = 0;

    if (milliseconds > miliLimit)
        miliLimit = milliseconds;

    int passed = time(0) - start_time;

    if (passed > miliLimit)
        start_time = time(0);

    return !(passed % milliseconds) && passed;
}