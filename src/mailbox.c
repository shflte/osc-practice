#include "peripherals/mailbox.h"
#include "mailbox.h"
#include "utils.h"

volatile unsigned int __attribute__((aligned(16))) mailbox[8];

unsigned int mailbox_call(unsigned int channel)
{
	unsigned int readChannel = (((unsigned int)((unsigned long)&mailbox) & ~0xF) | (channel & 0xF));

	while (*MAILBOX_STATUS & MAILBOX_FULL) {}

	*MAILBOX_WRITE = readChannel;

	while (1) {
		while (*MAILBOX_STATUS & MAILBOX_EMPTY) {}
		if (readChannel != *MAILBOX_READ) continue;

		if (mailbox[1] == MAILBOX_RESPONSE) {
			return 0;
		} else {
			return -1;
		}
	}
}

unsigned int get_board_revision(unsigned int *revision)
{
	mailbox[0] = 7 * 4;
	mailbox[1] = REQUEST_CODE;
	mailbox[2] = GET_BOARD_REVISION;
	mailbox[3] = 4;
	mailbox[4] = TAG_REQUEST_CODE;
	mailbox[5] = 0;
	mailbox[6] = END_TAG;

	if (mailbox_call(8)) {
		return -1;
	}

	*revision = mailbox[5];
	return 0;
}

unsigned int get_arm_memory(unsigned int *base_address, unsigned int *size)
{
	mailbox[0] = 8 * 4;
	mailbox[1] = REQUEST_CODE;
	mailbox[2] = GET_ARM_MEMORY;
	mailbox[3] = 8;
	mailbox[4] = TAG_REQUEST_CODE;
	mailbox[5] = 0;
	mailbox[6] = 0;
	mailbox[7] = END_TAG;

	if (mailbox_call(8)) {
		return -1;
	}

	*base_address = mailbox[5];
	*size = mailbox[6];
	return 0;
}

unsigned int get_clock_rate(unsigned int *rate, unsigned int clock_id) {
	mailbox[0] = 8 * 4;
	mailbox[1] = REQUEST_CODE;
	mailbox[2] = GET_CLOCK_RATE;
	mailbox[3] = 8;
	mailbox[4] = TAG_REQUEST_CODE;
	mailbox[5] = clock_id;
	mailbox[6] = 0;
	mailbox[7] = END_TAG;

	if (mailbox_call(8)) {
		return -1;
	}

	*rate = mailbox[6];
	return 0;
}

unsigned int set_clock_rate(unsigned int frequency, unsigned int clock_id) {
	mailbox[0] = 9 * 4;
	mailbox[1] = REQUEST_CODE;
	mailbox[2] = SET_CLOCK_RATE;
	mailbox[3] = 12;
	mailbox[4] = TAG_REQUEST_CODE;
	mailbox[5] = clock_id;
	mailbox[6] = frequency;
	mailbox[7] = 1;
	mailbox[8] = END_TAG;

	if (mailbox_call(8)) {
		return -1;
	}

	return 0;
}
