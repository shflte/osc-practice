#ifndef _UTILS_H
#define _UTILS_H

void trim_newline(char* str);
int strcmp(const char* a, const char* b);

extern void delay(unsigned long);
extern void put32(volatile unsigned int *, unsigned int);
extern unsigned int get32(volatile unsigned int *);

#endif
