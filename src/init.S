#include "mm.h"

.section ".text.boot"

.globl _start
_start:
	mov		x19, x0

	mrs		x0, mpidr_el1
	and		x0, x0, #0xFF
	cbz		x0, from_el2_to_el1
	b		proc_hang

proc_hang:
	b		proc_hang

from_el2_to_el1:
	mov		x0, (1 << 31)
    msr		hcr_el2, x0

    mov		x0, 0x3c5
    msr		spsr_el2, x0

	ldr		x0, =LOW_MEMORY
    msr		sp_el1, x0

	adr		x0, master
    msr		elr_el2, x0
    eret

master:
	ldr		x0, =exception_vector_table
    msr		vbar_el1, x0

	adr		x0, bss_begin
	adr		x1, bss_end
	sub		x1, x1, x0
	bl		memzero

	mov		x0, x19
	bl		kernel_main
