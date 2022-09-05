#include "../../include/linux/rsgl.hpp"
#include <math.h>

int RSGL::CircleCollide(RSGL::circle cir,RSGL::circle cir2){
	float distanceBetweenCircles = (float) sqrt(
	  (cir2.x - cir.x) * (cir2.x - cir.x) + 
    (cir2.y - cir.y) * (cir2.y - cir.y)
  );

	return !(distanceBetweenCircles > cir.radius + cir2.radius); // check if there is a collide
}

int RSGL::CircleCollideRect(RSGL::circle c, RSGL::rect r){
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
  return ( sqrt( ( (c.x - testX) * (c.x - testX) ) + ( (c.y - testY) * (c.y - testY) ) )  <= c.radius );
}


int RSGL::CircleCollidePoint(RSGL::circle c, RSGL::point p){
    return RSGL::CircleCollideRect(c, {p.x, p.y, 1, 1});
}

int RSGL::CircleCollideLine(RSGL::circle c,RSGL::point p1, RSGL::point p2) {
  float distX = p1.x - p2.x;
  float distY = p1.y - p2.y;
  float len = sqrt( (distX * distX) + (distY * distY) );

  float dot = ( ((c.x-p1.x)*(p2.x-p1.x)) + ((c.y-p1.y)*(p2.y-p1.y)) ) / pow(len,2);
  distX = (float)(p1.x + (dot * (p2.x-p1.x))) - c.x;  distY = (float)(p1.y + (dot * (p2.y-p1.y))) - c.y;
  if ((float)(sqrt( (distX*distX) + (distY*distY) )) <= c.radius) return true; return false;
}

int RSGL::RectCollidePoint(RSGL::rect r, RSGL::point p){
    return (p.x >= r.x &&  p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h);
}

int RSGL::PointCollide(RSGL::point p, RSGL::point p2){
  return (p.x == p2.x && p.y == p2.y);
}

int RSGL::RectCollideRect(RSGL::rect r, RSGL::rect r2){
    return (r.x + r.w >= r2.x && r.x <= r2.x + r2.w && r.y + r.h >= r2.y && r.y <= r2.y + r2.h);
}

int RSGL::ImageCollideRect(RSGL::image img, RSGL::rect r){
    return 0; // not done yet
}

int RSGL::ImageCollideCircle(RSGL::image img, RSGL::circle c){ 
  return 0; // not done yet
}

int RSGL::ImageCollidePoint(RSGL::image img, RSGL::point p){
  return 0; // not done yet
}

int RSGL::ImageCollideImage(RSGL::image img, RSGL::image img2){
  return 0; // not done yet
}
