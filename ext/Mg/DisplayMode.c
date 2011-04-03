#include <ruby.h>

#include <Mg/DisplayMode.h>

#if defined(MG_PLATFORM_LINUX) && defined(MG_PLATFORM_LINUX_X11)
    #include "x11/NativeDisplayMode.c"
#endif

static VALUE Mg_DisplayMode_Class = Qnil;

static VALUE display_mode_get_current_mode(VALUE klass) {
    return native_display_mode_get_current_mode(klass);
}

static VALUE display_mode_get_modes(VALUE klass) {
    return native_display_mode_get_modes(klass);
}

static void init_display_mode_class_under(VALUE module) {
    Mg_DisplayMode_Class = rb_define_class_under(module, "DisplayMode", rb_cObject);
    rb_define_singleton_method(Mg_DisplayMode_Class,
                               "current",
                               display_mode_get_current_mode,
                               0);
    rb_define_singleton_method(Mg_DisplayMode_Class,
                               "all",
                               display_mode_get_modes,
                               0);
}
