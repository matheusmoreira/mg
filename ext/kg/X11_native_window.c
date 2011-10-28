#include "X11_native_window.h"

#include "X11_Window.h"

#include <ruby.h>

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

void mg_native_window_start_event_loop_thread(VALUE self) {
    X11_Window_start_event_loop_thread_for(self);
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

const char * mg_native_window_name(VALUE self) {
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

void mg_native_window_event_filter(VALUE self) {
    X11_Window_event_filter(self);
}

void mg_native_window_system_init(void) {
    if(!XInitThreads()) {
        rb_raise(rb_eRuntimeError, "could not enable X11 thread support");
    }
}

X11_Window * X11_Window_from(VALUE window) {
    X11_Window * w = 0;
    Data_Get_Struct(window, X11_Window, w);
    return w;
}
