#ifndef MG_EVENT_H
#define MG_EVENT_H

#include <pthread.h>

#include <ruby.h>

/* Mg Event Queue */

/**
 * Represents an event that is to be handled by Ruby.
 */
typedef struct mg_event_struct {
    VALUE window; /** Window where the event happened; */
    VALUE args; /** Ruby array of arguments to pass to the handler. */
    int handled; /** Whether this event has been handled. */
    pthread_mutex_t mutex; /** Event mutex. */
    pthread_cond_t cond; /** Event condition variable. */
    mg_event_struct * next; /** Pointer to the next event. */
} mg_event_t;

/**
 * Add event to the end of the global event list.
 */
extern void mg_event_push(mg_event_t * event);

/**
 * Removes the first event from the global event list and returns it.
 */
extern mg_event_t * mg_event_pop(void);

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
extern VALUE mg_event_call_close_handler(VALUE window, VALUE args);

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
