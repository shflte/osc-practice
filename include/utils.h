#ifndef _UTILS_H
#define _UTILS_H
#include <stddef.h>
#include <stdarg.h>

int utils_str_compare(const char* a, const char* b);
void utils_int2str_dec(int a, char* str);
void utils_uint2str_dec(unsigned int num, char* str);
void utils_uint2str_hex(unsigned int num, char* str, int is_upper);
void utils_trim_newline(char* str);
int format_string(char *buffer, int buffer_size, const char *fmt, va_list args);

extern void delay(unsigned long);
extern void put32(volatile unsigned int *, unsigned int);
extern unsigned int get32(volatile unsigned int *);

#endif
