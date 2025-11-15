#include "mm.h"
#include "utils.h"

static void* free_ptr = (void*)SIMPLE_MALLOC_BASE;

void* mini_malloc(size_t size) {
    if (((char*)free_ptr + round_8(size)) >= (char*)SIMPLE_MALLOC_END) return 0;

    void* res = free_ptr;
    free_ptr = (char*)free_ptr + round_8(size);
    return res;
}
