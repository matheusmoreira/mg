#ifndef MG_X11_NATIVE_WINDOW_H
#define MG_X11_NATIVE_WINDOW_H

#include "X11_Window.h"

#include <ruby.h>

/**
 * Retrieves a X11_Window from a ruby object.
 */
extern X11_Window * window_from(VALUE obj);

/**
 * Allocates memory for X11_Window and stores it in the object.
 */
extern VALUE native_window_alloc(VALUE klass);

/**
 * Creates a window with the specified parameters.
 */
extern void native_window_init(VALUE self,
                               const char * name,
                               int x, int y,
                               unsigned int w, unsigned int h);

/**
 * Returns the X coordinate of the window.
 */
extern int native_window_x(VALUE self);

/**
 * Returns the Y coordinate of the window.
 */
extern int native_window_y(VALUE self);

/**
 * Returns the width coordinate of the window.
 */
extern unsigned int native_window_w(VALUE self);

/**
 * Returns the height coordinate of the window.
 */
extern unsigned int native_window_h(VALUE self);

/**
 * Returns the name of the window.
 */
extern const char * native_window_name(VALUE self);

/**
 * Returns zero if the window is visible, anything else if not.
 */
extern int native_window_visible(VALUE self);

/**
 * Sets the position of the window in 2D space.
 */
extern void native_window_set_pos(VALUE self, int x, int y);

/**
 * Sets the size of the window.
 */
extern void native_window_set_size(VALUE self, unsigned int w, unsigned int h);

/**
 * Sets the area of the window.
 */
extern void native_window_set_area(VALUE self, int x, int y, unsigned int w, unsigned int h);

/**
 * Sets the visibility of the window.
 */
extern void native_window_set_visible(VALUE self, int visible);

/**
 * Sets the X coordinate of the window.
 */
extern void native_window_set_x(VALUE self, int x);

/**
 * Sets the Y coordinate of the window.
 */
extern void native_window_set_y(VALUE self, int y);

/**
 * Sets the width of the window.
 */
extern void native_window_set_w(VALUE self, unsigned int w);

/**
 * Sets the height of the window.
 */
extern void native_window_set_h(VALUE self, unsigned int h);

/**
 * Sets the name of the window.
 */
extern void native_window_set_name(VALUE self, const char * name);

/**
 * Make the window span the entire screen.
 */
extern void native_window_set_fullscreen(VALUE self, int fs);

#endif /* MG_X11_NATIVE_WINDOW_H */
