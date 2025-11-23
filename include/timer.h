#ifndef _TIMER_H
#define _TIMER_H

#include "exception.h"

typedef void (*timer_cb_t)(void*);
typedef struct timer_node {
    unsigned long expire;
    timer_cb_t callback;
    void* arg;
    struct timer_node* next;
} timer_node_t;

void timer_init(void);
void add_timer(timer_cb_t callback, void* arg, unsigned int expire);
void timer_handler(trap_frame_t* regs);

#endif
