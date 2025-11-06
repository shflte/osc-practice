ENTRY(_start)
SECTIONS
{
	. = 0x60000;
	bl_begin = .;
	.text : {
		*(.text.relo)
		bl_entry = .;
		*(.text.boot)
		*(.text)
		*(.rodata)
	}
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) }
	bss_end = .;
	bl_end = .;
	bl_size = bl_end - bl_begin;
}
