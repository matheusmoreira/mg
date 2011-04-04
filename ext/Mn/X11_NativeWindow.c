#include "X11_NativeWindow.h"
#include "X11_Window.h"

#include <ruby.h>

X11_Window * window_from(VALUE obj) {
    X11_Window * w;
    Data_Get_Struct(obj, X11_Window, w);
    return w;
}

VALUE native_window_alloc(VALUE klass) {
    X11_Window * w = X11_Window_new();
    X11_Window_init(w);
    // Wrap X11_Window into Ruby VALUE
    return Data_Wrap_Struct(klass, 0, X11_Window_free, w);
}

void native_window_init(VALUE self,
                               const char * name,
                               int x, int y,
                               unsigned int w, unsigned int h) {
    X11_Window * window = window_from(self);
    X11_Window_create(window, x, y, w, h);
    X11_Window_set_name(window, name);
}

int native_window_x(VALUE self) {
    return X11_Window_get_attributes(window_from(self)).x;
}

int native_window_y(VALUE self) {
    return X11_Window_get_attributes(window_from(self)).y;
}

unsigned int native_window_w(VALUE self) {
    return X11_Window_get_attributes(window_from(self)).width;
}

unsigned int native_window_h(VALUE self) {

    return X11_Window_get_attributes(window_from(self)).height;
}

const char * native_window_name(VALUE self) {
    return X11_Window_name(window_from(self));
}

int native_window_visible(VALUE self) {
    return X11_Window_visible(window_from(self));
}

void native_window_set_pos(VALUE self, int x, int y) {
    X11_Window_set_pos(window_from(self), x, y);
}

void native_window_set_size(VALUE self, unsigned int w, unsigned int h) {
    X11_Window_set_size(window_from(self), w, h);
}

void native_window_set_area(VALUE self, int x, int y, unsigned int w, unsigned int h) {
    X11_Window_set_area(window_from(self), x, y, w, h);
}

void native_window_set_visible(VALUE self, int visible) {
    X11_Window_set_visible(window_from(self), visible);
}

void native_window_set_x(VALUE self, int x) {
    native_window_set_pos(self, x, native_window_y(self));
}

void native_window_set_y(VALUE self, int y) {
    native_window_set_pos(self, native_window_x(self), y);
}

void native_window_set_w(VALUE self, unsigned int w) {
    native_window_set_size(self, w, native_window_h(self));
}

void native_window_set_h(VALUE self, unsigned int h) {
    native_window_set_size(self, native_window_w(self), h);
}

void native_window_set_name(VALUE self, const char * name) {
    X11_Window_set_name(window_from(self), name);
}

void native_window_set_fullscreen(VALUE self, int fs) {
    X11_Window_set_fullscreen(window_from(self), fs);
}
