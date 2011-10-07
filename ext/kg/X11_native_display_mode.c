#include "X11_native_display_mode.h"

#include "display_mode.h"

#include <ruby.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

/**
 * Returns the current display mode.
 */
VALUE mg_native_display_mode_get_current_mode(VALUE klass) {
    VALUE mode = Qnil;
    Display * d = 0;
    XRRScreenConfiguration * config = 0;
    XRRScreenSize * sizes = 0;
    int * depths = 0;
    Rotation currentRotation;
    int s, numSizes = 0, currentMode = 0;

    /* Open connection to the X Display Server */
    d = XOpenDisplay(0);

    /* Raise if display is invalid */
    check_display(d);

    /* Raise if Xrandr is not present */
    look_for_Xrandr(d);

    /* Get the default screen identifier for this display */
    s = XDefaultScreen(d);

    /* Get screen information */
    config = XRRGetScreenInfo(d, RootWindow(d, s));

    /* Raise if the screen configuration is invalid */
    check_screen_configuration(config);

    /* Get current display configuration */
    currentMode = XRRConfigCurrentConfiguration(config, &currentRotation);

    /* Get all avaliable screen dimensions */
    sizes = XRRConfigSizes(config, &numSizes);

    /* If at least one screen size could be retrieved... */
    if (numSizes > 0) {

        /*
         * Create a new Mg::DisplayMode containing the width and height of the
         * current display mode, and the value of the default depth for this
         * X server and screen.
         */
        mode = mg_display_mode_new(klass,
                                   sizes[currentMode].width,
                                   sizes[currentMode].height,
                                   DefaultDepth(d, s));
    }

    /* Free the screen information structure */
    XRRFreeScreenConfigInfo(config);

    /* Close the connection to the display server */
    XCloseDisplay(d);

    /* return the current display mode */
    return mode;
}

/**
 * Returns all available modes in a Ruby array.
 */
VALUE mg_native_display_mode_get_modes(VALUE klass) {
    VALUE mode = Qnil, modes = Qnil;
    Display * d = 0;
    XRRScreenConfiguration * config = 0;
    XRRScreenSize * sizes = 0;
    int * depths = 0;
    int s, i, j, numSizes = 0, numDepths = 0;

    /* Open connection to the X Display Server */
    d = XOpenDisplay(0);

    /* Raise if display is invalid */
    check_display(d);

    /* Raise if Xrandr is not present */
    look_for_Xrandr(d);

    /* Get the default screen identifier for this display */
    s = XDefaultScreen(d);

    /* Get screen configuration information */
    config = XRRGetScreenInfo(d, RootWindow(d, s));

    /* Raise if the screen configuration is invalid */
    check_screen_configuration(config);

    /* Retrieve screen size */
    numSizes = 0;
    sizes = XRRConfigSizes(config, &numSizes);

    /* If at least one screen configuration could be retrieved... */
    if (numSizes > 0) {

        /* Retrieve screen depths. */
        numDepths = 0;
        depths = XListDepths(d, s, &numDepths);

        /* If at least one depth could be retrieved... */
        if (numDepths > 0) {

            /* Create new Ruby array in order to store the display modes */
            modes = rb_ary_new();

            /* For every depth at index i... */
            for (i = 0; i < numDepths; ++i) {

                /* And for every screen at index j... */
                for (j = 0; j < numSizes; ++j) {

                    /*
                     * Create a new Mg::DisplayMode containing the width and
                     * height of the current screen, and the value of the
                     * current depth
                     */
                    mode = mg_display_mode_new(klass,
                                               sizes[j].width,
                                               sizes[j].height,
                                               depths[i]);

                    /* Then add it to the ruby array */
                    rb_ary_push(modes, mode);
                }
            }
        }

        /* Free the array of depths */
        XFree(depths);
    }

    /* Free the screen information structure */
    XRRFreeScreenConfigInfo(config);

    /* Close the connection to the display server */
    XCloseDisplay(d);

    /* Return the display modes */
    return modes;
}

void check_display(Display * d) {
    if (d == 0) {
        rb_raise(rb_eRuntimeError, "could not open Display");
    }
}

void look_for_Xrandr(Display * d) {
    int check;
    if (!XQueryExtension(d, "RANDR", &check, &check, &check)) {
        rb_raise(rb_eRuntimeError, "Xrandr is not present");
        return Qnil;
    }
}

void check_screen_configuration(XRRScreenConfiguration * config) {
    if (config == 0) {
        rb_raise(rb_eRuntimeError, "could not retrieve screen configuration");
        return Qnil;
    }
}
