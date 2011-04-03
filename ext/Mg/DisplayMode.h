#ifndef MG_DISPLAY_MODE_H
#define MG_DISPLAY_MODE_H

#include <ruby.h>

/**
 * DisplayMode class.
 */
static VALUE Mg_DisplayMode_Class = Qnil;

/**
 * Returns the display mode that's currently in use.
 */
static VALUE display_mode_get_current_mode(VALUE klass);

/**
 * Returns all available display modes in a Ruby array.
 */
static VALUE display_mode_get_modes(VALUE klass);

/**
 * Initializes the DisplayMode class.
 */
static void init_display_mode_class_under(VALUE module);

#endif /* MG_DISPLAY_MODE_H */
