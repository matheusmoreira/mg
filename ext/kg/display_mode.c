#include "display_mode.h"

#if defined(MG_PLATFORM_LINUX) && defined(MG_PLATFORM_LINUX_X11)
    #include "X11_native_display_mode.h"
#endif

#include <ruby.h>

VALUE mg_display_mode_get_current_mode(VALUE klass) {
    return mg_native_display_mode_get_current_mode(klass);
}

VALUE mg_display_mode_get_modes(VALUE klass) {
    return mg_native_display_mode_get_modes(klass);
}

VALUE mg_display_mode_new(VALUE klass, int w, int h, int bpp) {
    int n = 0;
    const int argc = 3;
    VALUE argv[argc];
    argv[n++] = INT2FIX(w);
    argv[n++] = INT2FIX(h);
    argv[n++] = INT2FIX(bpp);
    return rb_class_new_instance(argc, argv, klass);
}

void init_mg_display_mode_class_under(VALUE module) {
    mg_display_mode_class = rb_define_class_under(module,
                                                  "DisplayMode",
                                                  rb_cObject);
    rb_define_singleton_method(mg_display_mode_class,
                               "current",
                               mg_display_mode_get_current_mode,
                               0);
    rb_define_singleton_method(mg_display_mode_class,
                               "all",
                               mg_display_mode_get_modes,
                               0);
}
