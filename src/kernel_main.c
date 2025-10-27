#include "uart.h"
#include "shell.h"

void kernel_main(void)
{
    uart_init();
    // uart_send_f("Hello, world!\r\n");
    shell();
}
