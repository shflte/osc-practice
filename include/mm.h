#ifndef	_MM_H
#define	_MM_H

#define PAGE_SHIFT				12
#define TABLE_SHIFT				9
#define SECTION_SHIFT			(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE				(1 << PAGE_SHIFT)
#define SECTION_SIZE			(1 << SECTION_SHIFT)

#define LOW_MEMORY				(2 * SECTION_SIZE)

#define STARTUP_MALLOC_BASE      0x500000UL
#define STARTUP_MALLOC_SIZE      (1 * SECTION_SIZE)
#define STARTUP_MALLOC_END       (STARTUP_MALLOC_BASE + STARTUP_MALLOC_SIZE)

#define ALLOC_BEGIN				0x10000000UL
#define ALLOC_END				0x20000000UL
#define PAGE_FRAME_NUMBER		((ALLOC_END - ALLOC_BEGIN) / PAGE_SIZE)
#define PAGE_FRAME_USED			0xAABB00
#define PAGE_FRAME_CONT			0xCCDD00
#define PAGE_FRAME_RESV			0xEEFF00
#define MAX_ORDER				11

#define MEM_CHUNK_MIN			16
#define MEM_CHUNK_SIZES			8

#ifndef __ASSEMBLER__

#include <stddef.h>

void memzero(unsigned long src, unsigned long n);

void* startup_malloc(size_t size);

void buddy_init(void);
void* page_alloc(unsigned int order);
void page_free(void* ptr, unsigned int order);

void kmeminit(void);
void* kalloc(size_t size);
void kfree(void* ptr);

#endif
#endif
