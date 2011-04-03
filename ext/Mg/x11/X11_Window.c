#include <X11/Xlib.h>

/**
 * Contains data for a X11 window.
 */
typedef struct {
    Display * display; /** Pointer to the display on which the window will be created. */
    int screen; /** Screen on the display. */
    Window window; /** The window. */
} X11_Window;

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
