#include "utils.h"


int utils_str_compare(const char *a, const char *b)
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

void utils_int2str_dec(int num, char *str)
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

void utils_uint2str_dec(unsigned int num, char *str)
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

void utils_uint2str_hex(unsigned int num, char *str, int is_upper)
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

void utils_trim_newline(char *str)
{
	while (*str != '\0') {
		if (*str == '\n' || *str == '\r') {
			*str = '\0';
			return;
		}
		++str;
	}
}

int format_string(char *buffer, int buffer_size, const char *fmt, va_list args)
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
				char *str = va_arg(args, char *);
				while (*str && buf_ptr < end) {
					*buf_ptr++ = *str++;
				}
				break;
			}
			case 'd': {
				int num = va_arg(args, int);
				char temp[16];
				utils_int2str_dec(num, temp);
				for (char *t = temp; *t && buf_ptr < end; t++) {
					*buf_ptr++ = *t;
				}
				break;
			}
			case 'u': {
				unsigned int num = va_arg(args, unsigned int);
				char temp[16];
				utils_uint2str_dec(num, temp);
				for (char *t = temp; *t && buf_ptr < end; t++) {
					*buf_ptr++ = *t;
				}
				break;
			}
			case 'x':
			case 'X': {
				unsigned int num = va_arg(args, unsigned int);
				char temp[16];
				utils_uint2str_hex(num, temp, *p == 'X');
				for (char *t = temp; *t && buf_ptr < end; t++) {
					*buf_ptr++ = *t;
				}
				break;
			}
			case 'c': {
				char c = (char)va_arg(args, int);
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
