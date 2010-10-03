#ifndef WM_H
#define WM_H

#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>

void wm_start(Display *display, int area_left, int area_top, int area_width, int area_height);
void wm_stop();
void wm_process_events();

#endif
