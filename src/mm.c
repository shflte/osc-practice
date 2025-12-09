#include "mm.h"
#include "utils.h"

/* START UP MEMORY ALLOCATION */
static void* free_ptr = (void*)STARTUP_MALLOC_BASE;

void* startup_malloc(size_t size) {
    if (((char*)free_ptr + round_8(size)) >= (char*)STARTUP_MALLOC_END) return 0;

    void* res = free_ptr;
    free_ptr = (char*)free_ptr + round_8(size);
    return res;
}

/* BUDDY SYSTEM */
typedef struct page {
	unsigned int val;
	struct page* next;
} page_t;
static page_t* pages = 0;
static page_t* free_lists[MAX_ORDER + 1];

void buddy_init(void) {
	pages = startup_malloc(sizeof(page_t) * PAGE_FRAME_NUMBER);
	for (int i = 0; i <= MAX_ORDER; i++) {
		free_lists[i] = 0;
	}

	for (unsigned int i = 0; i < PAGE_FRAME_NUMBER; i++) {
		if (i % (1 << MAX_ORDER) == 0) {
			pages[i].val = 1 << MAX_ORDER;
			pages[i].next = free_lists[MAX_ORDER];
			free_lists[MAX_ORDER] = &pages[i];
		} else {
			pages[i].val = PAGE_FRAME_CONT;
			pages[i].next = 0;
		}
	}
}

void* page_alloc(unsigned int order) {
	unsigned split_from = 0;
	for (int i = order; i <= MAX_ORDER; i++) {
		if (free_lists[i]) {
			split_from = i;
			break;
		}
	}

	unsigned int res_pfn = free_lists[split_from] - pages;
	unsigned int cur_pfn;
	free_lists[split_from] = free_lists[split_from]->next;
	for (unsigned int i = split_from; i > order; i--) {
		cur_pfn = res_pfn + (1 << (i - 1));

		pages[cur_pfn].val = 1 << (i - 1);

		pages[cur_pfn].next = free_lists[i - 1];
		free_lists[i - 1] = &pages[cur_pfn];
	}

	pages[res_pfn].val = PAGE_FRAME_USED;
	return (void*)(ALLOC_BEGIN + (res_pfn * PAGE_SIZE));
}

void page_free(void* ptr, unsigned int order) {
	unsigned int pfn = ((unsigned long)ptr - ALLOC_BEGIN) / PAGE_SIZE;
	unsigned int cur_order = order;

	unsigned int buddy;
	page_t** indirect = 0;
	while (cur_order < MAX_ORDER) {
		buddy = pfn ^ (1 << cur_order);
		if (pages[buddy].val != (unsigned int)(1 << cur_order)) break;

		pages[pfn].val = PAGE_FRAME_CONT;
		pages[buddy].val = PAGE_FRAME_CONT;

		indirect = &free_lists[cur_order];
		while (*indirect && *indirect != &pages[buddy]) indirect = &(*indirect)->next;
		*indirect = (*indirect)->next;

		pfn &= buddy;
		cur_order++;
	}

	pages[pfn].val = 1 << cur_order;

	pages[pfn].next = free_lists[cur_order];
	free_lists[cur_order] = &pages[pfn];
}
