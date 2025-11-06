#include "uart.h"
#include "utils.h"

#define KERNEL_ADDRESS 0x80000

const char* READY_MSG = "[Bootloader] UART initialized\r\n";
const char* OK_MSG = "[Bootloader] Received length\r\n";

void bootloader_main(void)
{
    uart_init();
    uart_flush_rx();
    uart_send_f(READY_MSG);

    unsigned int len = 0;
    for (int i = 0; i < 4; i++) {
        len |= uart_recv() << i * 8;
    }
    uart_send_f("[Bootloader] Length of kernel: %d\r\n", len);

    char* kernel_dest = (char*)KERNEL_ADDRESS;
    for (unsigned int i = 0; i < len; i++) {
        kernel_dest[i] = uart_recv();
        uart_send('.');
    }
    uart_send_f("[Bootloader] Done transmission.\r\n");

    void (*kernel_entry_point)(void) = (void*)KERNEL_ADDRESS;
    kernel_entry_point();
}
