#include "../../include/linux/rsgl.hpp" // source headers
#include <GL/glx.h> // opengl/x11
#include <X11/XKBlib.h>

// get the current time
#include <sys/time.h>

// vars for counting fps
float TimeCounter, LastFrameTimeCounter, DT, prevTime = 0.0, FPS;
struct timeval	tv, tv0;
int	Frame = 1, FramesPerFPS;

// buffer attributes, double by default unless it's not supported 
int singleBufferAttributes[] = {
	GLX_RGBA,
	GLX_RED_SIZE,   8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE,  8,
	None 
};

int doubleBufferAttributes[] =  { 
	GLX_RGBA, 
	GLX_DOUBLEBUFFER,
	GLX_RED_SIZE,   8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE,  8,
	None 
};

//the root grab function
bool rootChosen = false; // if there's a root yet or not

RSGL::window root; // the root window
RSGL::window* RSGL::root(){ return &::root; }

void RSGL::window::moveTo(RSGL::point p){ 
	XMoveWindow(display, d, p.x, p.y); 
}

RSGL::window::window(std::string wname,RSGL::rect winrect, RSGL::color c, RSGL::winArgs args/*={}*/){   
	XInitThreads(); // init X11 threading
	// init the display
	display = XOpenDisplay(0);
	if (display == NULL) exit(0);
	
	// set RSGL::window vars to the args 
	if (!winrect.w && !winrect.h) winrect = {0, 0, winrect.x, winrect.y};
	areesize = args.autoResize;
	GPU = args.gpu;
	name = wname; 
	r = winrect; 
	color = c;

	// set up FPS for the window
	gettimeofday(&tv0, NULL);
	FramesPerFPS = 30; 

	long event_mask = ExposureMask
		| KeyPressMask
		| KeyReleaseMask
		| ButtonPressMask
		| ButtonReleaseMask
		| FocusChangeMask
		| PointerMotionMask
		| SubstructureNotifyMask
		| StructureNotifyMask; // X11 events accepted

	if ( GPU == 1 ){
		// fetch the X11/GLX visual info
		XVisualInfo *vi = glXChooseVisual(display, DefaultScreen(display), doubleBufferAttributes);
	
		if (vi == NULL) // switch to single buffer if double buffer fails
			vi = glXChooseVisual(display, DefaultScreen(display), singleBufferAttributes);

		Colormap cmap = XCreateColormap(display, RootWindow(display, vi->screen),
			   vi->visual, AllocNone); // make the cmap from the visual
	
		context = glXCreateContext(display, vi, 0, GL_TRUE); // create the GLX context with the visual
		
		// make X window attrubutes
		XSetWindowAttributes swa;

		swa.colormap = cmap;
		swa.border_pixel = 0;
		swa.event_mask = event_mask;
		
		// create the window
		d = XCreateWindow(display, RootWindow(display, vi->screen), winrect.x, winrect.y, winrect.w, winrect.h,
						0, vi->depth, InputOutput, vi->visual,
						(1L<<1) | (1L<<13) | CWBorderPixel | CWEventMask, &swa); 

		XSelectInput(display, d, swa.event_mask); // tell X11 what events we want
		
		// make it so the user can't close the window until the program does
		Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", true);
		XSetWMProtocols(display, d, &wm_delete, 1);
		
		//connect the context to the window 
		glXMakeCurrent(display, d, context); 
		
		// set the background
		XSetWindowBackground(display, d, RSGLRGBTOHEX(c.r,c.g,c.b)); 
		glClearColor(color.r, color.g, color.b, color.a);

		XStoreName(display, d, wname.data()); // set the name

		glEnable(GL_BLEND); //Enable blending.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function
		
		XMapWindow(display, d); // draw the window
		moveTo({winrect.x, winrect.y}); // move the window to it's proper cords
	} 
	// set the window as the root window if another window hasn't been chosen
	if (!rootChosen){ rootChosen=true; isRoot=true;  } 
}

bool pressed=false; // if the mouse has been pressed or not

void RSGL::window::checkEvents(){
	// make this window the current glx context
	glXMakeCurrent(display,d,context);
	
	// calculate FPS
	LastFrameTimeCounter = TimeCounter;
	gettimeofday(&tv, NULL);
	TimeCounter = (float)(tv.tv_sec-tv0.tv_sec) + 0.000001*((float)(tv.tv_usec-tv0.tv_usec));
	DT = TimeCounter - LastFrameTimeCounter;

	Frame++;
	if ((Frame%FramesPerFPS) == 0) {
		FPS = ((float)(FramesPerFPS)) / (TimeCounter-prevTime);
		prevTime = TimeCounter;
	}
	
	debug.fps = FPS; // send the fps

	XEvent E; // raw X11 event

	// if there is no unread qued events, get a new one
	if ( XEventsQueued(display,QueuedAlready) + XEventsQueued(display,QueuedAfterReading) ) 
		XNextEvent(display, &E);
	
	// vars for getting the mouse x/y with query
	int x, y, i; 
	unsigned m, m2; 
	Window root, child; 
	
	if (E.type == KeyRelease || E.type == KeyRelease){
		// query the keys if it's an event key event
		XQueryKeymap(display, keyboard);

		// get keysym
		event.keysym = XkbKeycodeToKeysym(display,E.xkey.keycode, 0, E.xkey.state & ShiftMask ? 1 : 0);
		// convert to string
		event.key=XKeysymToString(event.keysym);
	}

	switch (E.type){
		case KeyPress: 
			event.type = RSGL::keyPressed;
			break;
		
		case KeyRelease: 
			event.type = RSGL::keyReleased;
			break;
		
		case ButtonPress: 
			pressed = true;
			event.button = event.button;
			event.type = RSGL::mouseButtonPressed;
			break;
		
		case ButtonRelease:
			pressed = false;
			event.type = RSGL::mouseButtonReleased;
			event.button = event.button;
			break;

		case MotionNotify:
			// if the x/y changes, fetch the x/y with query
			if (XQueryPointer(display, d, &root, &child, &x, &y, &x, &y, &m))
				event.mouse = { x, y }; 
			event.type = RSGL::mousePosChanged;
			break;

		case ClientMessage: 
			// if the client closed the window
			if ( E.xclient.data.l[0] == (long int)XInternAtom(display, "WM_DELETE_WINDOW", true))
				event.type = RSGL::quit;
			break;

		default:
			// double check if anything is weird
			if (XQueryPointer(display, d, &root, &child, &x, &y, &x, &y, &m))
				if (m){ // if there actually is an button pressed/released, say
					if (pressed)
						event.type = RSGL::mouseButtonPressed;
					else 
						event.type = RSGL::mouseButtonReleased; // else something was released

					// find out which button is [un]pressed
					if (m == 256) event.button = 1;
					else if (m == 212) event.button = 2;
					else if (m == 1024) event.button = 3;
					
					pressed = !pressed; // switch pressed 
				}
			break;
	}
	
	// make sure the window attrubutes are up-to-date
	XWindowAttributes a;
	XGetWindowAttributes(display, d, &a); 

	if (r.w != a.width && areesize || r.h != a.height  && areesize)
		glViewport(0, 0, a.width, a.height); // if the size changed, tell opengl

	r = {a.x, a.y, a.width, a.height}; // update the window r
	
	// if the key has a weird name, change it
	XKeyboardState keystate;
	if (event.key == "ISO_Left_Tab") event.key = "Tab";

	if (isRoot) ::root=*this; // keep root up-to-date if this is the root window
}

void RSGL::window::close(){
	XDestroyWindow(display, d); // close the window
	XCloseDisplay(display); // kill the display
}

void RSGL::window::clear(){
	if (GPU==1){
		glXMakeCurrent(display, d, context); // switch the current GL context to this wondow
		glFlush(); // flush the window
		
		if (swapFlag) // swap the buffer
			glXSwapBuffers( display, d );
		
		// clear the window
		glClearColor(color.r/250.0, color.g/250.0, color.b/250.0, color.a/250.0); 
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void RSGL::window::setColor(RSGL::color c){ 
	color = c; // change win.color to c 
	XSetWindowBackground(display, d, RSGLRGBTOHEX(c.r, c.g, c.b)); // update it on X11's end
}

bool RSGL::window::isPressed(unsigned long key) {
	KeyCode kc2 = XKeysymToKeycode(display, key); // convert the key to a keycode
	return !!(keyboard[kc2 >> 3] & (1 << (kc2 & 7))); // check if the key is pressed
}


bool RSGL::window::isPressed(std::string key){ 
	// swap the names if it's a weird name (and check if L or R are pressed if it's not specific)
	if (key == "Tab") key = "ISO_Left_Tab";
	else if (key == "Shift") 
		return (isPressed(XStringToKeysym("Shift_L")) || isPressed(XStringToKeysym("Shift_R")));
	else if (key == "Control") 
		return (isPressed(XStringToKeysym("Control_L")) || isPressed(XStringToKeysym("Control_R")));
	
	return isPressed(XStringToKeysym(key.data())); 
}

void RSGL::func::start(bool running , void (*gameLoop)(RSGL::window), void (*eventLoop)(int,RSGL::window), RSGL::window win){
	// no comment
	while (running){
		win.checkEvents();
		eventLoop(win.event.type, win);
		gameLoop(win);
	}
	win.close();
}

void RSGL::func::start(bool* running , void (*gameLoop)(RSGL::window), void (*eventLoop)(int,RSGL::window), RSGL::window win){
	while (*running){ // oo, but this one converts the bool* to bool, still no comment
		win.checkEvents();
		eventLoop(win.event.type, win);
		gameLoop(win);
	}
	win.close();
}

RSGL::rect RSGL::fullscreen(){
	// fetch the default screen and check it's size
	Screen*  scrn = DefaultScreenOfDisplay(::root.display); 
	return {0, 0, scrn->width,scrn->height};
}