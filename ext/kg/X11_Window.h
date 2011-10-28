#ifndef MG_X11_X11_WINDOW_H
#define MG_X11_X11_WINDOW_H

#include <ruby.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <GL/gl.h>
#include <GL/glx.h>

/**
 * Contains data for a X11 window.
 */
typedef struct {
    Display * display; /** Pointer to the display connection. */
    int screen; /** Window's screen. */
    Window window; /** The window. */
    GLXContext context; /** The OpenGL context. */
    Atom close_event_atom; /** Atom that identifies the window close event. */
    int event_loop_running; /** Whether this window's event loop is running. */
} X11_Window;


/**
 * Returns a pointer to newly allocated memory for a X11_Window.
 */
extern X11_Window * X11_Window_new(void);

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

/**
 * Filters X11 window events and calls appropriate callbacks.
 */
extern void X11_Window_event_filter(VALUE self);

/**
 * Starts a new thread that runs an event loop for the window.
 */
extern void X11_Window_start_event_loop_thread_for(VALUE self);

#endif /* MG_X11_X11_WINDOW_H */
