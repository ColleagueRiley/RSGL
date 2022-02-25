#pragma once
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <math.h>

#define RSGLRGBTOHEX(r, g, b) ((r << 16) + (g << 8) + b)

namespace RSGL{
    void* xfb;
    _gx_rmodeobj* rmode;
    ir_t ir;

    struct rect{int x,y; int length, width;};
    struct point{int x,y;};
    struct circle{int x,y; int radius;};
    struct color{int r,g,b;};

    int drawRect(RSGL::rect r, RSGL::color c, bool fill=true);
    int drawCircle(RSGL::circle c, color col, bool fill=true);
    int drawPoint(RSGL::point p, RSGL::color c);

    int setBackground(RSGL::color c);

    const int KeyPressed=2;
    const int KeyReleased=3;
    const int MousePosChanged=4;

    int HideRSGLpasspresseddonotuse;

    int init();
    bool isPressed(int key);
    int clear(){VIDEO_ClearFrameBuffer(RSGL::rmode, RSGL::xfb, COLOR_BLACK); return 1;}
    
    //required console only function to move to or properly move to the next frame (put at the end of your game loop)
    void nextFrame(){ VIDEO_WaitVSync(); }
}


int RSGL::init(){

	VIDEO_Init();
	PAD_Init();

	WPAD_Init();
	WPAD_SetVRes(0, 640, 480);
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);


	rmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb, 20, 20, rmode -> fbWidth, rmode -> xfbHeight, rmode -> fbWidth*VI_DISPLAY_PIX_SZ);

	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode -> viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    return 1;
}

bool RSGL::isPressed(int key) {return HideRSGLpasspresseddonotuse == key;}

int RSGL::drawRect(RSGL::rect r, RSGL::color c, bool fill){
    for(int w=0;w<=(r.width/2)-5;w++){
        for(int l=0;l<=r.length-2;l++){
            if (fill || w==0 || w == (r.width/2)-5 || l==0 || l == r.length-2){
                u32* tmpfb = (u32*)RSGL::xfb;
                tmpfb[(w+r.x)+320*(l+r.y)]=RSGLRGBTOHEX(c.b,c.g,c.r);
            }
        }
    }
    return 1;
}


int RSGL::drawCircle(RSGL::circle c, RSGL::color col, bool fill){
    if (fill){
        for (int y=-c.radius; y < (c.radius-1); y++){
          for (int x=-c.radius; x < (c.radius-1); x++){
            if ((x * x) + (y * y) <= (c.radius * c.radius)){
              u32* tmpfb = (u32*)RSGL::xfb;
              tmpfb[(x+c.x)/2+320*(y+c.y)]=RSGLRGBTOHEX(col.b,col.g,col.r);
            }
          }
        }
    }
    else {
        u32* tmpfb = (u32*)RSGL::xfb;
        int x, y;

        for (x = 0; x <= (c.radius-1); x++) {
          y = (int) sqrt ((double) (c.radius * c.radius) - (x * x));

          tmpfb[(c.x+x)/2+320*(c.y+y)]=RSGLRGBTOHEX(col.b,col.g,col.r);
          tmpfb[(c.x+x)/2+320*(c.y+ -y)]=RSGLRGBTOHEX(col.b,col.g,col.r);
          tmpfb[(c.x+ -x)/2+320*(c.y+y)]=RSGLRGBTOHEX(col.b,col.g,col.r);
          tmpfb[(c.x+ -x)/2+320*(c.y+ -y)]=RSGLRGBTOHEX(col.b,col.g,col.r);

          tmpfb[(c.x+y)/2+320*(c.y+x)]=RSGLRGBTOHEX(col.b,col.g,col.r);
          tmpfb[(c.x+y)/2+320*(c.y+ -x)]=RSGLRGBTOHEX(col.b,col.g,col.r);
          tmpfb[(c.x+ -y)/2+320*(c.y+x)]=RSGLRGBTOHEX(col.b,col.g,col.r);
          tmpfb[(c.x+ -y)/2+320*(c.y+ -x)]=RSGLRGBTOHEX(col.b,col.g,col.r);
        }
    }
    return 1;
}





int RSGL::drawPoint(RSGL::point p, RSGL::color c){
    u32 *tmpfb = (u32*)RSGL::xfb;
    tmpfb[p.x+320*p.y]=RSGLRGBTOHEX(c.b,c.g,c.r); 
    return 1;
}

int RSGL::setBackground(RSGL::color c){
    u32* tmpfb = (u32*)RSGL::xfb;
    unsigned int color = RSGLRGBTOHEX(c.b,c.g,c.r);
    if (c.r+c.g+c.b == 255*3)  color =COLOR_WHITE;
    else if(!c.r+c.g+c.b)  color =COLOR_BLACK;
    for (int i=0; i < sizeof(tmpfb); i++){
        tmpfb[i] = color;
    }
}



struct Event{
  int type;
  int x,y;
  void CheckEvents();
  private:
    u32 ppressed = 0;
};

void Event::CheckEvents(){
    WPAD_ScanPads();
    u32 pressed = WPAD_ButtonsDown(0);
    if (pressed != 0) type = 2; else if (pressed == 0 && ppressed != 0) type = 3;
    RSGL::HideRSGLpasspresseddonotuse = pressed;
    ppressed = WPAD_ButtonsDown(0);
    WPAD_IR(0, &RSGL::ir);
    if (RSGL::ir.valid) {
        if (RSGL::ir.x != x || RSGL::ir.y != y) type = 4;
        x = RSGL::ir.x;
        y = RSGL::ir.y;
    }
}
