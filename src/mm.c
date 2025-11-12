#include "mm.h"

#define round_8(number) ((number + 7) / 8 * 8)

static void* free_ptr = (void*)SIMPLE_MALLOC_BASE;

void* mini_malloc(size_t size) {
    if (((char*)free_ptr + round_8(size)) >= (char*)SIMPLE_MALLOC_END) return 0;

    void* res = free_ptr;
    free_ptr = (char*)free_ptr + round_8(size);
    return res;
}
