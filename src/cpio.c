#include <stddef.h>
#include "cpio.h"
#include "utils.h"
#include "fdt.h"

static int g_in_node = 0;
static char* g_cpio_base = 0;

static unsigned int str2size(const char* str) {
    unsigned int size_res = 0;
    unsigned int cur;
    for (int i = 0; i < 8; i++) {
        if (str[i] >= 'a' && str[i] <= 'f') cur = str[i] - 'a' + 10;
        else if (str[i] >= 'A' && str[i] <= 'F') cur = str[i] - 'A' + 10;
        else if (str[i] >= '0' && str[i] <= '9') cur = str[i] - '0';
        else cur = 0;
        size_res <<= 4;
        size_res += cur;
    }
    return size_res;
}

void cpio_fdt_probe(uint32_t token,
					const char* name,
					const void* value,
					unsigned int value_len) {
    switch(token) {
        case FDT_BEGIN_NODE:
            if (strcmp(name, "chosen") == 0) g_in_node = 1;
            break;
        case FDT_END_NODE:
            if (strcmp(name, "chosen") == 0) g_in_node = 0;
            break;
        case FDT_PROP:
            if (g_in_node && strcmp(name, "linux,initrd-start") == 0) {
                if (value_len == 4) {
                    g_cpio_base = (char*)(uintptr_t)b2l_32(*((uint32_t*)value));
                } else if (value_len == 8) {
                    g_cpio_base = (char*)(uintptr_t)b2l_64(*((uint64_t*)value));
                }
            }
            break;
        default:
            break;
    }
}

void cpio_ls(void) {
    char* cur_file = (char*)CPIO_BASE;
    unsigned int namesize = 0, filesize = 0;

    while (1) {
        namesize = str2size(((cpio_header_t*)cur_file)->c_namesize);
        filesize = str2size(((cpio_header_t*)cur_file)->c_filesize);

        if (strncmp(cur_file + sizeof(cpio_header_t), "TRAILER!!!", 10) == 0) break;
        uart_send_f("%s\n", cur_file + sizeof(cpio_header_t));
        cur_file += round_4(sizeof(cpio_header_t) + namesize) + round_4(filesize);
    }
}

char* cpio_get_file(const char* filename, unsigned int* size) {
    char* cur_file = (char*)CPIO_BASE;
    unsigned int namesize = 0, filesize = 0;

    *size = 0;
    while (1) {
        namesize = str2size(((cpio_header_t*)cur_file)->c_namesize);
        filesize = str2size(((cpio_header_t*)cur_file)->c_filesize);

        if (strncmp(cur_file + sizeof(cpio_header_t), "TRAILER!!!", 10) == 0) return 0;
        if (strlen(filename) == strlen(cur_file + sizeof(cpio_header_t)) &&
            strncmp(cur_file + sizeof(cpio_header_t), filename, strlen(filename)) == 0) {
            *size = filesize;
            return cur_file + round_4(sizeof(cpio_header_t) + namesize);
        }
        cur_file += round_4(sizeof(cpio_header_t) + namesize) + round_4(filesize);
    }
}
