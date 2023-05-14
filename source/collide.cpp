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

#ifdef WINE
#include <wine/math.h>
#else
#include <math.h>
#endif

namespace RSGL {
  int circleCollide(circle cir,circle cir2){
    float distanceBetweenCircles = (float) sqrt(
      (cir2.x - cir.x) * (cir2.x - cir.x) + 
      (cir2.y - cir.y) * (cir2.y - cir.y)
    );

    return !(distanceBetweenCircles > (cir.d/2) + (cir2.d/2)); // check if there is a collide
  }

  int circleCollideRect(circle c, rect r){
    // test cords
    float testX = c.x; 
    float testY = c.y;

    // fill cords based on x/ys of the shapes
    if (c.x < r.x)
      testX = r.x;  

    else if (c.x > r.x+r.w) 
      testX = r.x-r.w;

    if (c.y < r.y)  
      testY = r.y;  

    else if (c.y > r.y+r.h)
      testY = r.y+r.h; 
    
    // check
    return ( sqrt( ( (c.x - testX) * (c.x - testX) ) + ( (c.y - testY) * (c.y - testY) ) )  <= (c.d/2) );
  }


  int circleCollidePoint(circle c, point p){
      return circleCollideRect(c, (rect){p.x, p.y, 1, 1});
  }

  int circleCollideLine(circle c,point p1, point p2) {
    float distX = p1.x - p2.x;
    float distY = p1.y - p2.y;
    float len = sqrt( (distX * distX) + (distY * distY) );

    float dot = ( ((c.x-p1.x)*(p2.x-p1.x)) + ((c.y-p1.y)*(p2.y-p1.y)) ) / pow(len,2);
    distX = (float)(p1.x + (dot * (p2.x-p1.x))) - c.x;  distY = (float)(p1.y + (dot * (p2.y-p1.y))) - c.y;
    if ((float)(sqrt( (distX*distX) + (distY*distY) )) <= (c.d/2)) return true; return false;
  }

  int rectCollidePoint(rect r, point p){
      return (p.x >= r.x &&  p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h);
  }

  int pointCollide(point p, point p2){
    return (p.x == p2.x && p.y == p2.y);
  }

  int rectCollideRect(rect r, rect r2){
      return (r.x + r.w >= r2.x && r.x <= r2.x + r2.w && r.y + r.h >= r2.y && r.y <= r2.y + r2.h);
  }

  int imageCollideRect(image img, rect r){
      return 0; // not done yet
  }

  int imageCollideCircle(image img, circle c){ 
    return 0; // not done yet
  }

  int imageCollidePoint(image img, point p){
    return 0; // not done yet
  }

  int imageCollideImage(image img, image img2){
    return 0; // not done yet
  }
}