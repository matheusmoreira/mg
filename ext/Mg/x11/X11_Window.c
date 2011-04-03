#include <X11/Xlib.h>
#include <X11/Xatom.h>

/**
 * Contains data for a X11 window.
 */
typedef struct {
    Display * display; /** Pointer to the display on which the window will be created. */
    int screen; /** Screen on the display. */
    Window window; /** The window. */
} X11_Window;

/**
 * Motif window hints
 */
typedef struct {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long          inputMode;
    unsigned long status;
} Hints;

/**
 * Atoms.
 */

#define _NET_WM_STATE_REMOVE 0L
#define _NET_WM_STATE_ADD    1L
#define _NET_WM_STATE_TOGGLE 2L

Atom _NET_WM_STATE;
Atom _NET_WM_STATE_HIDDEN;
Atom _NET_WM_STATE_MAXIMIZED_VERT;
Atom _NET_WM_STATE_MAXIMIZED_HORZ;
Atom _NET_WM_STATE_FULLSCREEN;
Atom _NET_WM_NAME;
Atom _NET_WM_ICON_NAME;
Atom _NET_WM_ICON;


/**
 * Returns a pointer to newly allocated memory for a X11_Window.
 */
static X11_Window * X11_Window_new(void) {
    return malloc(sizeof(X11_Window));
}

/**
 * Initializes a X11_Window.
 */
static void X11_Window_init(X11_Window * w) {
    w->display = XOpenDisplay(0);
    if (w->display == 0) {
        rb_raise(rb_eRuntimeError, "could not open Display");
    }
    w->screen = XDefaultScreen(w->display);
}

/**
 * Creates a new display window.
 */
static void X11_Window_create(X11_Window * w, int x, int y, unsigned int width, unsigned int height) {
    int white = XWhitePixel(w->display, w->screen);
    w->window = XCreateSimpleWindow(w->display,
                                    XDefaultRootWindow(w->display),
                                    x, y, width, height,
                                    0, white, white);
}

/**
 * Frees resources and deallocates memory.
 */
static void X11_Window_free(void * p) {
    X11_Window * w = p;
    XDestroyWindow(w->display, w->window);
    XCloseDisplay(w->display);
}

/**
 * Retrieves the attributes of the X11_Window.
 */
static XWindowAttributes X11_Window_get_attributes(X11_Window * w) {
    XWindowAttributes attributes;
    XGetWindowAttributes(w->display, w->window, &attributes);
    return attributes;
}

/**
 * Returns the name of the window.
 */
static const char * X11_Window_name(X11_Window * w) {
    char * name;
    XFetchName(w->display, w->window, &name);
    return name;
}

/**
 * Sets the name of the window.
 */
static void X11_Window_set_name(X11_Window * w, const char * name) {
    XStoreName(w->display, w->window, name);
    XFlush(w->display);
}

/**
 * Sets the position of the X11_Window in 2D space.
 */
static void X11_Window_set_pos(X11_Window * w, int x, int y) {
    XMoveWindow(w->display, w->window, x, y);
    XFlush(w->display);
}

/**
 * Sets the size of the window.
 */
static void X11_Window_set_size(X11_Window * w, unsigned int width, unsigned int height) {
    XResizeWindow(w->display, w->window, width, height);
    XFlush(w->display);
}

/**
 * Sets the area of the window.
 */
static void X11_Window_set_area(X11_Window * w, int x, int y, unsigned int width, unsigned int height) {
    XMoveResizeWindow(w->display, w->window, x, y, width, height);
    XFlush(w->display);
}

/**
 * Sets the visibility of the window.
 */
static void X11_Window_set_visible(X11_Window * w, int visible) {
    if (visible) {
        XMapWindow(w->display, w->window);
    } else {
        XUnmapWindow(w->display, w->window);
    }
    XFlush(w->display);
}

/**
 * Returns true if the window has been mapped, false otherwise.
 */
static int X11_Window_visible(X11_Window * w) {
    return X11_Window_get_attributes(w).map_state != IsUnmapped;
}

/**
 * Make the window span the entire screen.
 */
static void X11_Window_set_fullscreen(X11_Window * w, int fs) {
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

/**
 * _NET_WM implementation
 */
static void X11_Window_set_fs(X11_Window * w, int fs) {
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
