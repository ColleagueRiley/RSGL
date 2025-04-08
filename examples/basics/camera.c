#include <stdio.h>
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define RSGL_INT_DEFINED
#define RSGL_IMPLEMENTATION
#include "RSGL.h"
#include "RSGL_gl.h"

#ifdef __APPLE__
	#ifndef GL_SILENCE_DEPRECATION
		#define GL_SILENCE_DEPRECATION
	#endif
	#include <OpenGL/gl.h>
	#include <OpenGL/OpenGL.h>
#else
    #include <GL/gl.h>
#endif


int main(void) {
    RGFW_window* win = RGFW_createWindow("First person camera", RGFW_RECT(0, 0, 800, 450), RGFW_windowCenter | RGFW_windowNoResize );
//    RGFW_window_swapInterval(win, 1);

    RSGL_init(RSGL_AREA(win->r.w, win->r.h), (void*)RGFW_getProcAddress, RSGL_GL_renderer());	
    
    RSGL_camera camera = {.fov = 60};
    RGFW_window_mouseHold(win, RGFW_AREA(win->r.w / 2, win->r.h / 2));    
    glEnable(GL_DEPTH_TEST); 

    u32 frames = 0;
    float frameStartTime = RGFW_getTime();

    while (RGFW_window_shouldClose(win) == 0) {
        while (RGFW_window_checkEvent(win)) {
            switch (win->event.type) {
                case RGFW_mousePosChanged: {      
                    int dev_x = win->event.vector.x;
                    int dev_y = win->event.vector.y;
                    
					/* apply the changes to pitch and yaw*/
                    camera.yaw += (float)dev_x / 15.0;
                    camera.pitch += (float)dev_y / 15.0;
                    break;
                }
                case RGFW_keyPressed:
                    switch (win->event.key) {
                        case RGFW_left:
                            camera.yaw -= 5;
                            break;
                        case RGFW_right:
                            camera.yaw += 5;
                            break;
                        case RGFW_up:
                            camera.pitch -= 5;
                            break;
                        case RGFW_down:
                            camera.pitch += 5;
                            break;
                        default: break;
                    }
                default: break;
            }
        } 
          
        if (RGFW_isPressed(win, RGFW_w)) {
            camera.pos.x += cos((camera.yaw + 90) * DEG2RAD)/5.0;
            camera.pos.z -= sin((camera.yaw + 90) * DEG2RAD)/5.0;
        }
        if (RGFW_isPressed(win, RGFW_s)) {
            camera.pos.x += cos((camera.yaw + 270) * DEG2RAD)/5.0;
            camera.pos.z -= sin((camera.yaw + 270) * DEG2RAD)/5.0;
        }
        
        if (RGFW_isPressed(win, RGFW_a)) {
            camera.pos.x += cos(camera.yaw * DEG2RAD)/5.0;
            camera.pos.z -= sin(camera.yaw * DEG2RAD)/5.0;
        }
        
        if (RGFW_isPressed(win, RGFW_d)) {
            camera.pos.x += cos((camera.yaw + 180) * DEG2RAD)/5.0;
            camera.pos.z -= sin((camera.yaw + 180) * DEG2RAD)/5.0;
        }

        if (RGFW_isPressed(win, RGFW_controlL))
            camera.pos.y -= 5;
        if (RGFW_isPressed(win, RGFW_space))
            camera.pos.y += 5;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        RSGL_clear(RSGL_RGB(80, 80, 110));
       
        RSGL_point3D cam = camera.pos; 
        camera.pos = RSGL_POINT3D(0, 0, 0);
        RSGL_setGlobalMatrix(RSGL_getCameraMatrix(camera));
 //           RSGL_drawCylinder(RSGL_CUBE(19, 20, 9, 20, 300, 0.5), 200, 8, RSGL_RGB(255, 0, 0)); 
//            RSGL_drawSphere(RSGL_CUBE(20, 20, 3, 200, 200, 0.5), RSGL_RGB(255, 0, 0));

            RSGL_drawCube(RSGL_CUBE(cam.x + 49, cam.y + 20, cam.z + 5, 200, 200, 0.5), RSGL_RGB(255, 0, 0));
            RSGL_drawTriangle3D(
                RSGL_createTriangle3D(cam.x + 25.0f, cam.y + 480.0f, cam.z + 0.0,
                                      cam.x + 250, cam.y + 480, cam.z + 3.0f,
                                      cam.x + 800, cam.y + 480.0f, cam.z + 0.0f),
                    RSGL_RGB(255, 0, 0)
            );
        RSGL_resetGlobalMatrix();
        camera.pos = cam;

        RSGL_draw();
        RGFW_window_swapBuffers(win);
     
     	RGFW_checkFPS(frameStartTime, frames, 60);
        frames++;
    }

    RSGL_free();
    RGFW_window_close(win);
}
