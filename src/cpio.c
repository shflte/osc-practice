#include <stddef.h>
#include "cpio.h"
#include "uart.h"
#include "utils.h"

#define round_4(number) ((number + 3) / 4 * 4)

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
