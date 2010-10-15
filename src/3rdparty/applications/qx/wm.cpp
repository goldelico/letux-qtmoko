#include "wm.h"

static Display *dpy;
static int left, top, width, height;

static void map_request(XEvent *e)
{
    static XWindowAttributes wa;

    XMapRequestEvent *ev = &e->xmaprequest;

    if(!XGetWindowAttributes(dpy, ev->window, &wa))
    {
        return;
    }

    if(wa.override_redirect)
    {
        return;
    }

    XMoveResizeWindow(dpy, ev->window, left, top, width, height); /* some windows require this */
    XMapWindow(dpy, ev->window);
    XSetInputFocus(dpy, ev->window, RevertToPointerRoot, CurrentTime);
}

void wm_process_events()
{
    XEvent ev;
    int num;

    XSync(dpy, False);
    num = XPending(dpy);
    while(num > 0 && !XNextEvent(dpy, &ev))
    {
        if(ev.type == MapRequest)
        {
            map_request(&ev);
        }
        num--;
    }
}

void wm_start(Display *display, int area_left, int area_top, int area_width, int area_height)
{
    XSetWindowAttributes wa;
    int screen;
    Window root;

    dpy = display;
    left = area_left;
    top = area_top;
    width = area_width;
    height = area_height;

    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);
    /* select for events */
    wa.event_mask = SubstructureRedirectMask|SubstructureNotifyMask|ButtonPressMask
                    |EnterWindowMask|LeaveWindowMask|StructureNotifyMask
                    |PropertyChangeMask;

    XChangeWindowAttributes(dpy, root, CWEventMask, &wa);
    XSelectInput(dpy, root, wa.event_mask);
}

void wm_stop()
{
}
