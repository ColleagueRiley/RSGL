#define STB_TRUETYPE_IMPLEMENTATION 
#include "deps/stb_truetype.h" // for loading fonts
#include "../../include/include/linux/rsgl.hpp" // source headers
#include "../../include/include/linux/deps/X11/Xutil.h"  // more xlib
#include <pthread.h> // pthread for threading
float			TimeCounter, LastFrameTimeCounter, DT, prevTime = 0.0, FPS;
struct timeval		tv, tv0;
int			Frame = 1, FramesPerFPS;
#include<sys/time.h>

int singleBufferAttributess[] = {
    GLX_RGBA,
    GLX_RED_SIZE,   8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE,  8,
    None };
int doubleBufferAttributes[] =  { 
	GLX_RGBA, 
	GLX_DOUBLEBUFFER,
	GLX_RED_SIZE,   8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE,  8,
	None };
#define DOUBLE_BUFFERING
#ifndef DOUBLE_BUFFERING
static int attributeList[] = {
    GLX_RGBA,
    GLX_RED_SIZE,   8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE,  8,
    None };
#else // DOUBLE_BUFFERING
static int attributeList[] = {
	GLX_RGBA, 
	GLX_DOUBLEBUFFER,
	GLX_RED_SIZE,   8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE,  8,
	None };
#endif // DOUBLE_BUFFERING

// the comments in this doc assume you already know what the args mean from the rsgl.hpp source file

void RSGL::notifiy(std::string title, std::string content ,std::string image){
  std::string com = "notify-send \"" + title +"\" \"" + content + "\" ";  // the command 
  if (image != "")  com += "-i \"" + image + "\""; // add in the image if a image added
  popen(com.data(),"r"); // run the command
}

void RSGL::messageBox(std::string message,bool question,bool error){
  std::string com = "zenity "; // command to run
  if (question) com+="--question "; else if (error) com+= "--error"; else com+="--warning "; //

  com += "--text \"" + message + "\"";
  std::cout << com.data();
  popen(com.data(),"r");
}

std::vector<std::string> RSGL::fileDialog(std::string title,bool multiple,bool save, bool directory){
  char filename[1024];
  std::string com="zenity --file-selection --title=\""+title+"\"";
  if (multiple) com += " --multiple --separator=\" \"";
  if (directory) com += " --directory";
  if (save) com += " --save";
  FILE *f = popen(com.data(), "r");
  fgets(filename, 1024, f);
  std::string fn(filename);
  if (multiple){ 
    std::vector<std::string> output;
    std::string file;
    for (int i=0; i < (int)fn.size(); i++){
      if (fn[i] == ' '){ output.insert(output.end(),file); file="";}
      else file+=fn[i];
    }
    return output;
  }
  
  return {fn};
}

char ttf_buffer[1<<25];
char ttf_buffer2[1<<25];

void RSGL::drawText(std::string text, RSGL::circle r, const char* font, RSGL::color col, RSGL::drawable d){
    stbtt_fontinfo Font; bool cre=false; 
    FILE* f = fopen(font, "rb");
    if (ttf_buffer != NULL && f != NULL) fread(ttf_buffer, 1, 1<<25,f);
    int high=0;
    for (int dr=0; dr<2; dr++){
        int L=0;
        for (int l=0; l < text.size(); l++){
            int w,h,i,j,c = text.at(l), s = r.radius,b; L++; 
            stbtt_InitFont(&Font, (const unsigned char*)ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char*)ttf_buffer,0));
            unsigned char * bitmap = stbtt_GetCodepointBitmap(&Font, 0,stbtt_ScaleForPixelHeight(&Font, s), c, &w, &h, 0,0);
            if (h > high && !dr) high=h; 
            else if (dr){
                if (h < high && text.size() > 2) b=high-h;
                else b=0;
                if (d.GPU==1){ glBegin(GL_POINTS);    glColor4f(col.r/255.0, col.g/255.0, col.b/255.0,col.a/255.0);}
                for (j=0; j < h; ++j){
                    for (i=0; i < w; ++i) if ( " .:ioVM@"[bitmap[j*w+i]>>5] != ' '){ 
                        if (!d.GPU){RSGL::drawPoint({i+r.x+(L*s),j+(r.y+b)},col,d);}
                        else if (d.GPU == 1){ 
                            RSGL::point p1 = {i+r.x+(L*s),j+(r.y+b)};
                            float i = d.r.width/2*1.0f;
                            float  x = (p1.x/i)-1.0f;
                            i = d.r.length/2*1.0f;
                            float  y = (-(p1.y)/i)+1.0f;
                            glVertex2f(x,y);
                        }
                    }
                }
                if (d.GPU==1){
                    glEnd();
                    glFlush();
                }
            }
        }
    }  
    *ttf_buffer=*ttf_buffer2;
}

RSGL::Text::Text(std::string txt, RSGL::circle r, const char* font, RSGL::color col, bool draw, RSGL::drawable d){
  rect=r; text=txt; c=col; f=font;
  if (draw){
    RSGL::drawText(txt,r,font,col,d);
  }
}

void RSGL::Text::draw(){
  RSGL::drawText(text,rect,f,c,d);
}


bool RSGL::window::isPressed(unsigned long key) {
    KeyCode kc2 = XKeysymToKeycode(display, key);
    bool isPressed = !!(keyboard[kc2 >> 3] & (1 << (kc2 & 7)));
    return isPressed;
}


bool RSGL::window::isPressed(std::string key){
    return isPressed(XStringToKeysym(key.data()));
}



void DrawDottedLine(RSGL::rect r,RSGL::color c,RSGL::drawable win=RSGL::root,bool t=false) {
    if (t){
        DrawDottedLine({r.x,r.y,r.x+r.width,r.y},c,win);
        DrawDottedLine({r.x,r.y,r.x,r.y+r.length},c,win);
        DrawDottedLine({r.x+r.width,r.y,r.x+r.width,r.y+r.length},c,win);
        
        DrawDottedLine({r.x,r.y+r.length,r.x+r.width,r.y+r.length},c,win);
    }   
    else{
    int x0=r.x, y0=r.y, x1=r.width, y1=r.length;
    int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2;
    int count = 0;
    glBegin(GL_POINTS);
    while (1) {
        if (count < 10) {
            float i = win.r.width/2*1.0f;
            float  x2 = (x0/i)-1.0f;
            i = win.r.length/2*1.0f;
            float  y2 = (-(y0)/i)+1.0f;
            glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
            glVertex2f(x2,  y2);
        }
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
        count = (count + 1) % 20;
    }
    glEnd();
    glFlush();
    }
}

void RSGL::drawRect(RSGL::rect r,color c, bool fill,bool dotted, RSGL::drawable win){
    if (!win.GPU){
        XSetForeground(win.display,XDefaultGC(win.display,XDefaultScreen(win.display)),
        RSGLRGBTOHEX(c.r,c.g,c.b));
        if (fill) XFillRectangle(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)),r.x,r.y,r.width,r.length);
        else{
            RSGL::drawLine({r.x,r.y},{r.x,r.y+r.length},c);
            RSGL::drawLine({r.x+r.width,r.y},{r.x+r.width,r.y+r.length},c);
            RSGL::drawLine({r.x,r.x},{r.x+r.width,r.y},c);
            RSGL::drawLine({r.x,r.y+r.length},{r.x+r.width,r.y+r.length},c);

        }        
    }
    else if (win.GPU == 1){
        float i = win.r.width/2*1.0f;
        float  x = (r.x/i)-1.0f;
        float  x2 =((r.x+r.width)/i)-1.0f;
        i = win.r.length/2*1.0f;
        float  y = (-(r.y)/i)+1.0f;
        float  y2 =(-(r.y+r.length)/i)+1.0f;
        if (dotted){DrawDottedLine(r,c,win,true);}
        else{
        GLenum m=GL_POLYGON; if (!fill) m=GL_LINE_STRIP;
        glBegin(m);
        glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
        glVertex2f(x,  y);
        glVertex2f(x2, y);
        glVertex2f(x2, y2);
        glVertex2f(x, y2);
        if (!fill) glVertex2f(x,  y);
        glEnd();
        glFlush();
        }
    }
}

void RSGL::drawLine(RSGL::point p1, RSGL::point p2, RSGL::color c, RSGL::drawable win){
    if (!win.GPU){
        XSetForeground(win.display,XDefaultGC(win.display,XDefaultScreen(win.display)),
        RSGLRGBTOHEX(c.r,c.g,c.b));
        XDrawLine(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)),p1.x,p1.y,p2.x,p2.y);
    }
    else if (win.GPU == 1){
        float i = win.r.width/2*1.0f;
        float  x = (p1.x/i)-1.0f;
        float  x2 =((p2.x)/i)-1.0f;
        i = win.r.length/2*1.0f;
        float  y = (-(p1.y)/i)+1.0f;
        float  y2 =(-(p2.y)/i)+1.0f;
        glBegin(GL_LINE_STRIP);
        glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
        glVertex2f(x,  y);
        glVertex2f(x2, y2);
        glEnd();
        glFlush();
    }
}

int RSGL::drawCircle(RSGL::circle c, color col,bool fill,RSGL::drawable win){
    if (!win.GPU){
        XSetForeground(win.display,XDefaultGC(win.display,XDefaultScreen(win.display)),RSGLRGBTOHEX(col.r,col.g,col.b));
        if (fill){XFillArc(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)), c.x-(30/2), c.y-(30/2), c.radius, c.radius, 0, 360*64);}
        else XDrawArc(win.display,win.d,XDefaultGC(win.display,XDefaultScreen(win.display)), c.x-(30/2), c.y-(30/2), c.radius, c.radius, 0, 360*64);
        return 1;
    }
    else if (win.GPU == 1){
        float i = win.r.width/2*1.0f;
        float  x2 = (c.x/i)-1.0f;
        i = win.r.length/2*1.0f;
        float  y2 = (-(c.y)/i)+1.0f;

        i = win.r.width/2*1.0f;
        float  r = ((-(c.radius-258)/i)-1.0f);
        if (!fill){
        glBegin(GL_LINE_LOOP);
                glColor4f(col.r/255.0, col.g/255.0, col.b/255.0,col.a/255.0);
                for(int ii = 0; ii < 60; ii++){
                    float theta = 2.0f * 3.1415926f * float(ii) / float(60);//get the current angle

                    float x = r * cosf(theta);//calculate the x component
                    float y = r * sinf(theta);//calculate the y component

                    glVertex2f(x + x2, y + y2);//output vertex

                }
        glEnd();
        return 1;
        }
        if (fill){
        glBegin(GL_TRIANGLE_FAN);
             glColor4f(col.r/255.0,col.g/255.0,col.b/255.0,col.a/255.0);
            glVertex2f(x2,y2);

            for (float angle=1.0f;angle<361.0f;angle+=0.2){
                float x3 = x2+sin(angle)*r;
                float y3 = y2+cos(angle)*r;
                glVertex2f(x3,y3);
            }   
        glEnd();
        }
  }
  return 1;
}


int RSGL::drawPoint(RSGL::point p, color c, RSGL::drawable win){ 
    RSGL::drawRect({p.x,p.y,1,1,},c,true,false,win);
    return 1;
}


bool pressed=false;
void RSGL::window::checkEvents(){
  LastFrameTimeCounter = TimeCounter;
  gettimeofday(&tv, NULL);
  TimeCounter = (float)(tv.tv_sec-tv0.tv_sec) + 0.000001*((float)(tv.tv_usec-tv0.tv_usec));
  DT = TimeCounter - LastFrameTimeCounter;
 
  //calculate FPS
  Frame ++;
  if ((Frame%FramesPerFPS) == 0) {
	FPS = ((float)(FramesPerFPS)) / (TimeCounter-prevTime);
	prevTime = TimeCounter;
  }
  debug.fps=FPS;
  XEvent E;
  if (XEventsQueued(display,QueuedAlready) + XEventsQueued(display,QueuedAfterReading)) XNextEvent(display, &E);
  event.type = E.type;
  switch (event.type){
        case 33: if (E.xclient.data.l[0] != (long int)XInternAtom(display, "WM_DELETE_WINDOW", true)) event.type=0;break;
        case 4: event.button = E.xbutton.button; 
            if (event.type==4) pressed=true;  else pressed=false;
            int x, y,i; unsigned m; unsigned m2; Window w=d; Window root, child;
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){event.x=x; event.y=y;} break;
        case 5: event.button = E.xbutton.button; 
            if (event.type==4) pressed=true;  else pressed=false;
            int x, y,i; unsigned m; unsigned m2; Window w=d; Window root, child;
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){event.x=x; event.y=y;} break;
        case 6:
            int x, y,i; unsigned m; unsigned m2; Window w=d; Window root, child;
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){event.x=x; event.y=y;} break;
        case 2: XQueryKeymap(display,keyboard); event.keycode = XKeycodeToKeysym(display,E.xkey.keycode,1); event.key=XKeysymToString(event.keycode); break;
        case 3: XQueryKeymap(display,keyboard); event.keycode = XKeycodeToKeysym(display,E.xkey.keycode,1); event.key=XKeysymToString(event.keycode); break;
        default:
            event.keycode = 0; event.key="";    
            int x, y,i; unsigned m; unsigned m2; Window w=d; Window root, child;
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){
            if (m ){
                    if (m == 256) event.button=1;
                    else if (m == 212) event.button=2;
                    else if (m == 1024) event.button=3;
                    event.type=RSGL::MouseButtonReleased;
                    if (pressed) event.type=RSGL::MouseButtonPressed;
                    pressed=!pressed;
                }
            } break;
  }
  XWindowAttributes a;
  XGetWindowAttributes(display,d,&a); 
  if (r.width != a.width&& areesize || r.length != a.height  && areesize){ glViewport(0,0,a.width,a.height); } //std::cout << E.xresizerequest.width << " , " << E.xresizerequest.height << std::endl;}
  r.width=a.width; r.length=a.height; r.x=a.x; r.y=a.y;
  XKeyboardState keystate;
  XGetKeyboardControl(display,&keystate); event.ledState= keystate.led_mask;
}

void RSGL::window::close(){
    XDestroyWindow(display, d);
    XCloseDisplay(display);
}
void RSGL::window::clear(){
    if (!GPU) XClearWindow(display,d);
    else if (GPU==1){
        glFlush();
        if (swapFlag) glXSwapBuffers( display, d );
        glClearColor(color.r/250.0,color.g/250.0,color.b/250.0,color.a/250.0);
        glClear( GL_COLOR_BUFFER_BIT );
    }
}


std::vector<std::vector<RSGL::color>> RSGL::resizeImage(std::vector<std::vector<RSGL::color>> values,RSGL::rect size,RSGL::rect ogSize){
    std::vector<std::vector<RSGL::color>> output;
    int rectShape[2] = {size.width-ogSize.width,size.length-ogSize.length};
    for (int y=0; y < size.length; y++){
        output.insert(output.end(),{{}});
        for (int x=0; x < size.length; x++){
            
        }
    }
    return output;
}

void RSGL::drawImage(std::string fileName, RSGL::rect r,bool resize,RSGL::drawable d){
    png::image< png::rgba_pixel > image(fileName);
    std::vector<std::vector<RSGL::color>> img;
    for (png::uint_16 y=0; y < image.get_height(); y++){  
        img.insert(img.end(),{{}}); 
        for (png::uint_16 x=0; x < image.get_width(); x++) img.at(img.size()-1).insert(img.at(img.size()-1).end(),{image[y][x].red,image[y][x].green,image[y][x].blue,(float)image[y][x].alpha});
    }
    if (image.get_width() != r.width && image.get_height() != r.length && resize) img = RSGL::resizeImage(img,r,{0,0,(int)image.get_width(),(int)image.get_height()});
    glBegin(GL_POINTS);        
    for (int y3=0; y3 < img.size(); y3++){
        for (int x3=0; x3 < img.at(y3).size(); x3++){
            if(img.at(y3).at(x3).a >= 100){
                RSGL::color c = img.at(y3).at(x3);
                float i = d.r.width/2*1.0f;
                float  x = (r.x+x3/i)-1.0f;
                float  x2 =((r.x+x3+1)/i)-1.0f;
                i = d.r.length/2*1.0f;
                float  y = (-(r.y+y3)/i)+1.0f;
                float  y2 =(-(r.y+y3+1)/i)+1.0f;
                glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0);
                glVertex2f(x,  y);
                glVertex2f(x2, y);
                glVertex2f(x2, y2);
                glVertex2f(x, y2);
                
            }
        }
    }
    glEnd();
    glFlush();
}



void RSGL::drawable::loadArea(RSGL::drawable& dsrc, RSGL::rect r, RSGL::point p){
  XCopyArea(display,dsrc.d,d,XDefaultGC(display,XDefaultScreen(display)),
    r.x,r.y,r.width,r.length,p.x,p.x);
}


RSGL::pixmap::pixmap(RSGL::drawable dr, RSGL::area a){
  display = dr.display;
  d = XCreatePixmap(display,dr.d,a.width,a.length,XDefaultDepth(display,XDefaultScreen(display)));
  data = XGetImage(display,dr.d,0,0,a.width,a.length,0,2);
  memset(data->data,RSGLRGBTOHEX(dr.color.r,dr.color.g,dr.color.g),a.width*a.length*3);
  XPutImage(display,d,XDefaultGC(display,XDefaultScreen(display)),
    data,0,0,0,0,a.width,a.length);
}

int RSGL::window::setColor(RSGL::color c){ 
    color = c;
    XSetWindowBackground(display,d,RSGLRGBTOHEX(c.r,c.g,c.b));
    return 1;
}

static Bool WaitForNotify( Display *dpy, XEvent *event, XPointer arg ) {return (event->type == MapNotify) && (event->xmap.window == (Window) arg);}


RSGL::window::window(std::string wname,RSGL::rect winrect, RSGL::color c, int gpu, bool resize, bool autoResize){   
    areesize=autoResize;
    XInitThreads();
    gettimeofday(&tv0, NULL);
    FramesPerFPS = 30; 
    display = XOpenDisplay(0);
    if (display == nullptr) exit(0);
    GPU=gpu;
    long event_mask = ExposureMask
        | KeyPressMask
        | KeyReleaseMask
        | ButtonPressMask
        | ButtonReleaseMask
        | FocusChangeMask
        | PointerMotionMask
        | SubstructureNotifyMask
        | StructureNotifyMask;
       // | ResizeRedirectMask;
    if (!gpu){
        Window parent = RootWindow(display, XDefaultScreen(display));
        d = XCreateSimpleWindow(display, parent, winrect.x,winrect.y,winrect.width,winrect.length, 0,0,  RSGLRGBTOHEX(c.r,c.g,c.b));

        XSelectInput(display, d, event_mask);
        XMapWindow(display, d);
        if (!d) d={};
        XClearWindow(display,d);
        XMapRaised(display,d);
    
        XEvent E;
        XNextEvent(display, &E);
        Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", true);
        XSetWMProtocols(display, d, &wm_delete, 1);
        //XGCValues values;
    
        Atom xdndAtom = XInternAtom( display, "XdndAware", False );
        char xdndVersion = 5;
        XChangeProperty( display, d, xdndAtom, XA_ATOM, 32,
            PropModeReplace, (unsigned char *)&xdndVersion, 1 );

        XLockDisplay(display);
        XUnlockDisplay(display); 
    }
    else if (gpu==1){
        int numReturned;
        //auto attributeList = doubleBufferAttributes;

        GLXFBConfig* fbConfigs = glXChooseFBConfig( display, DefaultScreen(display),
                                   attributeList, &numReturned );
        if ( fbConfigs == NULL ) { 
        //attributeList= singleBufferAttributess;
        fbConfigs = glXChooseFBConfig( display, DefaultScreen(display),attributeList, &numReturned );
        swapFlag = False;
        } 

        vInfo = glXGetVisualFromFBConfig( display, fbConfigs[0] );
        XVisualInfo *vi = glXChooseVisual(display, DefaultScreen(display), attributeList);
    
        Colormap cmap = XCreateColormap(display, RootWindow(display, vi->screen),
			   vi->visual, AllocNone);
        XSetWindowAttributes swa;
        context = glXCreateContext(display, vi, 0, GL_TRUE);

        swa.colormap = cmap;
        swa.border_pixel = 0;
        swa.event_mask =  event_mask;
        d = XCreateWindow(display, RootWindow(display, vi->screen), winrect.x, winrect.y, winrect.width,winrect.length,
                        0, vi->depth, InputOutput, vi->visual,
                        (1L<<1) | (1L<<13) | CWBorderPixel | CWEventMask, &swa);
        XSelectInput(display, d, swa.event_mask);
        XEvent event;
        XMapWindow(display, d);
        XIfEvent(display, &event, WaitForNotify, (char*)d);
        /* connect the context to the window */
        glXMakeCurrent(display, d, context); 
        
        
        XSetWindowBackground(display,d,RSGLRGBTOHEX(c.r,c.g,c.b));
        glClearColor(color.r,color.g,color.b,color.a);
        name = wname; r = winrect; color = c;
        dbuffer.d = XCreatePixmap(display,d,winrect.width,winrect.length,XDefaultDepth(display,XDefaultScreen(display)));
    }
    if (!RSGL::root.d){root=*this;root.display=display;}
}

int RSGL::CircleCollide(RSGL::circle cir,RSGL::circle cir2){
	float distanceBetweenCircles = (float)sqrt(
	(cir2.x - cir.x) * (cir2.x - cir.x) + 
    (cir2.y - cir.y) * (cir2.y - cir.y)
  	);
	if (distanceBetweenCircles > cir.radius + cir2.radius){return 0;}else{return 1;}
}

int RSGL::CircleCollideRect(RSGL::circle c, RSGL::rect r){
  float testX = c.x; float testY = c.y;

  if (c.x < r.x) {testX = r.x;}  else if (c.x > r.x+r.width) {testX = r.x+r.width;}
  if (c.y < r.y) {testY = r.y;}  else if (c.y > r.y+r.length) {testY = r.y+r.length;} 
  
  return (sqrt( ( (c.x-testX) * (c.x-testX) ) + ( (c.y-testY) *(c.y-testY) ) )  <= c.radius);
}

int RSGL::CircleCollidePoint(RSGL::circle c, RSGL::point p){
	float testX = c.x; float testY = c.y;

  	if (c.x < p.x) {testX = p.x;}  else if (c.x > p.x+1) {testX = p.x+1;}
  	if (c.y < p.y) {testY = p.y;}  else if (c.y > p.y+1) {testY = p.y+1;} 
  
  	return (sqrt( ( (c.x-testX) * (c.x-testX) ) + ( (c.y-testY) *(c.y-testY) ) )  <= c.radius);
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
    for (int i=0; i < (int)img.cords.size(); i++){
        if(RSGL::RectCollidePoint(r, {img.cords[i][0],img.cords[i][1]})){
            return 1;
        }
    }return 0;
}

int RSGL::ImageCollideCircle(RSGL::image img, RSGL::circle c){
    for (int i=0; i < (int)img.cords.size(); i++){
        if(RSGL::CircleCollidePoint(c, {img.cords[i][0],img.cords[i][1]})){
            return 1;
        }
    }return 0;    
    
}

int RSGL::ImageCollidePoint(RSGL::image img, RSGL::point p){
    for (int i=0; i < (int)img.cords.size(); i++){
        if(RSGL::PointCollide(p, {img.cords[i][0],img.cords[i][1]})){
            return 1;
        }
    }return 0;      
}

int RSGL::ImageCollideImage(RSGL::image img, RSGL::image img2){
    for (int i=0; i < (int)img.cords.size(); i++){
        for (int j=0; j < (int)img2.cords.size(); j++)
            if(RSGL::PointCollide({img2.cords[i][0],img2.cords[i][1]}, {img.cords[i][0],img.cords[i][1]})){
                return 1;
            }
    }return 0;      
}