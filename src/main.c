#include "fdt.h"
#include "uart.h"
#include "cpio.h"
#include "shell.h"
#include "timer.h"
#include "irq.h"
#include "utils.h"

void kernel_main(void* dtb_base)
{
    driver_register(cpio_fdt_probe);

    fdt_traverse(dtb_base);

    uart_init();

    irq_enable(AUX_IRQ);

    timer_init();
    enable_cpu_irq();

    shell();
}
