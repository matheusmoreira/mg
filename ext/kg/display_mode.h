#ifndef MG_DISPLAY_MODE_H
#define MG_DISPLAY_MODE_H

#include <ruby.h>

/**
 * DisplayMode class.
 */
static VALUE mg_display_mode_class = Qnil;

/**
 * Returns the display mode that's currently in use.
 */
extern VALUE mg_display_mode_get_current_mode(VALUE klass);

/**
 * Returns all available display modes in a Ruby array.
 */
extern VALUE mg_display_mode_get_modes(VALUE klass);

/**
 * Returns a new DisplayMode instance.
 */
extern VALUE mg_display_mode_new(VALUE w, VALUE h, VALUE bpp);

/**
 * Returns a new DisplayMode instance.
 * Convenience method that takes C integers.
 */
extern VALUE mg_display_mode_new_c(int w, int h, int bpp);

/**
 * Initializes the DisplayMode class.
 */
extern void init_mg_display_mode_class_under(VALUE module);

#endif /* MG_DISPLAY_MODE_H */
