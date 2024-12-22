#include "uart.h"
#include "utils.h"
#include "config.h"


void uart_send_string(const char* str)
{
	for (int i = 0; str[i] != '\0'; i++) {
		uart_send((char)str[i]);
	}
}

void uart_send_f(const char *fmt, ...)
{
	char buffer[UART_BUFFER_SIZE];
	va_list args;

	va_start(args, fmt);
	format_string(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	for (char *p = buffer; *p != '\0'; p++) {
		uart_send(*p);
	}
}
