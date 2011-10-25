#include "X11_Window.h"

#include "event.h"

#include <stdlib.h>

#include <ruby.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <stdio.h>

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

/* Constant definitions */

static int attribute_list[] = { GLX_RGBA, GLX_DOUBLEBUFFER,
                                GLX_RED_SIZE,   8,
                                GLX_GREEN_SIZE, 8,
                                GLX_BLUE_SIZE,  8,
                                GLX_DEPTH_SIZE, 24,
                                None };

static const unsigned long event_mask = FocusChangeMask     |
                                        ButtonPressMask     |
                                        ButtonReleaseMask   |
                                        ButtonMotionMask    |
                                        PointerMotionMask   |
                                        KeyPressMask        |
                                        KeyReleaseMask      |
                                        StructureNotifyMask |
                                        EnterWindowMask     |
                                        LeaveWindowMask;

/* Helper function prototypes */

struct event_struct {
    VALUE self;
    XEvent event;
    Atom close_event_atom;
};

typedef struct event_struct event_data_t;

/**
 * Processes events in a parallel thread.
 *
 * data should point to the Window Ruby object whose events are to be processed.
 */
static void process_events(void * data);

/**
 * Unblocking function that stops the event filter. Called by the Ruby runtime
 * if the event processing thread gets killed, the VM gets shutdown, etc.
 *
 * data should point to the Window Ruby object whose events were being processed
 * by the thread.
 */
static void stop_event_filter(void * data);

/**
 * Handles the event and calls the appropriate callbacks on the Ruby object.
 */
static void handle_event(event_data_t * event_data);

/**
 * Get the Ruby event handler arguments for the passed key event.
 */
static VALUE get_args_for_key(XKeyEvent event);

/**
 * Converts the given key to its corresponding Ruby symbol.
 */
static ID X11_key_to_ruby_symbol(KeySym key);

/**
 * Returns whether the event originated from the window.
 */
static Bool is_from(Display * display, XEvent * event, XPointer window);

/**
 * Applies all changes made to the Window.
 */
static inline void flush(X11_Window *);

/* X11_Window interface implementation */

X11_Window * X11_Window_new(void) {
    return malloc(sizeof(X11_Window));
}

void X11_Window_free(void * p) {
    X11_Window * w = (X11_Window *) p;
    XLockDisplay(w->display);
    if (w->context) {
        if (glXGetCurrentContext() == w->context) {
            glXMakeCurrent(w->display, None, 0);
        }
        glXDestroyContext(w->display, w->context);
    }
    XDestroyWindow(w->display, w->window);
    XUnlockDisplay(w->display);
    XCloseDisplay(w->display);
    free(w);
}

void X11_Window_create(X11_Window * w,
                       int x, int y,
                       unsigned int width, unsigned int height) {
    Colormap colormap;
    XSetWindowAttributes attributes;
    XVisualInfo * visual_info = 0;
    unsigned long attribute_value_mask = 0, white = 0;

    /* Connect to the X11 Display Server */
    w->display = XOpenDisplay(0);

    /* If a connection could not be established... */
    if (w->display == 0) {
        /* Raise an error */
        rb_raise(rb_eRuntimeError, "could not connect to the X Server");
    }

    /* Ensure exclusive access to the Display */
    XLockDisplay(w->display);

    /* If OpenGL isn't supported... */
    if (!glXQueryExtension(w->display, 0, 0)) {
        /* Close the connection to the X11 Display Server ... */
        XCloseDisplay(w->display);
        /* ... and raise an error */
        rb_raise(rb_eRuntimeError, "OpenGL is not supported by this X Server");
    }

    /* The window will be created on the default screen */
    w->screen = XDefaultScreen(w->display);

    /* Choose best visual that supports double buffering */
    visual_info = glXChooseVisual(w->display, w->screen, attribute_list);

    /* If no visual was chosen... */
    if (visual_info == 0) {
        /* Rendering with the given specifications is impossible. */
        XCloseDisplay(w->display);
        rb_raise(rb_eRuntimeError, "Rendering not supported");
    }

    /* Create the OpenGL context */
    w->context = glXCreateContext(w->display, visual_info, 0, 1);

    /* Obtain the value of the white color for this display and screen */
    white = XWhitePixel(w->display, visual_info->screen);

    /* Set the window attributes */
    attributes.event_mask = event_mask;
    attributes.border_pixel = attributes.background_pixel = white;

    /* Indicate which attributes have been set */
    attribute_value_mask = CWEventMask   |
                           CWBorderPixel |
                           CWBackPixel;

    /* Create color map */
    colormap = XCreateColormap(w->display,
                               RootWindow(w->display, w->screen),
                               visual_info->visual, AllocNone);

    /* If the color map has been successfully created... */
    if (colormap) {
        /* Set the appropriate attribute */
        attributes.colormap = colormap;

        /* Indicate that it has been set */
        attribute_value_mask |= CWColormap;
    }

    /* Create the window */
    w->window = XCreateWindow(w->display,
                              RootWindow(w->display, visual_info->screen),
                              x, y,
                              width, height, 0,
                              visual_info->depth,
                              InputOutput, visual_info->visual,
                              attribute_value_mask,
                              &attributes);

    /* Obtain the close event atom */
    w->close_event_atom = XInternAtom(w->display, "WM_DELETE_WINDOW", 0);

    /* Configure the window to use it */
    XSetWMProtocols(w->display, w->window, &w->close_event_atom, 1);

    /* Connect the OpenGL context to the window
     * Multiple Windows: maybe move this line to before rendering is done in
     * each window?
     */
    glXMakeCurrent(w->display, w->window, w->context);

    /* Release the Display */
    XUnlockDisplay(w->display);

    /* Free the visual info */
    XFree(visual_info);
}

XWindowAttributes X11_Window_get_attributes(X11_Window * w) {
    XWindowAttributes attributes;
    XLockDisplay(w->display);
    XGetWindowAttributes(w->display, w->window, &attributes);
    XUnlockDisplay(w->display);
    return attributes;
}

const char * X11_Window_name(X11_Window * w) {
    char * name;
    XLockDisplay(w->display);
    XFetchName(w->display, w->window, &name);
    XUnlockDisplay(w->display);
    return name;
}

void X11_Window_set_name(X11_Window * w, const char * name) {
    XLockDisplay(w->display);
    XStoreName(w->display, w->window, name);
    flush(w);
    XUnlockDisplay(w->display);
}

void X11_Window_set_pos(X11_Window * w, int x, int y) {
    XLockDisplay(w->display);
    XMoveWindow(w->display, w->window, x, y);
    flush(w);
    XUnlockDisplay(w->display);
}

void X11_Window_set_size(X11_Window * w, unsigned int width, unsigned int height) {
    XLockDisplay(w->display);
    XResizeWindow(w->display, w->window, width, height);
    flush(w);
    XUnlockDisplay(w->display);
}

void X11_Window_set_area(X11_Window * w, int x, int y, unsigned int width, unsigned int height) {
    XLockDisplay(w->display);
    XMoveResizeWindow(w->display, w->window, x, y, width, height);
    flush(w);
    XUnlockDisplay(w->display);
}

void X11_Window_set_visible(X11_Window * w, int visible) {
    XLockDisplay(w->display);
    if (visible) {
        XMapWindow(w->display, w->window);
    } else {
        XUnmapWindow(w->display, w->window);
    }
    flush(w);
    XUnlockDisplay(w->display);
}

int X11_Window_visible(X11_Window * w) {
    return X11_Window_get_attributes(w).map_state != IsUnmapped;
}

void X11_Window_set_fullscreen(X11_Window * w, int fs) {
    XLockDisplay(w->display);
    if (X11_Window_visible(w)) {
        
    } else {
        Hints h;
        Atom property;
        memset(&h, 0, sizeof(Hints));
        h.flags = 2;
        property = XInternAtom(w->display, "_MOTIF_WM_HINTS", True);
        if (property != 0) {
            XChangeProperty(w->display, w->window, property, property, 32,
                            PropModeReplace, (unsigned char *) &h, 5);
        } else {
            // Failed
        }
    }
    XUnlockDisplay(w->display);
}

void X11_Window_set_fs(X11_Window * w, int fs) {
    XLockDisplay(w->display);
    if (X11_Window_visible(w)) {
        XEvent e;
        memset(&e, 0, sizeof(XEvent));
        e.xany.type = ClientMessage;
        e.xclient.message_type = _NET_WM_STATE;
        e.xclient.format = 32;
        e.xclient.window = w->window;
        e.xclient.data.l[0] = fs ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
        e.xclient.data.l[1] = _NET_WM_STATE_FULLSCREEN;
        e.xclient.data.l[3] = 0L;
        XSendEvent(w->display, RootWindow(w->display, w->screen),
                   0, SubstructureNotifyMask | SubstructureRedirectMask, &e);
    } else {
        int count = 0;
        Atom atoms[1];
        if (fs) {
            atoms[count++] = _NET_WM_STATE_FULLSCREEN;
        }
        if (count > 0) {
            XChangeProperty(w->display, w->window, _NET_WM_STATE, XA_ATOM, 32,
                            PropModeReplace, (unsigned char *) atoms, count);
        } else {
            XDeleteProperty(w->display, w->window, _NET_WM_STATE);
        }
        flush(w);
    }
    XUnlockDisplay(w->display);
}

void X11_Window_event_filter(VALUE self) {
    rb_thread_call_without_gvl(process_events, &self, stop_event_filter, &self);
}

/* Helper function implementation */

static void process_events(void * data) {
    event_data_t event_data;
    X11_Window * w = 0;
    VALUE * v = (VALUE *) data;

    event_data.self = *v;

    /* Retrieve the window data from the Ruby object */
    Data_Get_Struct(event_data.self, X11_Window, w);

    event_data.close_event_atom = w->close_event_atom;

    w->event_loop_running = 1;

    while (w->event_loop_running) {
        XLockDisplay(w->display);
        /* Handle only events that originated from the window */
        while (XCheckIfEvent(w->display, &event_data.event, is_from, (XPointer) w->window)) {
            rb_thread_call_with_gvl(handle_event, &event_data);
        }
        XUnlockDisplay(w->display);
    }
}

static void stop_event_filter(void * data) {
    X11_Window * w = 0;
    VALUE * v = (VALUE *) data;
    VALUE self = *v;

    /* Retrieve the window data from the Ruby object */
    Data_Get_Struct(self, X11_Window, w);

    XLockDisplay(w->display);

    w->event_loop_running = 0;

    XUnlockDisplay(w->display);
}

static void handle_event(event_data_t * event_data) {
    /* Process each kind of event and call the appropriate handler */
    switch (event_data->event.type) {
        /* Window is about to be destroyed by X */
        case DestroyNotify: {
            break;
        }
        /* Window was closed by the user */
        case ClientMessage: {
            if (event_data->event.xclient.format == 32 &&
                event_data->event.xclient.data.l[0] == (long) event_data->close_event_atom) {
                mg_event_call_close_handler(event_data->self);
            }
            break;
        }
        /* Key was pressed */
        case KeyPress: {
            mg_event_call_key_press_handler(event_data->self,
                                            get_args_for_key(event_data->event.xkey));
            break;
        }
        /* Key was released */
        case KeyRelease: {
            mg_event_call_key_release_handler(event_data->self,
                                              get_args_for_key(event_data->event.xkey));
            break;
        }
    }
}

static VALUE get_args_for_key(XKeyEvent event) {
    static const int buffer_size = 32;
    char buffer[buffer_size];
    KeySym key;
    VALUE args;

    /* Check which key was pressed */
    XLookupString(&event, buffer, buffer_size, &key, 0);

    /* Initialize Ruby argument array */
    args = rb_ary_new();

    /* Get the key's Ruby symbol and push it into the array */
    rb_ary_push(args, X11_key_to_ruby_symbol(key));

    /* Return the argument list */
    return args;
}

static VALUE X11_key_to_ruby_symbol(KeySym key) {
    KeySym lower, upper;

    /* Convert to upper case so that we can deal with only one type of key */
    XConvertCase(key, &lower, &upper);

    /* Convert key to the corresponding Ruby symbol */
    switch (upper) {
        case XK_A: return mg_event_keyboard_key_a_symbol;
        case XK_B: return mg_event_keyboard_key_b_symbol;
        case XK_C: return mg_event_keyboard_key_c_symbol;
        case XK_D: return mg_event_keyboard_key_d_symbol;
        case XK_E: return mg_event_keyboard_key_e_symbol;
        case XK_F: return mg_event_keyboard_key_f_symbol;
        case XK_G: return mg_event_keyboard_key_g_symbol;
        case XK_H: return mg_event_keyboard_key_h_symbol;
        case XK_I: return mg_event_keyboard_key_i_symbol;
        case XK_J: return mg_event_keyboard_key_j_symbol;
        case XK_K: return mg_event_keyboard_key_k_symbol;
        case XK_L: return mg_event_keyboard_key_l_symbol;
        case XK_M: return mg_event_keyboard_key_m_symbol;
        case XK_N: return mg_event_keyboard_key_n_symbol;
        case XK_O: return mg_event_keyboard_key_o_symbol;
        case XK_P: return mg_event_keyboard_key_p_symbol;
        case XK_Q: return mg_event_keyboard_key_q_symbol;
        case XK_R: return mg_event_keyboard_key_r_symbol;
        case XK_S: return mg_event_keyboard_key_s_symbol;
        case XK_T: return mg_event_keyboard_key_t_symbol;
        case XK_U: return mg_event_keyboard_key_u_symbol;
        case XK_V: return mg_event_keyboard_key_v_symbol;
        case XK_W: return mg_event_keyboard_key_w_symbol;
        case XK_X: return mg_event_keyboard_key_x_symbol;
        case XK_Y: return mg_event_keyboard_key_y_symbol;
        case XK_Z: return mg_event_keyboard_key_z_symbol;
        default:   return mg_event_keyboard_key_unsupported_symbol;
    }

    /* If we somehow get here, just return unsupported */
    return mg_event_keyboard_key_unsupported_symbol;
}

static Bool is_from(Display * display, XEvent * event, XPointer arg) {
    return event->xany.window == (Window) arg;
}

static inline void flush(X11_Window * w) {
    XFlush(w->display);
}
