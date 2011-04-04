#include "X11_NativeDisplayMode.h"

#include "DisplayMode.h"

#include <ruby.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

VALUE native_display_mode_get_current_mode(VALUE klass) {
    Display * d = XOpenDisplay(0);
    if (d == 0) {
        rb_raise(rb_eRuntimeError, "could not open Display");
    }
    int check, s = XDefaultScreen(d);
    if (!XQueryExtension(d, "RANDR", &check, &check, &check)) {
        rb_raise(rb_eRuntimeError, "Xrandr is not present - cannot get supported video modes");
    }
    XRRScreenConfiguration * config = XRRGetScreenInfo(d, RootWindow(d, s));
    if (config == 0) {
        rb_raise(rb_eRuntimeError, "could not retrieve screen configuration");
    }
    Rotation currentRotation;
    int currentMode = XRRConfigCurrentConfiguration(config, &currentRotation);
    int numSizes = 0;
    XRRScreenSize * sizes = XRRConfigSizes(config, &numSizes);
    VALUE mode = Qnil;
    if (numSizes > 0) {
        mode = rb_funcall(Mg_Display_Mode_Class,
                          rb_intern("new"),
                          3,
                          INT2FIX(sizes[currentMode].width),
                          INT2FIX(sizes[currentMode].height),
                          INT2FIX(DefaultDepth(d, s)));
    }
    return mode;
}

/**
 * Returns all available modes in a Ruby array.
 */
VALUE native_display_mode_get_modes(VALUE klass) {
    Display * d = XOpenDisplay(0);
    if (d == 0) {
        rb_raise(rb_eRuntimeError, "could not open Display");
    }
    int check, s = XDefaultScreen(d);
    if (!XQueryExtension(d, "RANDR", &check, &check, &check)) {
        rb_raise(rb_eRuntimeError, "Xrandr is not present - cannot get supported video modes");
    }
    XRRScreenConfiguration * config = XRRGetScreenInfo(d, RootWindow(d, s));
    if (config == 0) {
        rb_raise(rb_eRuntimeError, "could not retrieve screen configuration");
    }
    VALUE modes = rb_ary_new();
    int numSizes = 0;
    XRRScreenSize * sizes = XRRConfigSizes(config, &numSizes);
    if (numSizes > 0) {
        int numDepths = 0;
        int * depths = XListDepths(d, s, &numDepths);
        if (numDepths > 0) {
            int i;
            for (i = 0; i < numDepths; ++i) {
                int j;
                for (j = 0; j < numSizes; ++j) {
                    VALUE mode = rb_funcall(Mg_Display_Mode_Class,
                                            rb_intern("new"),
                                            3,
                                            INT2FIX(sizes[j].width),
                                            INT2FIX(sizes[j].height),
                                            INT2FIX(depths[i]));
                    rb_ary_push(modes, mode);
                }
            }
        }
    }
    return modes;
}
