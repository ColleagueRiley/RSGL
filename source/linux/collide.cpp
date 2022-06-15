#ifndef RSGL
#include "../../include/include/linux/rsgl.hpp" 
#endif
#include <math.h>

int RSGL::CircleCollide(RSGL::circle cir,RSGL::circle cir2){
	float distanceBetweenCircles = (float)sqrt(
	(cir2.x - cir.x) * (cir2.x - cir.x) + 
    (cir2.y - cir.y) * (cir2.y - cir.y)
  	);
	if (distanceBetweenCircles > cir.radius + cir2.radius){return 0;}else{return 1;}
}

int RSGL::CircleCollideRect(RSGL::circle c, RSGL::rect r){
  float testX = c.x; float testY = c.y;

  if (c.x < r.x) {testX = r.x;}  else if (c.x > r.x+r.width) {testX = r.x-r.width;}
  if (c.y < r.y) {testY = r.y;}  else if (c.y > r.y+r.length) {testY = r.y+r.length;} 
  
  return (sqrt( ( (c.x-testX) * (c.x-testX) ) + ( (c.y-testY) *(c.y-testY) ) )  <= c.radius);
}


int RSGL::CircleCollidePoint(RSGL::circle c, RSGL::point p){
	float testX = c.x; float testY = c.y;

  	if (c.x < p.x) {testX = p.x;}  else if (c.x > p.x+1) {testX = p.x+1;}
  	if (c.y < p.y) {testY = p.y;}  else if (c.y > p.y+1) {testY = p.y+1;} 
  
  	return (sqrt( ( (c.x-testX) * (c.x-testX) ) + ( (c.y-testY) *(c.y-testY) ) )  <= c.radius);
}

int RSGL::CircleCollideLine(RSGL::circle c,RSGL::point p1, RSGL::point p2) {
  float distX = p1.x - p2.x;
  float distY = p1.y - p2.y;
  float len = sqrt( (distX*distX) + (distY*distY) );

  float dot = ( ((c.x-p1.x)*(p2.x-p1.x)) + ((c.y-p1.y)*(p2.y-p1.y)) ) / pow(len,2);
  distX = (float)(p1.x + (dot * (p2.x-p1.x))) - c.x;  distY = (float)(p1.y + (dot * (p2.y-p1.y))) - c.y;
  if ((float)(sqrt( (distX*distX) + (distY*distY) )) <= c.radius) return true; return false;
}

int RSGL::RectCollidePoint(RSGL::rect r, RSGL::point p){
    if (p.x >= r.x &&  p.x <= r.x + r.width && p.y >= r.y && p.y <= r.y + r.length) return 1;
    return 0;
}

int RSGL::PointCollide(RSGL::point p, RSGL::point p2){
    if (p.x == p2.x && p.y == p2.y) return 1;
    return 0;
}

int RSGL::RectCollideRect(RSGL::rect r, RSGL::rect r2){
    if(r.x + r.width >= r2.x && r.x <= r2.x + r2.width && r.y + r.length >= r2.y && r.y <= r2.y + r2.length) return 1; 
    return 0;
}

int RSGL::ImageCollideRect(RSGL::image img, RSGL::rect r){

}

int RSGL::ImageCollideCircle(RSGL::image img, RSGL::circle c){

    
}

int RSGL::ImageCollidePoint(RSGL::image img, RSGL::point p){

}

int RSGL::ImageCollideImage(RSGL::image img, RSGL::image img2){
   
}