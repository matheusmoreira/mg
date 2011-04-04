#include "X11_Window.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>

X11_Window * X11_Window_new(void) {
    return malloc(sizeof(X11_Window));
}

void X11_Window_init(X11_Window * w) {
    w->display = XOpenDisplay(0);
    if (w->display == 0) {
        rb_raise(rb_eRuntimeError, "could not open Display");
    }
    w->screen = XDefaultScreen(w->display);
}

void X11_Window_create(X11_Window * w, int x, int y, unsigned int width, unsigned int height) {
    int white = XWhitePixel(w->display, w->screen);
    w->window = XCreateSimpleWindow(w->display,
                                    XDefaultRootWindow(w->display),
                                    x, y, width, height,
                                    0, white, white);
}

void X11_Window_free(void * p) {
    X11_Window * w = p;
    XDestroyWindow(w->display, w->window);
    XCloseDisplay(w->display);
    free(w);
}

XWindowAttributes X11_Window_get_attributes(X11_Window * w) {
    XWindowAttributes attributes;
    XGetWindowAttributes(w->display, w->window, &attributes);
    return attributes;
}

const char * X11_Window_name(X11_Window * w) {
    char * name;
    XFetchName(w->display, w->window, &name);
    return name;
}

void X11_Window_set_name(X11_Window * w, const char * name) {
    XStoreName(w->display, w->window, name);
    XFlush(w->display);
}

void X11_Window_set_pos(X11_Window * w, int x, int y) {
    XMoveWindow(w->display, w->window, x, y);
    XFlush(w->display);
}

void X11_Window_set_size(X11_Window * w, unsigned int width, unsigned int height) {
    XResizeWindow(w->display, w->window, width, height);
    XFlush(w->display);
}

void X11_Window_set_area(X11_Window * w, int x, int y, unsigned int width, unsigned int height) {
    XMoveResizeWindow(w->display, w->window, x, y, width, height);
    XFlush(w->display);
}

void X11_Window_set_visible(X11_Window * w, int visible) {
    if (visible) {
        XMapWindow(w->display, w->window);
    } else {
        XUnmapWindow(w->display, w->window);
    }
    XFlush(w->display);
}

int X11_Window_visible(X11_Window * w) {
    return X11_Window_get_attributes(w).map_state != IsUnmapped;
}

void X11_Window_set_fullscreen(X11_Window * w, int fs) {
    if (X11_Window_visible(w)) {
        
    } else {
        Hints h;
        Atom property;
        memset(&h, 0, sizeof(Hints));
        h.flags = 2;
        property = XInternAtom(w->display, "_MOTIF_WM_HINTS", True);
        if (property != 0) {
            XChangeProperty(w->display, w->window, property, property, 32,
                            PropModeReplace, (unsigned char *) &h, 5);
        } else {
            // Failed
        }
    }
}

void X11_Window_set_fs(X11_Window * w, int fs) {
    if (X11_Window_visible(w)) {
        XEvent e;
        memset(&e, 0, sizeof(XEvent));
        e.xany.type = ClientMessage;
        e.xclient.message_type = _NET_WM_STATE;
        e.xclient.format = 32;
        e.xclient.window = w->window;
        e.xclient.data.l[0] = fs ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
        e.xclient.data.l[1] = _NET_WM_STATE_FULLSCREEN;
        e.xclient.data.l[3] = 0L;
        XSendEvent(w->display, RootWindow(w->display, w->screen),
                   0, SubstructureNotifyMask | SubstructureRedirectMask, &e);
    } else {
        int count = 0;
        Atom atoms[1];
        if (fs) {
            atoms[count++] = _NET_WM_STATE_FULLSCREEN;
        }
        if (count > 0) {
            XChangeProperty(w->display, w->window, _NET_WM_STATE, XA_ATOM, 32,
                            PropModeReplace, (unsigned char *) atoms, count);
        } else {
            XDeleteProperty(w->display, w->window, _NET_WM_STATE);
        }
        XFlush(w->display);
    }
}
