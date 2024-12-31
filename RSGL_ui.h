
/* 
*******
RSGL_gui
*******
*/

#ifdef RSGL_IMPLEMENTATION
#define RSGL_UI_IMPLEMENTATION
#endif

#ifndef RSGL_UI_H

#define RSGL_UI_H

/* 
******************
	RSGL_UI_BACKEND 
	this stuff is used by RSGL for abstracting backends
	or used by backend modules
******************
*/

typedef RSGL_ENUM(u8, RSGL_event_types) {
	/*! event codes */
 	RSGL_keyPressed = 1, /* a key has been pressed */
	RSGL_keyReleased, /*!< a key has been released*/
	RSGL_mouseButtonPressed, /*!< a mouse button has been pressed (left,middle,right)*/
	RSGL_mouseButtonReleased, /*!< a mouse button has been released (left,middle,right)*/
	RSGL_mousePosChanged, /*!< the position of the mouse has been changed*/
};

typedef RSGL_ENUM(u8, RSGL_mouseCodes) {
	RSGL_mouseLeft  =  1, /*!< left mouse button is pressed*/
	RSGL_mouseMiddle =  2, /*!< mouse-wheel-button is pressed*/
	RGL_mouseRight  = 3, /*!< right mouse button is pressed*/
	RSGL_mouseScrollUp =  4, /*!< mouse wheel is scrolling up*/
	RSGL_mouseScrollDown =  5 /*!< mouse wheel is scrolling down*/
};

typedef RSGL_ENUM(u8, RSGL_mouseIcons) {
	RSGL_MOUSE_ARROW = 1,
	RSGL_MOUSE_IBEAM  = 2,
	RSGL_MOUSE_POINTING_HAND = 4,
};

typedef RSGL_ENUM(u8, RSGL_keys) { 
	RSGL_key_none = 0,
	RSGL_Up,
	RSGL_Down,
	RSGL_Left,
	RSGL_Right,
	RSGL_Tab,
	RSGL_BackSpace,
	RSGL_last_key
};

typedef struct {
    u32 type;  /*!< event type */
    RSGL_point point; /*!< cursor point */
    u8 button; /*!< which mouse button has been clicked (0) left (1) middle (2) right OR which joystick button was pressed*/
    double scroll; /*!< the raw mouse scroll value */
    u32 keyCode; /*!< code of key pressed */
	char ch; /*!< key char */
} RSGL_Event;

/*!< it checks if the key is pressed globally */
RSGLDEF b8 RSGL_isPressed(u32 key); /*!< if key is pressed (key code)*/
RSGLDEF b8 RSGL_isReleased(u32 key); /*!< if key is released (key code)*/


/*!, if mouse button is pressed globally */
RSGLDEF b8 RSGL_isMousePressed(u32 key);
RSGLDEF b8 RSGL_isMouseReleased(u32 key);

/*!< get current mouse point */
RSGLDEF RSGL_point RSGL_getMousePoint(void);

/*!< send event to RSGL (should be used by the platform in RSGL_checkEvent ) */ 
RSGLDEF void RSGL_sendEvent(void* userPtr, RSGL_Event event);

/*! these functions are to be defined by the backend */

 /*!< sets the mouse to an standard mouse */
RSGLDEF void RSGL_setMouseStandard(void* userPtr, u32 mouse);

/*! mark released keys as not released after its frame is done (should be used by the platform in RSGL_checkEvent */ 
RSGLDEF void RSGL_clearReleased(void);

/*!< check events and update internal data for event *
* returns true if there was an event */
RSGLDEF b8 RSGL_checkEvent(void* userPtr);

/*
	RSGL UI
*/

typedef RSGL_ENUM(u8, RSGL_widgetState) {
	RSGL_STATE_NONE = 0,
	RSGL_HOVERED = 1,
	RSGL_PRESSED = 2,
	RSGL_RELEASED = 3,
};

typedef RSGL_ENUM(u32, RSGL_widgetStyle) {
	RSGL_STYLE_NONE = 0,
	RSGL_OFFSET_X = (1 << 1), // the next widget will be offset by this widget's width
	RSGL_OFFSET_Y = (1 << 2), // the next widget will be offset by this widget's height
	RSGL_OFFSET_MODE = RSGL_OFFSET_X | RSGL_OFFSET_Y,

	RSGL_STYLE_ROUND = (1 << 3), // draw button as a rounded rectangle
	RSGL_SHAPE_POLYGON = (1 << 4), // draw a polygon instead of a rect
	RSGL_NO_SHAPE = (1 << 5), // do not render the widget

	RSGL_SHAPE_INFO = RSGL_STYLE_ROUND | RSGL_SHAPE_POLYGON | RSGL_NO_SHAPE,
	/* 
		if either of these flags are enabled 
		RSGL's default style system is used
	*/
	RSGL_STYLE_DARK = (1L << 6), /* dark mode*/
    RSGL_STYLE_LIGHT = (1L << 7), /* light mode*/
    RSGL_STYLE_MODE  = RSGL_STYLE_LIGHT | RSGL_STYLE_DARK,
	
	RSGL_NO_TITLE = (1L << 8), /* do not draw cantainer handle */
	RSGL_NO_CONTAINER = (1L << 9), /* do not draw container */ 

	RSGL_SLIDER_CIRCLE = (1L << 10), 

	RSGL_STYLE_RED = (1 << 18),
    RSGL_STYLE_BLUE = (1 << 19),
    RSGL_STYLE_GREEN = (1 << 20),
    RSGL_STYLE_YELLOW = (1 << 21),
    RSGL_STYLE_TEAL = (1 << 22),
    RSGL_STYLE_PURPLE = (1 << 23),
    RSGL_STYLE_COLOR = RSGL_STYLE_RED | RSGL_STYLE_BLUE | RSGL_STYLE_GREEN | RSGL_STYLE_YELLOW | RSGL_STYLE_TEAL | RSGL_STYLE_PURPLE
};


RSGLDEF void RSGL_openBlankContainer(RSGL_rect r);
RSGLDEF void RSGL_openContainer(const char* name, RSGL_rect* r, RSGL_color background, u32 args, b8* grabbed, b8* toggle);

RSGLDEF void RSGL_widgetOutline(u32 outline, RSGL_color idle, RSGL_color hover, RSGL_color press, RSGL_color release);
RSGLDEF void RSGL_widgetColor(RSGL_color idle, RSGL_color hover, RSGL_color press, RSGL_color release);
RSGLDEF void RSGL_widgetPolygonPoints(u32 points); 
RSGLDEF void RSGL_widgetRounding(RSGL_point point);
RSGLDEF void RSGL_widgetAlign(u8 align);

RSGLDEF void RSGL_evalTheme(RSGL_widgetStyle buttonStyle);
RSGLDEF void RSGL_scaleRect(RSGL_rectF* rect, RSGL_widgetStyle args);

RSGLDEF void RSGL_label(const char* str, RSGL_rectF);
RSGLDEF RSGL_widgetState RSGL_labeledButton(const char* str, RSGL_rectF rect, RSGL_widgetStyle args);

RSGLDEF void RSGL_separator(RSGL_rectF rect); 

RSGLDEF void RSGL_openSubContainer(RSGL_rectF rect, u32 args, b8* open);
RSGLDEF void RSGL_closeSubContainer(void);

RSGLDEF RSGL_widgetState RSGL_button(RSGL_rectF rect, RSGL_widgetStyle args);
RSGLDEF b8 RSGL_toggleButton(RSGL_rectF rect, RSGL_widgetStyle args, b8* toggle);
RSGLDEF b8 RSGL_checkbox(RSGL_rectF rect, RSGL_widgetStyle args, b8* checked);
RSGLDEF void RSGL_grab(RSGL_rectF rect, RSGL_widgetStyle args, b8* grabbed);
RSGLDEF void RSGL_slider(RSGL_rectF rect, RSGL_widgetStyle args, float* value, b8* grabbed);
RSGLDEF RSGL_widgetState RSGL_radioButtons(RSGL_rectF rect, u32 count, RSGL_widgetStyle args, u8* selected, u8* index);
RSGLDEF b8 RSGL_combobox(RSGL_rectF rect, u32 args, char* strings[], size_t len, b8* open, size_t* index); 
#endif /* ndef RSGL_UI_H */

#ifdef RSGL_UI_IMPLEMENTATION

/*
********************
RSGL_UI_BACKEND
*********************
*/

typedef struct RSGL_key {
	u8 pressed : 1;
	u8 released : 1;
} RSGL_key;

RSGL_key RSGL_keyMap[RSGL_last_key] = {{0}};
RSGL_key RSGL_mouseMap[5] = {{0}};

RSGL_point RSGL_mousePoint = {0, 0};
/* where the mouse was when it was pressed, this is used checking if a button was released */
RSGL_point RSGL_mousePressPoint = {0, 0};
b8 RSGL_wasMousePosChanged = 0;

char RSGL_charQueue[20] = {0};
size_t RSGL_charQueue_size = 0; 

u8 RSGL_isPressed(u32 key) { return RSGL_keyMap[key].pressed; }

b8 RSGL_isReleased(u32 key) { return RSGL_keyMap[key].released; } 
b8 RSGL_isMousePressed(u32 button) { return RSGL_mouseMap[button].pressed; }
b8 RSGL_isMouseReleased(u32 button) { return RSGL_mouseMap[button].released; }
RSGL_point RSGL_getMousePoint(void) { return RSGL_mousePoint; }

void RSGL_sendEvent(void* userPtr, RSGL_Event event) {	
	switch (event.type) {
		case RSGL_keyPressed:
			RSGL_keyMap[event.keyCode].released = 0;	
			RSGL_keyMap[event.keyCode].pressed = 1;
			
			RSGL_charQueue[RSGL_charQueue_size] = event.ch;
			RSGL_charQueue_size += 1;
			break;
		case RSGL_keyReleased:
			RSGL_keyMap[event.keyCode].pressed = 0;	
			RSGL_keyMap[event.keyCode].released = 1;	
			break;
		case RSGL_mouseButtonPressed:
			RSGL_mouseMap[event.button].released = 0;
			RSGL_mouseMap[event.button].pressed = 1;
			RSGL_mousePressPoint = RSGL_mousePoint;
			break;
		case RSGL_mouseButtonReleased:
			RSGL_mouseMap[event.button].pressed = 0;
			RSGL_mouseMap[event.button].released = 1;
			break;
		case RSGL_mousePosChanged:
			RSGL_mousePoint = event.point;
			RSGL_wasMousePosChanged = 1;
			break;
	}
}

void RSGL_clearReleased(void) {
	size_t i;
	for (i = 0; i < RSGL_last_key; i++)
		RSGL_keyMap[i].released = 0;	
	for (i = 0; i < 5; i++)
		RSGL_mouseMap[i].released = 0;

	RSGL_wasMousePosChanged = 0;
}

/* RSGL_UI */

struct RSGL_widgetInfo {
	RSGL_rect r;
	RSGL_color color[4]; // 1 color per stat 
	u32 points; 
	RSGL_point rounding;

	u32 outline;
	RSGL_color outlineColor[4];

	RSGL_point padding;
	u32 textSize;
	u8 align;
	
	RSGL_point curPoint;
	RSGL_point offset;

	b8 canDraw : 1;
	RSGL_point grab;
} RSGL_widgetInfo;

RSGLDEF RSGL_widgetState RSGL_doToggleButton(RSGL_rectF rect, RSGL_widgetStyle args, b8* toggle);
RSGLDEF void RSGL_drawButton(RSGL_rectF rect, RSGL_color color[4], RSGL_color outlineColor[4], u32 args, u32 state);
	

void RSGL_openBlankContainer(RSGL_rect r) {
	RSGL_openContainer(NULL, &r, RSGL_RGB(0, 0, 0), RSGL_NO_CONTAINER, NULL, NULL);
}

void RSGL_openContainer(const char* name, RSGL_rect* r, RSGL_color background, u32 args, b8* grabbed, b8* toggle) {
	RSGL_widgetInfo.r = *r;
	RSGL_widgetInfo.offset = RSGL_POINT(0, 0);
	RSGL_widgetInfo.curPoint = RSGL_POINT(0, 0);
	RSGL_widgetInfo.outline = 0;
	RSGL_widgetInfo.rounding = RSGL_POINT(20, 20);
	
	RSGL_widgetInfo.align = RSGL_ALIGN_LEFT | RSGL_ALIGN_DOWN;
	RSGL_widgetInfo.textSize = 25;
	RSGL_widgetInfo.padding = RSGL_POINT(0, 0);

	RSGL_widgetInfo.canDraw = 1;
	
	if (args & RSGL_NO_CONTAINER)
		return;

	if (args & RSGL_NO_TITLE) {
		RSGL_drawRect(*r, background);
		return;
	}

	RSGL_rectF rect = RSGL_RECTF(r->x, r->y - 30, r->w - (r->w * 0.10), 30);

	b8 oldGrab = *grabbed;
	RSGL_grab(rect, args | RSGL_NO_SHAPE, grabbed);		
	rect.w = r->w;
	
	if (!oldGrab && *grabbed) {
		RSGL_widgetInfo.grab = RSGL_mousePoint;
		
		RSGL_widgetInfo.grab.x = rect.x - RSGL_widgetInfo.grab.x;
		RSGL_widgetInfo.grab.y = rect.y - RSGL_widgetInfo.grab.y;
	}

	if (*grabbed && RSGL_wasMousePosChanged) {
		rect.x = RSGL_mousePoint.x + RSGL_widgetInfo.grab.x;
		if (rect.x < 0) rect.x = 0;

		r->x = rect.x;
			
		i32 y = RSGL_mousePoint.y + RSGL_widgetInfo.grab.y + 30;
		if (y - 30 < 0) y = 30;

		r->y = y;
		rect.y = r->y - 30;

		RSGL_widgetInfo.r = *r;
	}			

	RSGL_doToggleButton(RSGL_RECTF(rect.x + (rect.w - (rect.w * 0.10)), rect.y, rect.w * 0.10, rect.h), args, toggle);
	
	if (*toggle)
		RSGL_drawRect(*r, background);
	
	RSGL_drawButton(rect, RSGL_widgetInfo.color, RSGL_widgetInfo.outlineColor, args, 0); 
	
	if (name != NULL) {
		RSGL_label(name, rect);	
	}

	RSGL_widgetInfo.canDraw = *toggle;

	if (*toggle == false) {
		RSGL_widgetInfo.canDraw = 0;
		RSGL_drawTriangleF((RSGL_triangleF){	
										RSGL_POINTF(rect.x + (rect.w * 0.90) , rect.y + ( rect.h * 0.15)),
										RSGL_POINTF(rect.x + (rect.w * 0.90), rect.y + (rect.h * 0.90)),
										RSGL_POINTF(rect.x + (rect.w * 0.99), rect.y + (rect.h * 0.50)),
									 },
									RSGL_widgetInfo.outlineColor[1]);

		return;
	}

	RSGL_drawTriangleF((RSGL_triangleF){	
										RSGL_POINTF(rect.x + (rect.w * 0.90) , rect.y + ( rect.h * 0.15)),
										RSGL_POINTF(rect.x + (rect.w * 0.98), rect.y + (rect.h * 0.15)),
										RSGL_POINTF(rect.x + (rect.w * 0.94), rect.y + (rect.h * 0.90)),
									 },
									RSGL_widgetInfo.outlineColor[1]);
}

void RSGL_widgetOutline(u32 outline, RSGL_color idle, RSGL_color hover, RSGL_color press, RSGL_color release) {
	RSGL_widgetInfo.outline = outline;
	RSGL_widgetInfo.outlineColor[0] = idle;
	RSGL_widgetInfo.outlineColor[1] = hover;
	RSGL_widgetInfo.outlineColor[2] = press;
	RSGL_widgetInfo.outlineColor[3] = release;
}

void RSGL_widgetColor(RSGL_color idle, RSGL_color hover, RSGL_color press, RSGL_color release) {
	RSGL_widgetInfo.color[0] = idle;
	RSGL_widgetInfo.color[1] = hover;
	RSGL_widgetInfo.color[2] = press;
	RSGL_widgetInfo.color[3] = release;
}

void RSGL_widgetPolygonPoints(u32 points) {
	RSGL_widgetInfo.points = points;	
}

void RSGL_widgetRounding(RSGL_point point) {
	RSGL_widgetInfo.rounding = point;	
}

void RSGL_widgetAlign(u8 align) {
	RSGL_widgetInfo.align = align;
}

void RSGL_evalTheme(RSGL_widgetStyle buttonStyle) {
	if ((buttonStyle & RSGL_STYLE_MODE) == 0)
		return;

    switch (buttonStyle & RSGL_STYLE_MODE) {
        case RSGL_STYLE_DARK: {
            RSGL_color c = RSGL_RGB(20, 20, 20);

            switch (buttonStyle & RSGL_STYLE_COLOR) {
                case RSGL_STYLE_RED:
                    c.r += 15;
                    break;
                case RSGL_STYLE_BLUE:
                    c.b += 15;
                    break;
                case RSGL_STYLE_GREEN:
                    c.g += 15;
                    break;
                case RSGL_STYLE_YELLOW:
                    c.r += 15;
                    c.g += 15;
                    break;
                case RSGL_STYLE_TEAL:
                    c.g += 15;
                    c.b += 15;
                    break;
                case RSGL_STYLE_PURPLE:
                    c.r += 15;
                    c.b += 15;
                    break;
                default: break;
            }
			
			RSGL_color outlineColor = RSGL_RGB(c.r + 40, c.g + 40, c.b + 40);
            RSGL_widgetOutline(1, outlineColor, outlineColor, outlineColor, outlineColor);
            
			RSGL_color select = RSGL_RGB(c.r + 60, c.g + 60, c.b + 60);
			RSGL_widgetOutline(RSGL_widgetInfo.outline, RSGL_widgetInfo.outlineColor[0],  select, RSGL_widgetInfo.outlineColor[1], RSGL_widgetInfo.outlineColor[3]);
			RSGL_widgetColor(c, c, RSGL_RGB(c.r + 10, c.g + 10, c.b + 10), c);
            break;
        }
        case RSGL_STYLE_LIGHT: {
            RSGL_color c = RSGL_RGB(200, 200, 200);

            switch (buttonStyle & RSGL_STYLE_COLOR) {
                case RSGL_STYLE_RED:
                    c.g -= 60;
                    c.b -= 60;
                    break;
                case RSGL_STYLE_BLUE:
                    c.r -= 60;
                    c.g -= 60;
                    break;
                case RSGL_STYLE_GREEN:
                    c.r -= 60;
                    c.b -= 60;
                    break;
                case RSGL_STYLE_YELLOW:
                    c.b -= 60;
                    break;
                case RSGL_STYLE_TEAL:
                    c.r -= 60;
                    break;
                case RSGL_STYLE_PURPLE:
                    c.g -= 60;
                    break;
                default: break;
            }

            RSGL_widgetOutline(1, RSGL_RGB(c.r - 140, c.g - 140, c.b - 140), RSGL_RGB(c.r - 140, c.g - 140, c.b - 140), RSGL_RGB(c.r - 140, c.g - 140, c.b - 140), RSGL_RGB(c.r - 140, c.g - 140, c.b - 140));
            
            RSGL_widgetColor(c, c, RSGL_RGB(c.r - 20, c.g - 20, c.b - 20), c);

			RSGL_widgetOutline(1, RSGL_widgetInfo.outlineColor[0], RSGL_RGB(0, 0, 0), RSGL_widgetInfo.outlineColor[2], RSGL_widgetInfo.outlineColor[3]);
            break;
        }
        default: break;
    }
	
}

void RSGL_scaleRect(RSGL_rectF* rect, RSGL_widgetStyle args) {
	if (rect == NULL) return; // This shouldn't happen, but just in case 
	
	if (rect->w < 1)
		rect->w *= RSGL_widgetInfo.r.w; 
	if (rect->h < 1)
		rect->h *= RSGL_widgetInfo.r.h;

	if (rect->x < 1) {
		rect->x = ((RSGL_widgetInfo.r.w * rect->x) - (rect->w * rect->x)) + RSGL_widgetInfo.r.x;
	}
	if (rect->y < 1) {
		rect->y = ((RSGL_widgetInfo.r.h * rect->y) - (rect->h * rect->y)) + RSGL_widgetInfo.r.y;
	}

	if (RSGL_widgetInfo.curPoint.x == rect->x)
		rect->x += RSGL_widgetInfo.offset.x;
	else
		RSGL_widgetInfo.offset.x = 0;

	if (RSGL_widgetInfo.curPoint.y == rect->y) 
		rect->y += RSGL_widgetInfo.offset.y;
	else
		RSGL_widgetInfo.offset.y = 0;

	switch (args & RSGL_OFFSET_MODE) {
		case RSGL_OFFSET_X:
			RSGL_widgetInfo.offset.x = rect->x + rect->w;
			RSGL_widgetInfo.curPoint.x = rect->x;
			break;
		case RSGL_OFFSET_Y:
			RSGL_widgetInfo.offset.y = rect->y + rect->h;
			RSGL_widgetInfo.curPoint.y = rect->y;
			break;
		default:  break;
	}
}

void RSGL_label(const char* str, RSGL_rectF rect) {
	if (RSGL_widgetInfo.canDraw == 0)
		return;
	
	RSGL_circle circle = RSGL_alignText(str, RSGL_CIRCLE(0, 0, RSGL_widgetInfo.textSize), rect, RSGL_widgetInfo.align);
	RSGL_rectF trueRect = RSGL_RECTF(RSGL_widgetInfo.padding.x + circle.x, RSGL_widgetInfo.padding.y + circle.y, circle.d, circle.d);
	RSGL_scaleRect(&trueRect, 0);
	RSGL_drawText(str, RSGL_CIRCLE(trueRect.x, trueRect.y, trueRect.h), RSGL_widgetInfo.outlineColor[1]);
}

void RSGL_separator(RSGL_rectF rect) {
	if (RSGL_widgetInfo.canDraw == 0)
		return;
	
	RSGL_scaleRect(&rect, RSGL_OFFSET_X | RSGL_OFFSET_Y);
}

RSGL_widgetState RSGL_labeledButton(const char* str, RSGL_rectF rect, RSGL_widgetStyle args) {
	RSGL_scaleRect(&rect, args);

	RSGL_widgetState state = RSGL_button(rect, args);
	RSGL_label(str, rect);

	return state;
}

void RSGL_drawButton(RSGL_rectF rect, RSGL_color color[4], RSGL_color outlineColor[4], u32 args, u32 state) {
	if (RSGL_widgetInfo.outline) {
		RSGL_color color = outlineColor[state];
		u32 outline = RSGL_widgetInfo.outline;

		RSGL_rectF rectOutline = RSGL_RECTF(rect.x - outline, rect.y - outline, rect.w + (outline * 2), rect.h + (outline * 2));

		switch (args & RSGL_SHAPE_INFO) {
			case RSGL_STYLE_ROUND:
				RSGL_drawRoundRectF(rectOutline, RSGL_widgetInfo.rounding, color);
				break;	
			case RSGL_SHAPE_POLYGON:
				RSGL_drawPolygonF(rectOutline, RSGL_widgetInfo.points, color);
				break;
			default:
				RSGL_drawRectF(rectOutline, color);
				break;
		}
	}
	
	switch (args & RSGL_SHAPE_INFO) {
		case RSGL_STYLE_ROUND:
			RSGL_drawRoundRectF(rect, RSGL_widgetInfo.rounding, color[state]);
			break;
		case RSGL_SHAPE_POLYGON:
			RSGL_drawPolygonF(rect, RSGL_widgetInfo.points, color[state]);
			break;
		default:
			RSGL_drawRectF(rect, color[state]);
			break;
	}
}

RSGL_widgetState RSGL_doToggleButton(RSGL_rectF rect, RSGL_widgetStyle args, b8* toggle) {
	if (toggle == NULL) return 0;
	
	RSGL_widgetState state = RSGL_button(rect, args | RSGL_NO_SHAPE); 
	if (state == RSGL_RELEASED) {
		*toggle = !(*toggle);
	}
		
	return state;
}

b8 RSGL_toggleButton(RSGL_rectF rect, RSGL_widgetStyle args, b8* toggle) {
	if (RSGL_widgetInfo.canDraw == 0)
		return 0;

	b8 state = RSGL_doToggleButton(rect, args, toggle);	
	
	RSGL_scaleRect(&rect, args);

	u32 points = RSGL_widgetInfo.points;
	RSGL_point rounding = RSGL_widgetInfo.rounding;

	if (RSGL_widgetInfo.rounding.x == 20 && RSGL_widgetInfo.rounding.x == 20 && args & RSGL_STYLE_MODE)
		RSGL_widgetRounding(RSGL_POINT(rect.w / 2, rect.h));

	RSGL_drawButton(rect, RSGL_widgetInfo.color, RSGL_widgetInfo.color, args, state);

	if (args & RSGL_STYLE_MODE) {
		if (args & RSGL_STYLE_ROUND)
			args ^= RSGL_STYLE_ROUND;

		float verts = ((2 * M_PI * (rect.h * 0.75)) / 10);
		verts = (verts > 360 ? 360 : verts);

		RSGL_widgetPolygonPoints(verts);
		args |= RSGL_SHAPE_POLYGON;
	}
	
	RSGL_drawButton(RSGL_RECTF(rect.x + (rect.w * 0.10) + (*toggle * (rect.w * 0.45)), rect.y + (rect.h * 0.14), (rect.h * 0.75), (rect.h * 0.75)), 
								RSGL_widgetInfo.outlineColor, RSGL_widgetInfo.outlineColor, args, state != 0);

	RSGL_widgetInfo.points = points;
	RSGL_widgetInfo.rounding = rounding;
	return (state == RSGL_PRESSED);
}

b8 RSGL_checkbox(RSGL_rectF rect, RSGL_widgetStyle args, b8* checked) {
	if (RSGL_widgetInfo.canDraw == 0)
		return 0;

	b8 state = RSGL_doToggleButton(rect, args, checked);	

	RSGL_scaleRect(&rect, args);
	if (*checked) {
		RSGL_drawButton(rect, RSGL_widgetInfo.outlineColor, RSGL_widgetInfo.color, args, state);
		RSGL_drawLineF(RSGL_POINTF(rect.x + (rect.w * 0.22), rect.y + (rect.h * 0.55)), RSGL_POINTF(rect.x + (rect.w * 0.40), rect.y + (rect.h * 0.75)), 4, RSGL_widgetInfo.color[state]);
		RSGL_drawLineF(RSGL_POINTF(rect.x + (rect.w * 0.40), rect.y + (rect.h * 0.75)), RSGL_POINTF(rect.x + (rect.w * 0.80), rect.y + (rect.h * 0.30)), 4, RSGL_widgetInfo.color[state]);

	}
	else	
		RSGL_drawButton(rect, RSGL_widgetInfo.color, RSGL_widgetInfo.outlineColor, args, state);
	
	return state;
}

void RSGL_openSubContainer(RSGL_rectF rect, u32 args, b8* open) {
	if (RSGL_widgetInfo.canDraw == 0)
		return;
		
	RSGL_doToggleButton(rect, args, open);
	RSGL_button(rect, args);

	RSGL_scaleRect(&rect, args);

	if (*open == false) {
		RSGL_widgetInfo.canDraw = 0;
		RSGL_drawTriangleF((RSGL_triangleF){	
										RSGL_POINTF(rect.x + (rect.w * 0.70) , rect.y + ( rect.h * 0.15)),
										RSGL_POINTF(rect.x + (rect.w * 0.70), rect.y + (rect.h * 0.90)),
										RSGL_POINTF(rect.x + (rect.w * 0.93), rect.y + (rect.h * 0.50)),
									 },
									RSGL_widgetInfo.outlineColor[1]);

		return;
	}

	RSGL_drawTriangleF((RSGL_triangleF){	
										RSGL_POINTF(rect.x + (rect.w * 0.70) , rect.y + ( rect.h * 0.15)),
										RSGL_POINTF(rect.x + (rect.w * 0.90), rect.y + (rect.h * 0.15)),
										RSGL_POINTF(rect.x + (rect.w * 0.80), rect.y + (rect.h * 0.90)),
									 },
									RSGL_widgetInfo.outlineColor[1]);
}

void RSGL_closeSubContainer(void) {
	RSGL_widgetInfo.canDraw = 1;
}


b8 RSGL_combobox(RSGL_rectF rect, u32 args, char* strings[], size_t len, b8* open, size_t* index) {
	if (RSGL_widgetInfo.canDraw == 0)
		return 0;
	
	b8 offsetY = 0;
	if (args & RSGL_OFFSET_Y) {
		offsetY = 1;
		args ^= RSGL_OFFSET_Y;
	}

	RSGL_openSubContainer(rect, args, open);
	
	RSGL_scaleRect(&rect, args);
	
	b8 draw = RSGL_widgetInfo.canDraw;
	RSGL_widgetInfo.canDraw = 1;
	
	RSGL_label(strings[*index], rect);	
	RSGL_widgetInfo.canDraw = draw;
	
	size_t i;
	for (i = 0; i < len; i++) {
		rect.y += rect.h;
		RSGL_widgetState state = RSGL_button(rect, args);	

		RSGL_label(strings[i], rect);	

		if (state == RSGL_RELEASED) {
			*open = 0;
			*index = i;
			return 1;
		}
	}
	
	if (offsetY) {
		RSGL_separator(rect);
	}

	RSGL_closeSubContainer();

	return 0;
}

void RSGL_grab(RSGL_rectF rect, RSGL_widgetStyle args, b8* grabbed) {
	if (RSGL_isMouseReleased(RSGL_mouseLeft) || RSGL_isMousePressed(RSGL_mouseLeft) == 0)
		*grabbed = false;

	if (RSGL_button(rect, args) == RSGL_PRESSED)
		*grabbed = true; 
}

void RSGL_slider(RSGL_rectF rect, RSGL_widgetStyle args, float* value, b8* grabbed) {
	if (RSGL_widgetInfo.canDraw == 0)
		return;

	RSGL_scaleRect(&rect, 0);
	RSGL_grab(rect, args, grabbed); 
	if (*grabbed) {
		if (rect.w > rect.h) {	
			if (RSGL_mousePoint.x - rect.x < 0)
				*value = 0.0f;
			else if (RSGL_mousePoint.x > rect.x + rect.w)
				*value = 1.0f;
			else
				*value = (RSGL_mousePoint.x - rect.x) / rect.w;
		} else {
			if (RSGL_mousePoint.y - rect.y < 0)
				*value = 0.0f;
			else if (RSGL_mousePoint.y > rect.y + rect.h)
				*value = 1.0f;
			else
				*value = (RSGL_mousePoint.y - rect.y) / rect.h;
		}
	}		
	
	RSGL_widgetState state;

	RSGL_rectF clip_rect;

	if (rect.w > rect.h) {
		clip_rect = RSGL_RECTF((rect.x - 10) + ((rect.w - rect.h) * (*value)), rect.y - 10, rect.h + 20, rect.h + 20);
		state = RSGL_button(rect, args | RSGL_NO_SHAPE); 
	}
	else {
		clip_rect = RSGL_RECTF((rect.x - 10) + ((rect.w - rect.h) * (*value)), rect.y - 10, rect.h + 20, rect.h + 20);
		state = RSGL_button(rect, args | RSGL_NO_SHAPE); 
	}
	
	if (args & RSGL_SLIDER_CIRCLE) {
		if (args & RSGL_STYLE_ROUND)
			args ^= RSGL_STYLE_ROUND;

		args |= RSGL_SHAPE_POLYGON;
		RSGL_widgetInfo.points = 36;
	}

	
	if (RSGL_button(clip_rect, args | RSGL_NO_SHAPE) == RSGL_PRESSED)
		state = RSGL_PRESSED;

	RSGL_drawButton(clip_rect, RSGL_widgetInfo.outlineColor, RSGL_widgetInfo.color, args, state);

	if (state == RSGL_PRESSED)
		*grabbed = 1;
}
	
RSGL_widgetState RSGL_radioButtons(RSGL_rectF rect, u32 count, RSGL_widgetStyle args, u8* selected, u8* index) {
	if (RSGL_widgetInfo.canDraw == 0)
		return 0;

	RSGL_scaleRect(&rect, 0);

	RSGL_pointF offset = RSGL_POINTF(0, 0);
	switch (args & RSGL_OFFSET_MODE) {
		case RSGL_OFFSET_X: offset.x = 10; break;
		case RSGL_OFFSET_Y: offset.y = 10; break;
		default: offset.y = rect.h + 10; break;
	}
	
	RSGL_widgetState final_state = 0;

	size_t i;
	for (i = 0; i < count; i++) { 
		RSGL_widgetState state =  RSGL_button(rect, args | RSGL_NO_SHAPE);

		if (state) {
			final_state = state;
			*index = i;
		
			if (state == RSGL_RELEASED)
				*selected = i;
		}

		RSGL_rectF trueRect = rect;
		RSGL_scaleRect(&trueRect, args);
		
		RSGL_drawButton(trueRect, RSGL_widgetInfo.color, RSGL_widgetInfo.color, args, state);
		
		if (state == RSGL_HOVERED || (*selected == i)) {
				RSGL_drawButton(RSGL_RECTF(trueRect.x + (trueRect.w * 0.25), trueRect.y + (trueRect.h * 0.25), trueRect.w * 0.52, trueRect.h * 0.52), 
										RSGL_widgetInfo.outlineColor, RSGL_widgetInfo.outlineColor, args, !state);
		}

		rect.x += offset.x;
		rect.y += offset.y;	
	}

	return final_state;
}

RSGL_widgetState RSGL_button(RSGL_rectF rect, RSGL_widgetStyle args) {
	if (RSGL_widgetInfo.canDraw == 0)
		return 0;

	RSGL_scaleRect(&rect, args);
	
	RSGL_evalTheme(args);

	u32 state = 0;
	if (RSGL_rectCollidePointF(rect, RSGL_POINTF(RSGL_mousePoint.x, RSGL_mousePoint.y))) {
		state = RSGL_HOVERED;
		if (RSGL_isMousePressed(RSGL_mouseLeft) && RSGL_rectCollidePointF(rect, RSGL_POINTF(RSGL_mousePressPoint.x, RSGL_mousePressPoint.y)))
			state = RSGL_PRESSED;
		else if (RSGL_isMouseReleased(RSGL_mouseLeft) && RSGL_rectCollidePointF(rect, RSGL_POINTF(RSGL_mousePressPoint.x, RSGL_mousePressPoint.y))) {
				state = RSGL_RELEASED;
		}
	}

	
	if (args & RSGL_NO_SHAPE)
		return state;
	
	RSGL_drawButton(rect, RSGL_widgetInfo.color, RSGL_widgetInfo.outlineColor, args, state);
	return state;
}


#endif /*  RSGL_UI_IMPLEMENTATION */
