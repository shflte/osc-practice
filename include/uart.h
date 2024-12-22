#ifndef _UART_H
#define _UART_H

#include "config.h"

#ifdef USE_MINI_UART
#include "mini_uart.h"
#define uart_init		mini_uart_init
#define uart_send		mini_uart_send
#define uart_recv		mini_uart_recv
#endif

#ifdef USE_PL011_UART
#include "pl011_uart.h"
#define uart_init		pl011_uart_init
#define uart_send		pl011_uart_send
#define uart_recv		pl011_uart_recv
#endif

void uart_send_string(const char* str);
void uart_send_f(const char* fmt, ...);

#endif
