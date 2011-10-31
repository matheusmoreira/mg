#include "window.h"

#if defined(MG_PLATFORM_LINUX) && defined(MG_PLATFORM_LINUX_X11)
    #include "X11_native_window.h"
#endif

#include "event.h"

#include <ruby.h>

VALUE mg_window_alloc(VALUE klass) {
    return mg_native_window_alloc(klass);
}

VALUE mg_window_initialize(VALUE self, VALUE name, VALUE x, VALUE y, VALUE w, VALUE h) {
    Check_Type(name, T_STRING);
    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);
    mg_native_window_init(self, StringValueCStr(name),
                          FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));
    return Qnil;
}

VALUE mg_window_x(VALUE self) {
    return INT2FIX(mg_native_window_x(self));
}

VALUE mg_window_y(VALUE self) {
    return INT2FIX(mg_native_window_y(self));
}

VALUE mg_window_w(VALUE self) {
    return INT2FIX(mg_native_window_w(self));
}

VALUE mg_window_h(VALUE self) {
    return INT2FIX(mg_native_window_h(self));
}

VALUE mg_window_title(VALUE self) {
    return rb_str_new_cstr(mg_native_window_title(self));
}

VALUE mg_window_visible(VALUE self) {
    return mg_native_window_visible(self) ? Qtrue : Qfalse;
}

VALUE mg_window_set_x(VALUE self, VALUE x) {
    Check_Type(x, T_FIXNUM);
    mg_native_window_set_x(self, FIX2INT(x));
    return Qnil;
}

VALUE mg_window_set_y(VALUE self, VALUE y) {
    Check_Type(y, T_FIXNUM);
    mg_native_window_set_y(self, FIX2INT(y));
    return Qnil;
}

VALUE mg_window_set_w(VALUE self, VALUE w) {
    Check_Type(w, T_FIXNUM);
    mg_native_window_set_w(self, FIX2INT(w));
    return Qnil;
}

VALUE mg_window_set_h(VALUE self, VALUE h) {
    Check_Type(h, T_FIXNUM);
    mg_native_window_set_h(self, FIX2INT(h));
    return Qnil;
}

VALUE mg_window_set_pos(VALUE self, VALUE x, VALUE y) {
    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    mg_native_window_set_pos(self, FIX2INT(x), FIX2INT(y));
    return Qnil;
}

VALUE mg_window_set_size(VALUE self, VALUE w, VALUE h) {
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);
    mg_native_window_set_size(self, FIX2INT(w), FIX2INT(h));
    return Qnil;
}

VALUE mg_window_set_area(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h) {
    Check_Type(x, T_FIXNUM);
    Check_Type(y, T_FIXNUM);
    Check_Type(w, T_FIXNUM);
    Check_Type(h, T_FIXNUM);
    mg_native_window_set_area(self, FIX2INT(x), FIX2INT(y), FIX2INT(w), FIX2INT(h));
    return Qnil;
}

VALUE mg_window_set_visible(VALUE self, VALUE visible) {
    mg_native_window_set_visible(self, RTEST(visible));
    return Qnil;
}

VALUE mg_window_set_name(VALUE self, VALUE name) {
    mg_native_window_set_name(self, StringValueCStr(name));
}

VALUE mg_window_set_fullscreen(VALUE self, VALUE fs) {
    mg_native_window_set_fullscreen(self, RTEST(fs));
}

VALUE mg_window_handle_events(VALUE self) {
    mg_native_window_event_filter(self);
    return Qnil;
}

void def_mg_window_method(const char * name, VALUE (*func)(), int argc) {
    rb_define_method(mg_window_class, name, func, argc);
}

void def_mg_window_alias(const char * alias, const char * old) {
    rb_define_alias(mg_window_class, alias, old);
}

void init_mg_window_class_under(VALUE module) {
    /* Initialize the native windowing system */
    mg_native_window_system_init();

    /* Define Mg::Window class */
    mg_window_class = rb_define_class_under(module, "Window", rb_cObject);

    /* Give it an allocation function */
    rb_define_alloc_func(mg_window_class, mg_window_alloc);

    /* Define the instance methods */
    def_mg_window_method("initialize",     mg_window_initialize,     5);
    def_mg_window_method("x",              mg_window_x,              0);
    def_mg_window_method("y",              mg_window_y,              0);
    def_mg_window_method("width",          mg_window_w,              0);
    def_mg_window_method("height",         mg_window_h,              0);
    def_mg_window_method("title",          mg_window_title,          0);
    def_mg_window_method("visible?",       mg_window_visible,        0);
    def_mg_window_method("x=",             mg_window_set_x,          1);
    def_mg_window_method("y=",             mg_window_set_y,          1);
    def_mg_window_method("width=",         mg_window_set_w,          1);
    def_mg_window_method("height=",        mg_window_set_h,          1);
    def_mg_window_method("name=",          mg_window_set_name,       1);
    def_mg_window_method("visible=",       mg_window_set_visible,    1);
    def_mg_window_method("fullscreen=",    mg_window_set_fullscreen, 1);
    def_mg_window_method("handle_events!", mg_window_handle_events,  0);
    def_mg_window_alias("name", "title");
    def_mg_window_alias("w",    "width");
    def_mg_window_alias("h",    "height");
    def_mg_window_alias("w=",   "width=");
    def_mg_window_alias("h=",   "height=");
}
