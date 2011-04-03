#include <ruby.h>

#include "Mg/Window.c"

static VALUE Mg_Module = Qnil;

void Init_Mg() {
    Mg_Module = rb_define_module("Mg");
    init_window_module_under(Mg_Module);
}
