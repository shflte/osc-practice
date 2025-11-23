#include "peripherals/uart.h"
#include "peripherals/gpio.h"
#include "utils.h"
#include "uart.h"
#include "exception.h"
#include <stddef.h>

static ringbuffer_t rx_buffer;
static ringbuffer_t tx_buffer;

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
	put32(AUX_MU_IER_REG, 1);				// Enable receive interrupts
	put32(AUX_MU_LCR_REG, 3);				// Enable 8 bit mode
	put32(AUX_MU_MCR_REG, 0);				// Set RTS line to be always high
	put32(AUX_MU_BAUD_REG, 270);			// Set baud rate to 115200

	put32(AUX_MU_IIR_REG, 6);
	put32(AUX_MU_CNTL_REG, 3);				// Finally, enable transmitter and receiver

	rb_init(&rx_buffer);
	rb_init(&tx_buffer);
}

void uart_send(char c)
{
	if (c == '\n') {
        uart_send('\r');
    }

	unsigned int ier;
	ier = get32(AUX_MU_IER_REG);

	while (rb_push(&tx_buffer, c) != 0) {
        put32(AUX_MU_IER_REG, ier | 1 << 1);
        // asm volatile("wfi");
    }

	put32(AUX_MU_IER_REG, ier | 1 << 1);
}

char uart_recv(void)
{
	char c;
	while (rb_pop(&rx_buffer, &c) != 0) {
        // asm volatile("wfi");
    }
	return c;
}

void uart_handler(trap_frame_t* regs) {
	unsigned int iir, ier;
	char c;
	int status;

	iir = get32(AUX_MU_IIR_REG);
	ier = get32(AUX_MU_IER_REG);
	if ((iir & 0x06) == 0x04) { // r
		c = get32(AUX_MU_IO_REG) & 0xFF;
		rb_push(&rx_buffer, c);
	} else if ((iir & 0x06) == 0x02) { // t
		status = rb_pop(&tx_buffer, &c);
		if (status != 0) {
			put32(AUX_MU_IER_REG, ier & ~(1 << 1));
		} else {
			put32(AUX_MU_IO_REG, c);
		}
	} else {
		// No interrupt or error
	}
}
