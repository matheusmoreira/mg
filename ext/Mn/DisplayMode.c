#include "DisplayMode.h"

#if defined(MG_PLATFORM_LINUX) && defined(MG_PLATFORM_LINUX_X11)
    #include "X11_NativeDisplayMode.h"
#endif

#include <ruby.h>

VALUE display_mode_get_current_mode(VALUE klass) {
    return native_display_mode_get_current_mode(klass);
}

VALUE display_mode_get_modes(VALUE klass) {
    return native_display_mode_get_modes(klass);
}

VALUE display_mode_new(VALUE klass, int w, int h, int bpp) {
    int n = 0;
    const int argc = 3;
    VALUE argv[argc];
    argv[n++] = INT2FIX(w);
    argv[n++] = INT2FIX(h);
    argv[n++] = INT2FIX(bpp);
    return rb_class_new_instance(argc, argv, klass);
}

void init_display_mode_class_under(VALUE module) {
    Mg_Display_Mode_Class = rb_define_class_under(module,
                                                  "DisplayMode",
                                                  rb_cObject);
    rb_define_singleton_method(Mg_Display_Mode_Class,
                               "current",
                               display_mode_get_current_mode,
                               0);
    rb_define_singleton_method(Mg_Display_Mode_Class,
                               "all",
                               display_mode_get_modes,
                               0);
}
