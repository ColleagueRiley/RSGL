#ifndef RSGL
#define GLX_GLXEXT_PROTOTYPES
#include "../../include/linux/rsgl.hpp" // source headers
#define RSGLJOYSTICKNOSRC
#include "../../include/linux/joystick.hpp"
#endif
#include <X11/Xatom.h>
#include <X11/Xutil.h> // more xlib
#include <pthread.h> // pthread for threading
float			TimeCounter, LastFrameTimeCounter, DT, prevTime = 0.0, FPS;
struct timeval		tv, tv0;
int			Frame = 1, FramesPerFPS;
#include<sys/time.h>
#include <cstring>

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
    GLX_RGBA, GLX_DOUBLEBUFFER,
    GLX_RED_SIZE,   8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE,  8,
    None };
#endif // DOUBLE_BUFFERING

static Bool WaitForNotify( Display *dpy, XEvent *event, XPointer arg ) {return (event->type == MapNotify) && (event->xmap.window == (Window) arg);}


RSGL::window root; bool rootChosen=false;
RSGL::window* RSGL::root(){ return &::root; }

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
        d = XCreateSimpleWindow(display, NULL, winrect.x,winrect.y,winrect.width,winrect.length, 0,0,  RSGLRGBTOHEX(c.r,c.g,c.b));

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
    else if (abs(gpu)==1){
	    gpu=1;
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
        //connect the context to the window 
        glXMakeCurrent(display, d, context); 
        
        
        XSetWindowBackground(display,d,RSGLRGBTOHEX(c.r,c.g,c.b));
        glClearColor(color.r,color.g,color.b,color.a);
        name = wname; r = winrect; color = c;
        dbuffer.d = XCreatePixmap(display,d,winrect.width,winrect.length,XDefaultDepth(display,NULL));
        glEnable(GL_BLEND); //Enable blending.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function

    } XStoreName(display,d,wname.data());
    if (!rootChosen){ rootChosen=true; isRoot=true;  } 
}

bool pressed=false;
void RSGL::window::checkEvents(){
//    for (int i=0; i < joysticks.size(); i++) joysticks.at(i)->checkEvents();
    glXMakeCurrent(display,d,context);
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
    if ( E.type == ButtonPress) event.type = RSGL::MouseButtonPressed;
    if ( E.type == ButtonRelease) event.type = RSGL::MouseButtonReleased;
    event.type = E.type; int x, y,i; unsigned m; unsigned m2; Window root, child,w; 
    switch (event.type){
        case 33: if (E.xclient.data.l[0] != (long int)XInternAtom(display, "WM_DELETE_WINDOW", true)) event.type=0;break;
        case 4: event.button = E.xbutton.button; 
            if (event.type==4) pressed=true;  else pressed=false;
            w=d; 
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){event.mouse.x=x; event.mouse.y=y;} break;
        case 5: event.button = E.xbutton.button; 
            if (event.type==4) pressed=true;  else pressed=false;
            w=d; 
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){event.mouse.x=x; event.mouse.y=y;} break;
        case 6:
           w=d; 
            if (XQueryPointer(display, w, &root, &child, &x, &y, &x, &y, &m)){event.mouse.x=x; event.mouse.y=y;} break;
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
    if (event.key == "ISO_Left_Tab") event.key = "Tab";
    if (isRoot) ::root=*this; 
}

void RSGL::window::close(){
    XDestroyWindow(display, d);
    XCloseDisplay(display);
}
void RSGL::window::clear(){
    if (!GPU) XClearWindow(display,d);
    else if (GPU==1){
        glXMakeCurrent(display,d,context);
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


bool RSGL::window::isPressed(std::string key){ 
    if (key == "Tab") key = "ISO_Left_Tab";
    if (key == "Shift") return (isPressed(XStringToKeysym("Shift_L")) || isPressed(XStringToKeysym("Shift_R")));
    if (key == "Control") return (isPressed(XStringToKeysym("Control_L")) || isPressed(XStringToKeysym("Control_R")));
    return isPressed(XStringToKeysym(key.data())); 
}


void RSGL::drawable::loadArea(RSGL::drawable& dsrc, RSGL::rect r, RSGL::point p){

}


RSGL::pixmap::pixmap(RSGL::drawable dr, RSGL::area a){
  display = dr.display;
}

void RSGL::func::start(bool running , void (*gameLoop)(RSGL::window), void (*eventLoop)(int,RSGL::window), RSGL::window win){
    while (running){
        win.checkEvents();
        eventLoop(win.event.type,win);
        gameLoop(win);
    }
    win.close();
}

void RSGL::func::start(bool* running , void (*gameLoop)(RSGL::window), void (*eventLoop)(int,RSGL::window), RSGL::window win){
    while (*running){
        win.checkEvents();
        eventLoop(win.event.type,win);
        gameLoop(win);
    }
    win.close();
}

RSGL::rect RSGL::fullscreen(){
    Display* disp = XOpenDisplay(NULL); 
    Screen*  scrn = DefaultScreenOfDisplay(disp);  
    return {0,0,scrn->width,scrn->height};
}