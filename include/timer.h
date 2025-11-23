#ifndef _TIMER_H
#define _TIMER_H

#include "exception.h"

void timer_init(void);
void timer_clear(void);
void timer_enable(void);
void timer_disable(void);
void timer_handler(trap_frame_t* regs);

#endif
