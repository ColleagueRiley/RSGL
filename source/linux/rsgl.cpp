#ifndef RSGL
#include "../../include/include/linux/rsgl.hpp" // source headers
#endif
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
    } XStoreName(display,d,wname.data());
    if (!RSGL::root.d){root=*this;root.display=display;}
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
  event.type = E.type; int x, y,i; unsigned m; unsigned m2; Window root, child,w; 
  switch (event.type){
        case 33: if (E.xclient.data.l[0] != (long int)XInternAtom(display, "WM_DELETE_WINDOW", true)) event.type=0;break;
        case 4: event.button = E.xbutton.button; 
            if (event.type==4) pressed=true;  else pressed=false;
            w=d; 
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){event.x=x; event.y=y;} break;
        case 5: event.button = E.xbutton.button; 
            if (event.type==4) pressed=true;  else pressed=false;
            w=d; 
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){event.x=x; event.y=y;} break;
        case 6:
           w=d; 
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){event.x=x; event.y=y;} break;
        case 2: XQueryKeymap(display,keyboard); event.keycode = XKeycodeToKeysym(display,E.xkey.keycode,1); event.key=XKeysymToString(event.keycode); break;
        case 3: XQueryKeymap(display,keyboard); event.keycode = XKeycodeToKeysym(display,E.xkey.keycode,1); event.key=XKeysymToString(event.keycode); break;
        default:
            event.keycode = 0; event.key="";    
            w=d; 
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

int RSGL::window::setColor(RSGL::color c){ 
    color = c;
    XSetWindowBackground(display,d,RSGLRGBTOHEX(c.r,c.g,c.b));
    return 1;
}



bool RSGL::window::isPressed(unsigned long key) {
    KeyCode kc2 = XKeysymToKeycode(display, key);
    bool isPressed = !!(keyboard[kc2 >> 3] & (1 << (kc2 & 7)));
    return isPressed;
}


bool RSGL::window::isPressed(std::string key){ return isPressed(XStringToKeysym(key.data())); }


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

namespace RSGL{RSGL::window root=root;};