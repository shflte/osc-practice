#ifndef _UTILS_H
#define _UTILS_H

#include <stddef.h>
#include <stdint.h>

#define round_4(number) ((number + 3) / 4 * 4)
#define round_8(number) ((number + 7) / 8 * 8)

#define UART_BUFFER_SIZE 256u

int strncmp(const char* a, const char* b, size_t size);
int strcmp(const char* a, const char* b);
int strlen(const char* str);
char* strcpy(char* dest, const char* src);

void uart_send_f(const char* fmt, ...);
int uart_getline(char *buffer, int max_size);

uint32_t b2l_32(uint32_t num);
uint64_t b2l_64(uint64_t num);

extern void delay(unsigned long);
extern void put32(volatile unsigned int *, unsigned int);
extern unsigned int get32(volatile unsigned int *);

#endif
