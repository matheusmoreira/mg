#include "X11_native_window.h"

#include "X11_Window.h"

#include <ruby.h>

/* Helper function prototypes */

/**
 * Catches native window events and calls appropriate callbacks.
 *
 * The data should point to a Mg::Window Ruby object.
 */
static VALUE mg_native_window_event_filter(void * data);

/**
 * Returns the encapsulated X11_Window structure from the Ruby object.
 */
static X11_Window * X11_Window_from(VALUE obj);

/* Native window interface implementation */

VALUE mg_native_window_alloc(VALUE klass) {
    X11_Window * w = X11_Window_new();
    // Wrap X11_Window into Ruby VALUE
    return Data_Wrap_Struct(klass, 0, X11_Window_free, w);
}

void mg_native_window_init(VALUE self,
                           const char * name,
                           int x, int y,
                           unsigned int w, unsigned int h) {
    X11_Window * window = X11_Window_from(self);
    X11_Window_create(window, x, y, w, h);
    X11_Window_set_name(window, name);
}

int mg_native_window_x(VALUE self) {
    return X11_Window_get_attributes(X11_Window_from(self)).x;
}

int mg_native_window_y(VALUE self) {
    return X11_Window_get_attributes(X11_Window_from(self)).y;
}

unsigned int mg_native_window_w(VALUE self) {
    return X11_Window_get_attributes(X11_Window_from(self)).width;
}

unsigned int mg_native_window_h(VALUE self) {
    return X11_Window_get_attributes(X11_Window_from(self)).height;
}

const char * mg_native_window_title(VALUE self) {
    return X11_Window_name(X11_Window_from(self));
}

int mg_native_window_visible(VALUE self) {
    return X11_Window_visible(X11_Window_from(self));
}

void mg_native_window_set_pos(VALUE self, int x, int y) {
    X11_Window_set_pos(X11_Window_from(self), x, y);
}

void mg_native_window_set_size(VALUE self, unsigned int w, unsigned int h) {
    X11_Window_set_size(X11_Window_from(self), w, h);
}

void mg_native_window_set_area(VALUE self,
                               int x, int y,
                               unsigned int w, unsigned int h) {
    X11_Window_set_area(X11_Window_from(self), x, y, w, h);
}

void mg_native_window_set_visible(VALUE self, int visible) {
    X11_Window_set_visible(X11_Window_from(self), visible);
}

void mg_native_window_set_x(VALUE self, int x) {
    mg_native_window_set_pos(self, x, mg_native_window_y(self));
}

void mg_native_window_set_y(VALUE self, int y) {
    mg_native_window_set_pos(self, mg_native_window_x(self), y);
}

void mg_native_window_set_w(VALUE self, unsigned int w) {
    mg_native_window_set_size(self, w, mg_native_window_h(self));
}

void mg_native_window_set_h(VALUE self, unsigned int h) {
    mg_native_window_set_size(self, mg_native_window_w(self), h);
}

void mg_native_window_set_name(VALUE self, const char * name) {
    X11_Window_set_name(X11_Window_from(self), name);
}

void mg_native_window_set_fullscreen(VALUE self, int fullscreen) {
    X11_Window_set_fullscreen(X11_Window_from(self), fullscreen);
}

VALUE mg_native_window_start_event_thread(VALUE self) {
    /* Implementation note:
     *
     * When (void *) &self is passed as the thread function parameter and then
     * dereferenced in the thread function:
     *
     *     VALUE self = *((VALUE *) data);
     *
     * The result is a segmentation fault due to an invalid pointer. That
     * happens because, in Ruby 1.9.2, rb_thread_create stores the void pointer
     * as a VALUE. As a result, a VALUE is passed to the function, not a pointer
     * to value.
     */
    return rb_thread_create(mg_native_window_event_filter, self);
}

void mg_native_window_system_init(void) {
    if(!XInitThreads()) {
        rb_raise(rb_eRuntimeError, "could not enable X11 thread support");
    }
}

/* Helper function implementation */

static VALUE mg_native_window_event_filter(void * data) {
    /* Void pointer cast to VALUE: see note above */
    VALUE self = (VALUE) data;
    X11_Window_event_filter(self);
    return Qnil;
}

static X11_Window * X11_Window_from(VALUE obj) {
    X11_Window * w = 0;
    Data_Get_Struct(obj, X11_Window, w);
    return w;
}
