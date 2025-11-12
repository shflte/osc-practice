#include "uart.h"
#include "config.h"
#include <stddef.h>
#include <stdarg.h>

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

int uart_getline(char *buffer, int max_size)
{
    int index = 0;
    char input;

    while (1) {
        input = uart_recv();

        if (input == 0x08 || input == 0x7F) {
            if (index > 0) {
                index--;

                uart_send('\b');
                uart_send(' ');
                uart_send('\b');
            }
            continue;
        }

        input = input == '\r' ? '\n' : input;

		uart_send(input);

        if (index < max_size - 1) buffer[index++] = input;

        if (input == '\n') break;
    }

    buffer[index] = '\0';

    return index;
}

int strncmp(const char* a, const char* b, size_t size)
{
	for (unsigned int i = 0; i < size; i++) {
		if (*a != *b) {
			return *a - *b;
		}
		a++;
		b++;
	}
	return 0;
}

int strcmp(const char* a, const char* b)
{
	while (*a != '\0' && *b != '\0') {
		if (*a != *b) {
			return *a - *b;
		}
		a++;
		b++;
	}
	return *a - *b;
}

int strlen(const char* str) {
	int len = 0;
	while (str[len] != '\0') {
		len++;
	}
	return len;
}

char* strcpy(char* dest, const char* src) {
	int i;
	for (i = 0; src[i] != '\0'; i++) {
		dest[i] = src[i];
	}
	dest[i] = '\0';

	return dest;
}
