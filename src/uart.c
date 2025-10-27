#include "peripherals/uart.h"
#include "peripherals/gpio.h"
#include "utils.h"
#include "uart.h"
#include "utils.h"
#include "config.h"

void uart_init(void)
{
	unsigned int selector;

	selector = get32(GPFSEL1);
	selector &= ~(7 << 12);					// clean gpio14
	selector |= 2 << 12;					// set alt5 for gpio14
	selector &= ~(7 << 15);					// clean gpio15
	selector |= 2 << 15;					// set alt5 for gpio15
	put32(GPFSEL1, selector);

	put32(GPPUD, 0);
	delay(150);
	put32(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
	put32(GPPUDCLK0, 0);

	put32(AUX_ENABLES, 1);					// Enable mini uart (this also enables access to its registers)
	put32(AUX_MU_CNTL_REG, 0);				// Disable auto flow control and disable receiver and transmitter (for now)
	put32(AUX_MU_IER_REG, 0);				// Disable receive and transmit interrupts
	put32(AUX_MU_LCR_REG, 3);				// Enable 8 bit mode
	put32(AUX_MU_MCR_REG, 0);				// Set RTS line to be always high
	put32(AUX_MU_BAUD_REG, 270);			// Set baud rate to 115200

	put32(AUX_MU_CNTL_REG, 3);				// Finally, enable transmitter and receiver
}

void uart_send(char c)
{
	while (1) {
		if (get32(AUX_MU_LSR_REG) & 0x20) break;
	}
	put32(AUX_MU_IO_REG, c);
}

char uart_recv(void)
{
	char received_char;
	while(1) {
		if (get32(AUX_MU_LSR_REG) & 0x01) break;
	}
	received_char = get32(AUX_MU_IO_REG) & 0xFF;
	return received_char == '\r' ? '\n' : received_char;
}

void uart_send_string(const char* str)
{
	for (int i = 0; str[i] != '\0'; i++) {
		uart_send((char)str[i]);
	}
}

void uart_send_f(const char *fmt, ...)
{
	char buffer[UART_BUFFER_SIZE];
	va_list args;

	va_start(args, fmt);
	format_string(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	for (char *p = buffer; *p != '\0'; p++) {
		uart_send(*p);
	}
}
