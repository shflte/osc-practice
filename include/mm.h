#ifndef	_MM_H
#define	_MM_H

#define PAGE_SHIFT				12
#define TABLE_SHIFT				9
#define SECTION_SHIFT			(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE				(1 << PAGE_SHIFT)
#define SECTION_SIZE			(1 << SECTION_SHIFT)

#define LOW_MEMORY				(2 * SECTION_SIZE)

#define SIMPLE_MALLOC_BASE      0x10000000u
#define SIMPLE_MALLOC_SIZE      (1 * SECTION_SIZE)
#define SIMPLE_MALLOC_END       (SIMPLE_MALLOC_BASE + SIMPLE_MALLOC_SIZE)

#ifndef __ASSEMBLER__

#include <stddef.h>

void memzero(unsigned long src, unsigned long n);
void* mini_malloc(size_t size);

#endif

#endif
