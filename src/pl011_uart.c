#include "peripherals/pl011_uart.h"
#include "peripherals/gpio.h"
#include "mailbox.h"
#include "utils.h"


void pl011_uart_init(void) {
	unsigned int selector;

	selector = get32(GPFSEL1);
	selector &= ~(7 << 12);								// Clean GPIO14
	selector |= 4 << 12;								// Set alt0 for GPIO14 (TXD0)
	selector &= ~(7 << 15);								// Clean GPIO15
	selector |= 4 << 15;								// Set alt0 for GPIO15 (RXD0)
	put32(GPFSEL1, selector);

	put32(GPPUD, 0);									// Disable pull-up/down
	delay(150);
	put32(GPPUDCLK0, (1 << 14) | (1 << 15));			// Apply pull-up/down setting
	delay(150);
	put32(GPPUDCLK0, 0);								// Reset GPPUDCLK0

	set_clock_rate(48000000, UART_CLOCK_ID);			// Set UART clock to 48MHz

	// Configure UART0 (PL011)
	put32(PU_CR, 0);									// Disable UART0
	put32(PU_ICR, 0x7FF);								// Clear all interrupts
	put32(PU_IBRD, 1);									// Set integer part of baud rate divisor
	put32(PU_FBRD, 40);									// Set fractional part of baud rate divisor
	put32(PU_LCRH, (3 << 5));							// 8-bit, no parity, 1 stop bit
	put32(PU_IMSC, (1 << 4) | (1 << 5) | (1 << 6));		// Enable RX, TX, RT interrupts
	put32(PU_CR, (1 << 0) | (1 << 8) | (1 << 9));		// Enable UART, TX, RX
}

void pl011_uart_send(char c) {
	while (get32(PU_FR) & (1 << 5));
	put32(PU_DR, c);
}

char pl011_uart_recv(void) {
	char received_char;
	while (get32(PU_FR) & (1 << 4));
	received_char = (char)(get32(PU_DR) & 0xFF);
	return received_char == '\r' ? '\n' : received_char;
}
