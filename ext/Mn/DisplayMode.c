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

void init_display_mode_class_under(VALUE module) {
    Mg_Display_Mode_Class = rb_define_class_under(module, "DisplayMode", rb_cObject);
    rb_define_singleton_method(Mg_Display_Mode_Class,
                               "current",
                               display_mode_get_current_mode,
                               0);
    rb_define_singleton_method(Mg_Display_Mode_Class,
                               "all",
                               display_mode_get_modes,
                               0);
}
