#ifndef _MAILBOX_H
#define _MAILBOX_H

#define MAILBOX_EMPTY		0x40000000
#define MAILBOX_FULL		0x80000000

#define GET_BOARD_REVISION	0x00010002
#define GET_ARM_MEMORY		0x00010005
#define GET_CLOCK_RATE		0x00030002
#define SET_CLOCK_RATE		0x00038002

#define UART_CLOCK_ID		0x00000002

#define REQUEST_CODE		0x00000000
#define REQUEST_SUCCEED		0x80000000
#define REQUEST_FAILED		0x80000001
#define MAILBOX_RESPONSE	0x80000000
#define TAG_REQUEST_CODE	0x00000000
#define END_TAG				0x00000000

unsigned int mailbox_call(unsigned int channel);
unsigned int get_board_revision(unsigned int *revision);
unsigned int get_arm_memory(unsigned int *base_address, unsigned int *size);
unsigned int get_clock_rate(unsigned int *rate, unsigned int clock_id);
unsigned int set_clock_rate(unsigned int frequency, unsigned int clock_id);

#endif
