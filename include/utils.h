#ifndef _UTILS_H
#define _UTILS_H

#include <stddef.h>
#include <stdint.h>

#define round_4(number) ((number + 3) / 4 * 4)
#define round_8(number) ((number + 7) / 8 * 8)

#define SEND_BUFFER_SIZE 256u
#define RING_BUFFER_SIZE 256

int strncmp(const char* a, const char* b, size_t size);
int strcmp(const char* a, const char* b);
int strlen(const char* str);
char* strcpy(char* dest, const char* src);
int atoi(const char* str);

void* memcpy(void* dest, const void* src, size_t n);

void uart_send_f(const char* fmt, ...);
int uart_getline(char *buffer, int max_size);

uint32_t b2l_32(uint32_t num);
uint64_t b2l_64(uint64_t num);

typedef struct {
    char buffer[RING_BUFFER_SIZE];
    int head;
    int tail;
} ringbuffer_t;

void rb_init(ringbuffer_t* rb);
int rb_push(ringbuffer_t* rb, char c);
int rb_pop(ringbuffer_t* rb, char* c);
int rb_full(ringbuffer_t* rb);
int rb_empty(ringbuffer_t* rb);

extern void delay(unsigned long);
extern void put32(volatile unsigned int *, unsigned int);
extern unsigned int get32(volatile unsigned int *);
extern void enable_cpu_irq(void);
extern void disable_cpu_irq(void);

#endif
