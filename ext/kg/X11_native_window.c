#include "X11_native_window.h"
#include "X11_Window.h"

#include <ruby.h>

X11_Window * mg_window_from(VALUE obj) {
    X11_Window * w;
    Data_Get_Struct(obj, X11_Window, w);
    return w;
}

VALUE mg_native_window_alloc(VALUE klass) {
    X11_Window * w = X11_Window_new();
    // Wrap X11_Window into Ruby VALUE
    return Data_Wrap_Struct(klass, 0, X11_Window_free, w);
}

void mg_native_window_init(VALUE self,
                           const char * name,
                           int x, int y,
                           unsigned int w, unsigned int h) {
    X11_Window * window = mg_window_from(self);
    X11_Window_init(window);
    X11_Window_create(window, x, y, w, h);
    X11_Window_set_name(window, name);
}

int mg_native_window_x(VALUE self) {
    return X11_Window_get_attributes(mg_window_from(self)).x;
}

int mg_native_window_y(VALUE self) {
    return X11_Window_get_attributes(mg_window_from(self)).y;
}

unsigned int mg_native_window_w(VALUE self) {
    return X11_Window_get_attributes(mg_window_from(self)).width;
}

unsigned int mg_native_window_h(VALUE self) {
    return X11_Window_get_attributes(mg_window_from(self)).height;
}

const char * mg_native_window_name(VALUE self) {
    return X11_Window_name(mg_window_from(self));
}

int mg_native_window_visible(VALUE self) {
    return X11_Window_visible(mg_window_from(self));
}

void mg_native_window_set_pos(VALUE self, int x, int y) {
    X11_Window_set_pos(mg_window_from(self), x, y);
}

void mg_native_window_set_size(VALUE self, unsigned int w, unsigned int h) {
    X11_Window_set_size(mg_window_from(self), w, h);
}

void mg_native_window_set_area(VALUE self,
                               int x, int y,
                               unsigned int w, unsigned int h) {
    X11_Window_set_area(mg_window_from(self), x, y, w, h);
}

void mg_native_window_set_visible(VALUE self, int visible) {
    X11_Window_set_visible(mg_window_from(self), visible);
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
    X11_Window_set_name(mg_window_from(self), name);
}

void mg_native_window_set_fullscreen(VALUE self, int fullscreen) {
    X11_Window_set_fullscreen(mg_window_from(self), fullscreen);
}
