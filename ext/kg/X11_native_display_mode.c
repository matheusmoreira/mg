#include "X11_native_display_mode.h"

#include "display_mode.h"

#include <ruby.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

/* DisplayData structure definition */

struct DisplayData {
    Display * display;
    int screen;
    XRRScreenConfiguration * screen_configuration;
};

/* Function prototypes */

/**
 * Opens a connection to the X Server, obtains the default screen and
 * information about its configuration. Stores all this data in an instance of
 * the DisplayData structure and then invokes the function pointer, passing it
 * as the parameter.
 *
 * This function is responsible for freeing the screen configuration information
 * and closing the connection to the X Server.
 */
static VALUE with_current_screen_configuration(VALUE (*f)(struct DisplayData *));

/**
 * Finds the current screen size and depth, given the DisplayData, and returns a
 * Mg::DisplayMode Ruby object containing the information for the current
 * display mode.
 */
static VALUE find_current_display_mode(struct DisplayData *);

/**
 * Finds all available screen sizes and depths, given the DisplayData, and
 * returns a Ruby array containing Mg::DisplayMode ruby objects, one for each
 * available display configuration.
 */
static VALUE find_all_display_modes(struct DisplayData *);

/* Native interface implementation */

/**
 * Returns all available display modes in a Ruby array.
 */
VALUE mg_native_display_mode_get_modes(VALUE klass) {
    return with_current_screen_configuration(&find_all_display_modes);
}

/**
 * Returns the current display mode.
 */
VALUE mg_native_display_mode_get_current_mode(VALUE klass) {
    return with_current_screen_configuration(&find_current_display_mode);
}

/* Helper function prototypes */

/**
 * Allocates memory for a new instance of the DisplayData structure and returns
 * a pointer to it.
 */
static struct DisplayData * DisplayData_new();

/**
 * Checks if the display is valid. Raises a Ruby exception if it isn't.
 */
static void check_display(struct DisplayData *);

/**
 * Checks if the Xrandr extension is present on the X Server to which the
 * display is connected to. Closes the connection and raises a Ruby exception if
 * the extension isn't present.
 *
 * This method assumes that the Display is valid.
 */
static void look_for_Xrandr(struct DisplayData *);

/**
 * Checks if the screen configuration is valid. Closes the display and raises a
 * Ruby exception if it isn't.
 *
 * This method assumes that both the Display is valid and that Xrandr extension
 * is present.
 */
static void check_screen_configuration(struct DisplayData *);

/* Helper function implementation */

static VALUE with_current_screen_configuration(VALUE (*f)(struct DisplayData *)) {
    VALUE value = Qnil;
    struct DisplayData * data = DisplayData_new();

    /* Open connection to the X Display Server */
    data->display = XOpenDisplay(0);
    check_display(data);

    /* Raise if Xrandr is not present */
    look_for_Xrandr(data);

    /* Get the default screen identifier for this display */
    data->screen = XDefaultScreen(data->display);

    /* Get screen information */
    data->screen_configuration = XRRGetScreenInfo(data->display,
                                                  RootWindow(data->display,
                                                             data->screen));

    /* Raise if the screen configuration is invalid */
    check_screen_configuration(data);

    /* Call the function and store its result */
    value = f(data);

    /* Free the screen information structure */
    XRRFreeScreenConfigInfo(data->screen_configuration);

    /* Close the connection to the display server */
    XCloseDisplay(data->display);

    /* Free display data */
    free(data);

    /* Return the result of the function */
    return value;
}

static VALUE find_current_display_mode(struct DisplayData * data) {
    VALUE mode = Qnil;
    XRRScreenSize * sizes = 0;
    Rotation currentRotation;
    int numSizes = 0, currentMode = 0;

    /* Get current display configuration */
    currentMode = XRRConfigCurrentConfiguration(data->screen_configuration,
                                                &currentRotation);

    /* Get all avaliable screen dimensions */
    sizes = XRRConfigSizes(data->screen_configuration, &numSizes);

    /* If at least one screen size could be retrieved... */
    if (numSizes > 0) {

        /*
         * Create a new Mg::DisplayMode containing the width and height of the
         * current display mode, and the value of the default depth for this
         * X server and screen.
         */
        mode = mg_display_mode_new_c(sizes[currentMode].width,
                                     sizes[currentMode].height,
                                     DefaultDepth(data->display, data->screen));
    }

    /* return the current display mode */
    return mode;
}

static VALUE find_all_display_modes(struct DisplayData * data) {
    VALUE mode = Qnil, modes = Qnil;
    XRRScreenSize * sizes = 0;
    int * depths = 0;
    int i, j, numSizes = 0, numDepths = 0;

    /* Retrieve screen size */
    numSizes = 0;
    sizes = XRRConfigSizes(data->screen_configuration, &numSizes);

    /* If at least one screen configuration could be retrieved... */
    if (numSizes > 0) {

        /* Retrieve screen depths. */
        numDepths = 0;
        depths = XListDepths(data->display, data->screen, &numDepths);

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
                    mode = mg_display_mode_new_c(sizes[j].width,
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

    /* Return the available display modes */
    return modes;
}

static void check_display(struct DisplayData * data) {
    if (data->display == 0) {
        rb_raise(rb_eRuntimeError, "could not open Display");
    }
}

static void look_for_Xrandr(struct DisplayData * data) {
    int check;
    if (!XQueryExtension(data->display, "RANDR", &check, &check, &check)) {
        XCloseDisplay(data->display);
        rb_raise(rb_eRuntimeError, "Xrandr is not present");
    }
}

static void check_screen_configuration(struct DisplayData * data) {
    if (data->screen_configuration == 0) {
        XCloseDisplay(data->display);
        rb_raise(rb_eRuntimeError, "could not retrieve screen configuration");
    }
}

static struct DisplayData * DisplayData_new() {
    return malloc(sizeof(struct DisplayData));
}
