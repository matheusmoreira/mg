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

VALUE mg_display_mode_new(VALUE w, VALUE h, VALUE bpp) {
    Check_Type(w,   T_FIXNUM);
    Check_Type(h,   T_FIXNUM);
    Check_Type(bpp, T_FIXNUM);
    static const int argc = 3;
    VALUE argv[] = { w, h, bpp };
    return rb_class_new_instance(argc, argv, mg_display_mode_class);
}

VALUE mg_display_mode_new_c(int w, int h, int bpp) {
    return mg_display_mode_new(INT2FIX(w), INT2FIX(h), INT2FIX(bpp));
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
