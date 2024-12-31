#ifndef RSGL_BACKEND /* all backends must start like this */
#define RSGL_BACKEND /* this is show RSGL knows if a backend has been loaded or not*/

#define RGFW_IMPLEMENTATION

#include "RGFW.h"

#define RSGL_point RGFW_point
#define RSGL_area RGFW_area
#define RSGL_rect RGFW_rect

#ifndef RSGL_H
#include "RSGL.h"
#include "RSGL_ui.h"
#endif

void RSGL_setMouseStandard(void* userPtr, u32 mouse) {
	RGFW_window_setMouseStandard((RGFW_window*)userPtr, mouse);
}

u8 RSGL_convertKey(u8 key) {
	switch (key) {
		case RGFW_Up: return RSGL_Up;
		case RGFW_Down: return RSGL_Down;
		case RGFW_Left: return RSGL_Left;
		case RGFW_Right: return RSGL_Right;
		case RGFW_Tab: return RSGL_Tab;
		case RGFW_BackSpace: return RSGL_BackSpace;  
		default: return 0;
	}
}

b8 RSGL_checkEvent(void* userPtr) {
	RGFW_window* win = (RGFW_window*)userPtr;
	
	if (win->event.type == 0)
		RSGL_clearReleased();

	if (RGFW_window_checkEvent(win) == NULL) {
		win->event.type = 0;
		return 0;
	}

	RSGL_Event ev = {};
	ev.type = 0;

	switch (win->event.type) {
		case RGFW_keyPressed:
			ev.type = RSGL_keyPressed;
			ev.keyCode = RSGL_convertKey(win->event.key);
			ev.ch = win->event.keyChar;
			break;
		case RGFW_keyReleased:
			ev.type = RSGL_keyReleased;
			ev.keyCode = RSGL_convertKey(win->event.key);
			ev.ch = win->event.keyChar;
			break;
		case RGFW_mouseButtonPressed:
			ev.type = RSGL_mouseButtonPressed;
			ev.button = win->event.button;
			ev.scroll = win->event.scroll;
			break;
		case RGFW_mouseButtonReleased:
			ev.type = RSGL_mouseButtonReleased;
			ev.button = win->event.button;
			ev.scroll = win->event.scroll;
			break;
		case RGFW_mousePosChanged:
			ev.type = RSGL_mousePosChanged;
			ev.point = win->event.point;
			break;
		default: return win->event.type;
	}
	
	RSGL_sendEvent(userPtr, ev);
	
	return ev.type;
}

#endif
