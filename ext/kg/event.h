#ifndef MG_EVENT_H
#define MG_EVENT_H

#include <ruby.h>

/* Keyboard key symbols */

VALUE mg_event_keyboard_key_a_symbol;
VALUE mg_event_keyboard_key_b_symbol;
VALUE mg_event_keyboard_key_c_symbol;
VALUE mg_event_keyboard_key_d_symbol;
VALUE mg_event_keyboard_key_e_symbol;
VALUE mg_event_keyboard_key_f_symbol;
VALUE mg_event_keyboard_key_g_symbol;
VALUE mg_event_keyboard_key_h_symbol;
VALUE mg_event_keyboard_key_i_symbol;
VALUE mg_event_keyboard_key_j_symbol;
VALUE mg_event_keyboard_key_k_symbol;
VALUE mg_event_keyboard_key_l_symbol;
VALUE mg_event_keyboard_key_m_symbol;
VALUE mg_event_keyboard_key_n_symbol;
VALUE mg_event_keyboard_key_o_symbol;
VALUE mg_event_keyboard_key_p_symbol;
VALUE mg_event_keyboard_key_q_symbol;
VALUE mg_event_keyboard_key_r_symbol;
VALUE mg_event_keyboard_key_s_symbol;
VALUE mg_event_keyboard_key_t_symbol;
VALUE mg_event_keyboard_key_u_symbol;
VALUE mg_event_keyboard_key_v_symbol;
VALUE mg_event_keyboard_key_w_symbol;
VALUE mg_event_keyboard_key_x_symbol;
VALUE mg_event_keyboard_key_y_symbol;
VALUE mg_event_keyboard_key_z_symbol;
VALUE mg_event_keyboard_key_unsupported_symbol;

/* Event interface */

/**
 * Calls the window's close event handler with the given arguments.
 */
extern VALUE mg_event_call_close_handler(VALUE window);

/**
 * Calls the window's key press event handler with the given arguments.
 */
extern VALUE mg_event_call_key_press_handler(VALUE window, VALUE args);

/**
 * Calls the window's key release event handler with the given arguments.
 */
extern VALUE mg_event_call_key_release_handler(VALUE window, VALUE args);

/**
 * Initializes Window event functionality.
 */
extern void init_mg_window_events();

#endif /* MG_EVENT_H */
