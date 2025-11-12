#ifndef _UTILS_H
#define _UTILS_H

#include <stddef.h>

int strncmp(const char* a, const char* b, size_t size);
int strcmp(const char* a, const char* b);
int strlen(const char* str);
char* strcpy(char* dest, const char* src);

void uart_send_f(const char* fmt, ...);
int uart_getline(char *buffer, int max_size);

extern void delay(unsigned long);
extern void put32(volatile unsigned int *, unsigned int);
extern unsigned int get32(volatile unsigned int *);

#endif
