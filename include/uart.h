#ifndef _UART_H
#define _UART_H

void uart_init();
void uart_send(char c);
char uart_recv();
void uart_flush_rx();
void uart_send_f(const char* fmt, ...);

#endif
