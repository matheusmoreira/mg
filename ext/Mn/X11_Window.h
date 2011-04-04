#ifndef MG_X11_X11_WINDOW_H
#define MG_X11_X11_WINDOW_H

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
extern X11_Window * X11_Window_new(void);

/**
 * Initializes a X11_Window.
 */
extern void X11_Window_init(X11_Window * w);

/**
 * Creates a new display window.
 */
extern void X11_Window_create(X11_Window * w,
                              int x, int y, unsigned int width, unsigned int height);

/**
 * Frees resources and deallocates memory.
 */
extern void X11_Window_free(void * p);

/**
 * Retrieves the attributes of the X11_Window.
 */
extern XWindowAttributes X11_Window_get_attributes(X11_Window * w);

/**
 * Returns the name of the window.
 */
extern const char * X11_Window_name(X11_Window * w);

/**
 * Sets the name of the window.
 */
extern void X11_Window_set_name(X11_Window * w, const char * name);

/**
 * Sets the position of the X11_Window in 2D space.
 */
extern void X11_Window_set_pos(X11_Window * w, int x, int y);

/**
 * Sets the size of the window.
 */
extern void X11_Window_set_size(X11_Window * w, unsigned int width, unsigned int height);

/**
 * Sets the area of the window.
 */
extern void X11_Window_set_area(X11_Window * w,
                                int x, int y, unsigned int width, unsigned int height);

/**
 * Sets the visibility of the window.
 */
extern void X11_Window_set_visible(X11_Window * w, int visible);

/**
 * Returns true if the window has been mapped, false otherwise.
 */
extern int X11_Window_visible(X11_Window * w);

/**
 * Make the window span the entire screen.
 */
extern void X11_Window_set_fullscreen(X11_Window * w, int fs);

/**
 * _NET_WM implementation
 */
extern void X11_Window_set_fs(X11_Window * w, int fs);

#endif /* MG_X11_X11_WINDOW_H */
