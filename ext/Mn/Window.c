#include "Window.h"

#if defined(MG_PLATFORM_LINUX) && defined(MG_PLATFORM_LINUX_X11)
    #include "X11_NativeWindow.h"
#endif

#include <ruby.h>

VALUE window_alloc(VALUE klass) {
    return native_window_alloc(klass);
}

VALUE window_initialize(VALUE self, VALUE name, VALUE x, VALUE y, VALUE w, VALUE h) {
    Check_Type(name, T_STRING);
    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);
    native_window_init(self, StringValueCStr(name), FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));
    return Qnil;
}

VALUE window_x(VALUE self) {
    return INT2FIX(native_window_x(self));
}

VALUE window_y(VALUE self) {
    return INT2FIX(native_window_y(self));
}

VALUE window_w(VALUE self) {
    return INT2FIX(native_window_w(self));
}

VALUE window_h(VALUE self) {
    return INT2FIX(native_window_h(self));
}

VALUE window_name(VALUE self) {
    return rb_str_new_cstr(native_window_name(self));
}

VALUE window_visible(VALUE self) {
    if (native_window_visible(self)) {
        return Qtrue;
    } else {
        return Qfalse;
    }
}

VALUE window_set_x(VALUE self, VALUE x) {
    Check_Type(x, T_FIXNUM);
    native_window_set_x(self, FIX2INT(x));
    return Qnil;
}

VALUE window_set_y(VALUE self, VALUE y) {
    Check_Type(y, T_FIXNUM);
    native_window_set_y(self, FIX2INT(y));
    return Qnil;
}

VALUE window_set_w(VALUE self, VALUE w) {
    Check_Type(w, T_FIXNUM);
    native_window_set_w(self, FIX2INT(w));
    return Qnil;
}

VALUE window_set_h(VALUE self, VALUE h) {
    Check_Type(h, T_FIXNUM);
    native_window_set_h(self, FIX2INT(h));
    return Qnil;
}

VALUE window_set_pos(VALUE self, VALUE x, VALUE y) {
    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    native_window_set_pos(self, FIX2INT(x), FIX2INT(y));
    return Qnil;
}

VALUE window_set_size(VALUE self, VALUE w, VALUE h) {
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);
    native_window_set_size(self, FIX2INT(w), FIX2INT(h));
    return Qnil;
}

VALUE window_set_area(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h) {
    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);
    native_window_set_area(self, FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));
    return Qnil;
}

VALUE window_set_visible(VALUE self, VALUE visible) {
    native_window_set_visible(self, RTEST(visible));
    return Qnil;
}

VALUE window_set_name(VALUE self, VALUE name) {
    native_window_set_name(self, StringValueCStr(name));
}

VALUE window_set_fullscreen(VALUE self, VALUE fs) {
    native_window_set_fullscreen(self, RTEST(fs));
}

void def_window_method(const char * name, VALUE (*func)(), int argc) {
    rb_define_method(Mg_Window_Class, name, func, argc);
}

void def_alias(const char * alias, const char * old) {
    rb_define_alias(Mg_Window_Class, alias, old);
}

void init_window_class_under(VALUE module) {
    Mg_Window_Class = rb_define_class_under(module, "Window", rb_cObject);
    rb_define_alloc_func(Mg_Window_Class, window_alloc);
    def_window_method("initialize", window_initialize, 5);
    def_window_method("x", window_x, 0);
    def_window_method("y", window_y, 0);
    def_window_method("w", window_w, 0);
    def_window_method("h", window_h, 0);
    def_window_method("name", window_name, 0);
    def_window_method("visible?", window_visible, 0);
    def_window_method("x=", window_set_x, 1);
    def_window_method("y=", window_set_y, 1);
    def_window_method("w=", window_set_w, 1);
    def_window_method("h=", window_set_h, 1);
    def_window_method("name=", window_set_name, 1);
    def_window_method("visible=", window_set_visible, 1);
    def_window_method("fullscreen=", window_set_fullscreen, 1);
    def_alias("width",  "w");
    def_alias("height", "h");
    def_alias("width=",  "w=");
    def_alias("height=", "h=");
}
