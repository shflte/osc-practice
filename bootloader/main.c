#include "peripherals/uart.h"
#include "uart.h"
#include "utils.h"

#define KERNEL_ADDRESS 0x80000

const char* READY_MSG = "[Bootloader] UART initialized\n";
const char* OK_MSG = "[Bootloader] Received length\n";

void bootloader_main(void* dtb_base)
{
    uart_init();
    uart_send_f(READY_MSG);

    unsigned int len = 0;
    char recved;
    for (int i = 0; i < 4; i++) {
        recved = uart_recv();
        if (recved == 0xFC || recved == 0xFE) {
            i--;
            continue;
        }
        len |= recved << i * 8;
    }
    uart_send_f("[Bootloader] Length of kernel: %d\n", len);

    char* kernel_dest = (char*)KERNEL_ADDRESS;
    unsigned int checksum = 0;
    char c;
    for (unsigned int i = 0; i < len; i++) {
        c = uart_recv();
        kernel_dest[i] = c;
        checksum += (unsigned char)c;
    }
    uart_send_f("[Bootloader] Done transmission. Checksum: 0x%x\n", checksum);
    while (!(get32(AUX_MU_LSR_REG) & (1 << 6))) {
        ;
    }

    void (*kernel_entry_point)(void*) = (void*)KERNEL_ADDRESS;
    kernel_entry_point(dtb_base);
}
