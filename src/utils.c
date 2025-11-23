#include "uart.h"
#include "utils.h"
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
	char buffer[SEND_BUFFER_SIZE];
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

void* memcpy(void* dest, const void* src, size_t n) {
	for (size_t i = 0; i < n; i++) {
		((char*)dest)[i] = ((const char*)src)[i];
	}
	return dest;
}

uint32_t b2l_32(uint32_t num) {
    uint32_t res = 0;
    res = ((num >> 24) & 0x000000ff) |
          ((num >> 8)  & 0x0000ff00) |
          ((num << 8)  & 0x00ff0000) |
          ((num << 24) & 0xff000000);
    return res;
}

uint64_t b2l_64(uint64_t num) {
    uint64_t res = 0;
    res = ((num >> 56) & 0x00000000000000ffULL) |
          ((num >> 40) & 0x000000000000ff00ULL) |
          ((num >> 24) & 0x0000000000ff0000ULL) |
          ((num >> 8)  & 0x00000000ff000000ULL) |
          ((num << 8)  & 0x000000ff00000000ULL) |
          ((num << 24) & 0x0000ff0000000000ULL) |
          ((num << 40) & 0x00ff000000000000ULL) |
          ((num << 56) & 0xff00000000000000ULL);
    return res;
}

void rb_init(ringbuffer_t* rb) {
	rb->head = 0;
	rb->tail = 0;
}

int rb_push(ringbuffer_t* rb, char c) {
	if (rb_full(rb)) return -1;
	rb->buffer[rb->head] = c;
	rb->head = (rb->head + 1) & (RING_BUFFER_SIZE - 1);
	return 0;
}

int rb_pop(ringbuffer_t* rb, char* c) {
	if (rb_empty(rb)) return -1;
	*c = rb->buffer[rb->tail];
	rb->tail = (rb->tail + 1) & (RING_BUFFER_SIZE - 1);
	return 0;
}

int rb_full(ringbuffer_t* rb) {
	return ((rb->head + 1) & (RING_BUFFER_SIZE - 1)) == (rb->tail);
}

int rb_empty(ringbuffer_t* rb) {
	return rb->head == rb->tail;
}
