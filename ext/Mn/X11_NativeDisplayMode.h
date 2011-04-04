#ifndef MG_X11_NATIVE_DISPLAY_MODE_H
#define MG_X11_NATIVE_DISPLAY_MODE_H

#include <ruby.h>

/**
 * Initializes a DisplayMode object with the current display mode.
 */
extern VALUE native_display_mode_get_current_mode(VALUE klass);

/**
 * Returns all available modes in a Ruby array.
 */
extern VALUE native_display_mode_get_modes(VALUE klass);

#endif /* MG_X11_NATIVE_DISPLAY_MODE_H */
