#include "event.h"

#include <pthread.h>

#include <ruby.h>

/* Mg event queue */

/**
 * Mg event list.
 */
static mg_event_t * mg_event_list;

/**
 * Mutex for the event list.
 */
static pthread_mutex_t mg_event_list_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Condition variable for the event list.
 */
static pthread_cond_t mg_event_list_cond = PTHREAD_COND_INITIALIZER;

/* Ruby event thread */

/**
 * The Ruby Event Thread.
 */
static VALUE mg_event_ruby_event_thread;

/**
 * Starts a Ruby thread that polls the event list continuously and calls the
 * event's window's callbacks. 
 */
static void mg_event_start_ruby_event_thread(void);

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

/* Event handler method IDs */

static ID mg_event_window_close_handler;
static ID mg_event_keyboard_key_press_handler;
static ID mg_event_keyboard_key_release_handler;
static ID call;

/* Event interface implementation */

VALUE mg_event_call_close_handler(VALUE window, VALUE args) {
    return mg_event_call_handler(window,
                                 mg_event_window_close_handler,
                                 args);
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

/* Ruby Event Thread */

static void mg_event_handle_event(mg_event_t * event) {
}

typedef struct event_poll_struct {
    mg_event_t * event;
    int waiting;
} event_poll_t;

static void wait_for_events(void * p) {
    event_poll_t * event_poll = (event_poll_t *) p;

    /* Lock the the event list in order to ensure exclusive access */
    pthread_mutex_lock(&mg_event_list_mutex);

    /* While we are waiting AND ... */
    while (event_poll->waiting &&
           /* ... the global event list is empty */
           (event_poll->event = mg_event_pop()) == 0) {
        /* Wait for a wakeup signal */
        pthread_cond_wait(&mg_event_list_cond);
    }

    /* Unlock the event list */
    pthread_mutex_unlock(&mg_event_list_mutex);
}

static void stop_waiting_for_events(void * p) {
    event_poll_t * event_poll = (event_poll_t *) p;

    /* Lock the the event list in order to ensure exclusive access */
    pthread_mutex_lock(&mg_event_list_mutex);

    /* Stop waiting for events */
    event_poll->waiting = 0;

    /* Wake up waiting thread */
    pthread_cond_signal(&mg_event_list_cond);

    /* Unlock the event list */
    pthread_mutex_unlocklock(&mg_event_list_mutex);
}

static void mg_event_poll_events(void * unused) {
    event_poll_t event_poll = { .event = 0, .waiting = 1 };

    while (event_poll.waiting) {
        /* Release the GVL and wait for events to handle */
        rb_thread_blocking_region(wait_for_events,         &event_poll,
                                  stop_waiting_for_events, &event_poll);

        /* If we received an event... */
        if (event_poll.event) {
            /* ... handle it in a new thread */
            rb_thread_create(mg_event_handle_event, event_poll.event);
        }
    }
}

static void mg_event_start_event_thread() {
    mg_event_ruby_event_thread = rb_thread_create(mg_event_poll_events, 0);
    rb_global_variable(mg_event_ruby_event_thread);
}

/* Mg event queue */

void mg_event_push(mg_event_t * event) {
    event->next = mg_event_list;
    mg_event_list = event;
}

mg_event_t * mg_event_pop(void) {
    mg_event_t * event = mg_event_list;
    if (event) {
        mg_event_list = event->next;
    }
    return event;
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
