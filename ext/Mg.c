#include <ruby.h>

#include <Mg.h>

#include <Mg/Window.h>
#include <Mg/DisplayMode.h>

void Init_Mg(void) {
    Mg_Module = rb_define_module("Mg");
    init_window_class_under(Mg_Module);
    init_display_mode_class_under(Mg_Module);
}
