#include "uart.h"
#include "shell.h"


void kernel_main(void)
{
    uart_init();
    shell();
}
