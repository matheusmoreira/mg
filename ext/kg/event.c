#include "event.h"

#include <ruby.h>

#include <stdio.h>

/* Event handler method IDs */

static ID mg_event_window_close_handler;
static ID mg_event_keyboard_key_press_handler;
static ID mg_event_keyboard_key_release_handler;
static ID call;

/* Helper function prototypes */

/**
 * Initializes a symbol constant.
 */
static void mg_event_init_sym(VALUE *, const char *);

/**
 * Retrieves the Proc for the given event handler from the given Window and
 * calls it with the arguments, which should be in a Ruby array.
 */
static VALUE mg_event_call_handler(VALUE window, ID handler, VALUE args);

/* Event interface implementation */

VALUE mg_event_call_close_handler(VALUE window) {
    return mg_event_call_handler(window,
                                 mg_event_window_close_handler,
                                 Qnil);
}

VALUE mg_event_call_key_press_handler(VALUE window, VALUE args) {
    return mg_event_call_handler(window,
                                 mg_event_keyboard_key_press_handler,
                                 args);
}

VALUE mg_event_call_key_release_handler(VALUE window, VALUE args) {
    return mg_event_call_handler(window,
                                 mg_event_keyboard_key_release_handler,
                                 args);
}

void init_mg_window_events() {
    /* Define event handler instance variable and method IDs */
    mg_event_window_close_handler = rb_intern("@close_handler");
    mg_event_keyboard_key_press_handler = rb_intern("@key_press_handler");
    mg_event_keyboard_key_release_handler = rb_intern("@key_release_handler");
    call = rb_intern("call");

    /* Initialize keyboard key symbols */
    mg_event_init_sym(&mg_event_keyboard_key_a_symbol, "a");
    mg_event_init_sym(&mg_event_keyboard_key_b_symbol, "b");
    mg_event_init_sym(&mg_event_keyboard_key_c_symbol, "c");
    mg_event_init_sym(&mg_event_keyboard_key_d_symbol, "d");
    mg_event_init_sym(&mg_event_keyboard_key_e_symbol, "e");
    mg_event_init_sym(&mg_event_keyboard_key_f_symbol, "f");
    mg_event_init_sym(&mg_event_keyboard_key_g_symbol, "g");
    mg_event_init_sym(&mg_event_keyboard_key_h_symbol, "h");
    mg_event_init_sym(&mg_event_keyboard_key_i_symbol, "i");
    mg_event_init_sym(&mg_event_keyboard_key_j_symbol, "j");
    mg_event_init_sym(&mg_event_keyboard_key_k_symbol, "k");
    mg_event_init_sym(&mg_event_keyboard_key_l_symbol, "l");
    mg_event_init_sym(&mg_event_keyboard_key_m_symbol, "m");
    mg_event_init_sym(&mg_event_keyboard_key_n_symbol, "n");
    mg_event_init_sym(&mg_event_keyboard_key_o_symbol, "o");
    mg_event_init_sym(&mg_event_keyboard_key_p_symbol, "p");
    mg_event_init_sym(&mg_event_keyboard_key_q_symbol, "q");
    mg_event_init_sym(&mg_event_keyboard_key_r_symbol, "r");
    mg_event_init_sym(&mg_event_keyboard_key_s_symbol, "s");
    mg_event_init_sym(&mg_event_keyboard_key_t_symbol, "t");
    mg_event_init_sym(&mg_event_keyboard_key_u_symbol, "u");
    mg_event_init_sym(&mg_event_keyboard_key_v_symbol, "v");
    mg_event_init_sym(&mg_event_keyboard_key_w_symbol, "w");
    mg_event_init_sym(&mg_event_keyboard_key_x_symbol, "x");
    mg_event_init_sym(&mg_event_keyboard_key_y_symbol, "y");
    mg_event_init_sym(&mg_event_keyboard_key_z_symbol, "z");

    /* Special symbols */
    mg_event_init_sym(&mg_event_keyboard_key_unsupported_symbol, "unsupported");
}

/* Helper function implementation */

static void mg_event_init_sym(VALUE * constant, const char * symbol) {
    /* Create the Ruby symbol */
    *constant = ID2SYM(rb_intern(symbol));

    /* Let the Ruby garbage collector know about the constant */
    rb_global_variable(constant);
}

static VALUE mg_event_call_handler(VALUE window, ID handler, VALUE args) {
    VALUE proc;
    proc = rb_ivar_get(window, handler);
    if (!NIL_P(proc)) {
        switch (TYPE(args)) {
            case T_NIL:
                return rb_funcall(proc, call, 0);
            case T_ARRAY:
                return rb_funcall2(proc, call,
                                   RARRAY_LEN(args), RARRAY_PTR(args));
                break;
            default:
                return rb_funcall(proc, call, 1, args);
        }
    }
    return Qnil;
}
