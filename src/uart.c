#include "peripherals/uart.h"
#include "peripherals/gpio.h"
#include "utils.h"
#include "uart.h"
#include "utils.h"
#include "config.h"
#include <stddef.h>
#include <stdarg.h>


void uart_init(void)
{
	unsigned int selector;

	selector = get32(GPFSEL1);
	selector &= ~(7 << 12);					// clean gpio14
	selector |= 2 << 12;					// set alt5 for gpio14
	selector &= ~(7 << 15);					// clean gpio15
	selector |= 2 << 15;					// set alt5 for gpio15
	put32(GPFSEL1, selector);

	put32(GPPUD, 0);
	delay(150);
	put32(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
	put32(GPPUDCLK0, 0);

	put32(AUX_ENABLES, 1);					// Enable mini uart (this also enables access to its registers)
	put32(AUX_MU_CNTL_REG, 0);				// Disable auto flow control and disable receiver and transmitter (for now)
	put32(AUX_MU_IER_REG, 0);				// Disable receive and transmit interrupts
	put32(AUX_MU_LCR_REG, 3);				// Enable 8 bit mode
	put32(AUX_MU_MCR_REG, 0);				// Set RTS line to be always high
	put32(AUX_MU_BAUD_REG, 270);			// Set baud rate to 115200

	put32(AUX_MU_CNTL_REG, 3);				// Finally, enable transmitter and receiver
}

void uart_send(char c)
{
	while (1) {
		if (get32(AUX_MU_LSR_REG) & 0x20) break;
	}
	put32(AUX_MU_IO_REG, c);
}

char uart_recv(void)
{
	char received_char;
	while(1) {
		if (get32(AUX_MU_LSR_REG) & 0x01) break;
	}
	received_char = get32(AUX_MU_IO_REG) & 0xFF;
	return received_char == '\r' ? '\n' : received_char;
}

static void uart_send_string(const char* str)
{
	for (int i = 0; str[i] != '\0'; i++) {
		uart_send((char)str[i]);
	}
}

static void int2str_dec(int num, char *str)
{
	char buffer[12];
	int is_negative = (num < 0);
	int index = 0;

	if (num == 0) {
		*str++ = '0';
		*str = '\0';
		return;
	}

	if (is_negative) {
		num = -num;
	}

	while (num > 0) {
		buffer[index++] = '0' + (num % 10);
		num /= 10;
	}

	if (is_negative) {
		buffer[index++] = '-';
	}

	for (int i = index - 1; i >= 0; i--) {
		*str++ = buffer[i];
	}
	*str = '\0';
}

static void uint2str_dec(unsigned int num, char *str)
{
	char buffer[11];
	int index = 0;

	if (num == 0) {
		*str++ = '0';
		*str = '\0';
		return;
	}

	while (num > 0) {
		buffer[index++] = '0' + (num % 10);
		num /= 10;
	}

	for (int i = index - 1; i >= 0; i--) {
		*str++ = buffer[i];
	}
	*str = '\0';
}

static void uint2str_hex(unsigned int num, char *str, int is_upper)
{
	const char *hex_digits_lower = "0123456789abcdef";
	const char *hex_digits_upper = "0123456789ABCDEF";
	const char *hex_digits = is_upper ? hex_digits_upper : hex_digits_lower;
	char buffer[9];
	int index = 0;

	if (num == 0) {
		*str++ = '0';
		*str = '\0';
		return;
	}

	while (num > 0) {
		buffer[index++] = hex_digits[num % 16];
		num /= 16;
	}

	for (int i = index - 1; i >= 0; i--) {
		*str++ = buffer[i];
	}

	*str = '\0';
}

static int format_string(char *buffer, int buffer_size, const char *fmt, __builtin_va_list args)
{
	char *buf_ptr = buffer;
	const char *end = buffer + buffer_size - 1;

	for (const char *p = fmt; *p != '\0'; p++) {
		if (*p != '%') {
			if (buf_ptr < end) {
				*buf_ptr++ = *p;
			}
			continue;
		}

		p++;
		switch (*p) {
			case 's': {
				char *str = __builtin_va_arg(args, char *);
				while (*str && buf_ptr < end) {
					*buf_ptr++ = *str++;
				}
				break;
			}
			case 'd': {
				int num = __builtin_va_arg(args, int);
				char temp[16];
				int2str_dec(num, temp);
				for (char *t = temp; *t && buf_ptr < end; t++) {
					*buf_ptr++ = *t;
				}
				break;
			}
			case 'u': {
				unsigned int num = __builtin_va_arg(args, unsigned int);
				char temp[16];
				uint2str_dec(num, temp);
				for (char *t = temp; *t && buf_ptr < end; t++) {
					*buf_ptr++ = *t;
				}
				break;
			}
			case 'x':
			case 'X': {
				unsigned int num = __builtin_va_arg(args, unsigned int);
				char temp[16];
				uint2str_hex(num, temp, *p == 'X');
				for (char *t = temp; *t && buf_ptr < end; t++) {
					*buf_ptr++ = *t;
				}
				break;
			}
			case 'c': {
				char c = (char)__builtin_va_arg(args, int);
				if (buf_ptr < end) {
					*buf_ptr++ = c;
				}
				break;
			}
			case '%': {
				if (buf_ptr < end) {
					*buf_ptr++ = '%';
				}
				break;
			}
			default: {
				if (buf_ptr < end) *buf_ptr++ = '%';
				if (buf_ptr < end) *buf_ptr++ = *p;
				break;
			}
		}
	}

	*buf_ptr = '\0';
	return buf_ptr - buffer;
}

void uart_send_f(const char *fmt, ...)
{
	char buffer[UART_BUFFER_SIZE];
	__builtin_va_list args;

	__builtin_va_start(args, fmt);
	format_string(buffer, sizeof(buffer), fmt, args);
	__builtin_va_end(args);

	uart_send_string(buffer);
}
