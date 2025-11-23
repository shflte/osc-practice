#ifndef _UART_H
#define _UART_H

#include "exception.h"

void uart_init();
void uart_send(char c);
char uart_recv();
void uart_handler(trap_frame_t* regs);

#endif
