#ifndef _FDT_H
#define _FDT_H

#include <stdint.h>

#define DRIVER_MAX          32

#define FDT_NODE_NAME_MAX	32u
#define FDT_PATH_LEN_MAX    FDT_NODE_NAME_MAX * 32
#define FDT_PATH_DEPTH_MAX  16u

#define FDT_BEGIN_NODE	0x00000001
#define FDT_END_NODE	0x00000002
#define FDT_PROP		0x00000003
#define FDT_NOP			0x00000004
#define FDT_END			0x00000009

typedef struct fdt_header {
	uint32_t magic;
	uint32_t totalsize;
	uint32_t off_dt_struct;
	uint32_t off_dt_strings;
	uint32_t off_mem_rsvmap;
	uint32_t version;
	uint32_t last_comp_version;
	uint32_t boot_cpuid_phys;
	uint32_t size_dt_strings;
	uint32_t size_dt_struct;
} fdt_header_t;

typedef struct fdt_prop {
    uint32_t len;
    uint32_t nameoff;
} fdt_prop_t;

/*
 * Driver callback function prototype
 * Parameters:
 *   token: current FDT token
 *   name:
 *     (A) node name when token is FDT_BEGIN_NODE or FDT_END_NODE
 *     (B) property name when token is FDT_PROP
 *   value: value of the property when token is FDT_PROP
 *   value_len: length of the value of property when token is FDT_PROP
*/
typedef void (*fdt_callback)(uint32_t token,
							 const char* name,
							 const void* value,
							 unsigned int value_len);

void fdt_traverse(void* fdt_base);
void driver_register(fdt_callback cb);

#endif
