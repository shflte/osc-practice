#ifndef _CPIO_H
#define _CPIO_H

#define CPIO_BASE 0x20000000u

#include <stdint.h>

typedef struct cpio_newc_header {
	char c_magic[6];
	char c_ino[8];
	char c_mode[8];
	char c_uid[8];
	char c_gid[8];
	char c_nlink[8];
	char c_mtime[8];
	char c_filesize[8];
	char c_devmajor[8];
	char c_devminor[8];
	char c_rdevmajor[8];
	char c_rdevminor[8];
	char c_namesize[8];
	char c_check[8];
} cpio_header_t;

void cpio_fdt_probe(uint32_t token,
					const char* name,
					const void* value,
					unsigned int value_len);
void cpio_ls(void);
char* cpio_get_file(const char* filename, unsigned int* size);

#endif
