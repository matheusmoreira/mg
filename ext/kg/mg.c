#include "mg.h"

#include "window.h"
#include "display_mode.h"

#include <ruby.h>

void init_mg_module(void) {
    mg_module = rb_define_module("Mg");
    init_mg_window_class_under(mg_module);
    init_mg_window_events();
    init_mg_display_mode_class_under(mg_module);
}
