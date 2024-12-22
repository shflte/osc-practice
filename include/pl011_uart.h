#ifndef _PL011_UART_H
#define _PL011_UART_H

void pl011_uart_init();
void pl011_uart_send(char c);
char pl011_uart_recv();

#endif
