#include "fdt.h"
#include "uart.h"
#include "cpio.h"
#include "shell.h"

void kernel_main(void* dtb_base)
{
    driver_register(cpio_fdt_probe);

    fdt_traverse(dtb_base);

    uart_init();

    shell();
}
