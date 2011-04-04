#include "Mg.h"

#include "Window.h"
#include "DisplayMode.h"

#include <ruby.h>

void init_Mg_module(void) {
    Mg_Module = rb_define_module("Mg");
    init_window_class_under(Mg_Module);
    init_display_mode_class_under(Mg_Module);
}
