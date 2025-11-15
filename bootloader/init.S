#include "mm.h"

.section ".text.relo"
.globl _start
_start:
	mov		x19, x0

	ldr		x10, =bl_size
	adr		x11, _start
	ldr		x12, =bl_begin

relo_loop:
	cmp		x10, #0
	beq		done_loop
	ldr		x13, [x11], #8
	str		x13, [x12], #8
	sub		x10, x10, #8
	b		relo_loop

done_loop:
	ldr		x10, =bl_entry
	br		x10

.section ".text.boot"
check_cores:
	mrs		x10, mpidr_el1
	and		x10, x10, #0xFF
	cbz		x10, master
	b		proc_hang

proc_hang:
	b		proc_hang

master:
	adr		x10, bss_begin
	adr		x11, bss_end
	sub		x11, x11, x10
	bl		memzero

	mov		sp, #LOW_MEMORY
	mov		x0, x19
	bl		bootloader_main
