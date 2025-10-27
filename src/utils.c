#include "utils.h"
#include <stddef.h>
#include <stdarg.h>


void trim_newline(char *str)
{
	while (*str != '\0') {
		if (*str == '\n' || *str == '\r') {
			*str = '\0';
			return;
		}
		++str;
	}
}

int strcmp(const char *a, const char *b)
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
