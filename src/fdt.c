#include "fdt.h"
#include "utils.h"
#include "mm.h"

static fdt_callback driver_cb_list[DRIVER_MAX];
static int driver_count = 0;

static char* fdt_path[FDT_PATH_DEPTH_MAX];
static int path_cnt = 0;

static void broadcast_cb(uint32_t token,
                         const char* name,
                         const void* value,
                         unsigned int value_len) {
    for (int i = 0; i < driver_count; i++) {
        driver_cb_list[i](token, name, value, value_len);
    }
}

void fdt_traverse(void* fdt_base) {
    if (b2l_32(((fdt_header_t*)fdt_base)->magic) != 0xd00dfeed) return;
    void* ptr = (char*)fdt_base + b2l_32(((fdt_header_t*)fdt_base)->off_dt_struct);  // structure pointer
    const char* strings = (char*)fdt_base + b2l_32(((fdt_header_t*)fdt_base)->off_dt_strings);

    uint32_t token = 0;
    while (1) {
        token = b2l_32(*((uint32_t*)ptr));
        ptr = (char*)ptr + 4;
        switch(token) {
            case FDT_BEGIN_NODE:
                fdt_path[path_cnt++] = (char*)ptr;
                broadcast_cb(token, fdt_path[path_cnt - 1], 0, 0);
                ptr = (char*)ptr + round_4(strlen((char*)ptr) + 1);
                break;
            case FDT_END_NODE:
                broadcast_cb(token, fdt_path[path_cnt - 1], 0, 0);
                path_cnt--;
                break;
            case FDT_PROP:
                broadcast_cb(token,
                             strings + b2l_32(((fdt_prop_t*)ptr)->nameoff),
                             ptr + sizeof(fdt_prop_t),
                             b2l_32(((fdt_prop_t*)ptr)->len));
                ptr = (char*)ptr + sizeof(fdt_prop_t) + round_4(b2l_32(((fdt_prop_t*)ptr)->len));
                break;
            case FDT_NOP:
                break;
            case FDT_END:
                return;
            default:
                return;
        }
    }
}

void driver_register(fdt_callback cb) {
    if (driver_count + 1 > DRIVER_MAX) return;
    driver_cb_list[driver_count++] = cb;
}
