#ifndef MG_WINDOW_H
#define MG_WINDOW_H

#include <ruby.h>

/**
 * Window class.
 */
static VALUE mg_window_class = Qnil;

/**
 * Allocates memory for the native window.
 */
extern VALUE mg_window_alloc(VALUE klass);

/**
 * Creates a new native window at the given coordinates with the given size and name.
 */
extern VALUE mg_window_initialize(VALUE self, VALUE name, VALUE x, VALUE y, VALUE w, VALUE h);

/**
 * Returns the X coordinate of the window.
 */
extern VALUE mg_window_x(VALUE self);

/**
 * Returns the Y coordinate of the window.
 */
extern VALUE mg_window_y(VALUE self);

/**
 * Returns the width of the window.
 */
extern VALUE mg_window_w(VALUE self);

/**
 * Returns the height of the window.
 */
extern VALUE mg_window_h(VALUE self);

/**
 * Returns the name of the window.
 */
extern VALUE mg_window_name(VALUE self);

/**
 * Returns whether or not the window is visible.
 */
extern VALUE mg_window_visible(VALUE self);

/**
 * Sets the X coordinate of the window.
 */
extern VALUE mg_window_set_x(VALUE self, VALUE x);

/**
 * Sets the Y coordinate of the window.
 */
extern VALUE mg_window_set_y(VALUE self, VALUE y);

/**
 * Sets the width of the window.
 */
extern VALUE mg_window_set_w(VALUE self, VALUE w);

/**
 * Sets the height coordinate of the window.
 */
extern VALUE mg_window_set_h(VALUE self, VALUE h);

/**
 * Sets the position of the window in 2D space.
 */
extern VALUE mg_window_set_pos(VALUE self, VALUE x, VALUE y);

/**
 * Sets the size of the window.
 */
extern VALUE mg_window_set_size(VALUE self, VALUE w, VALUE h);

/**
 * Sets the area of the window.
 */
extern VALUE mg_window_set_area(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h);

/**
 * Sets the visibility of the window.
 */
extern VALUE mg_window_set_visible(VALUE self, VALUE visible);

/**
 * Sets the name of the window.
 */
extern VALUE mg_window_set_name(VALUE self, VALUE name);

/**
 * Makes the window span the entire screen.
 */
extern VALUE mg_window_set_fullscreen(VALUE self, VALUE fs);

/**
 * Defines method under Window class.
 */
extern void def_mg_window_method(const char * name, VALUE (*func)(), int argc);

/**
 * Defines an alias to a method under Window class.
 */
extern void def_mg_window_alias(const char * alias, const char * old);

/**
 * Ruby Window class initialization.
 */
extern void init_mg_window_class_under(VALUE module);

#endif /* MG_WINDOW_H */
