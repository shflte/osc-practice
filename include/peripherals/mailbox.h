#ifndef _PERIPHERAL_MAILBOX_H
#define _PERIPHERAL_MAILBOX_H

#include "peripherals/base.h"

#define MAILBOX_BASE		MMIO_BASE + 0xb880
#define MAILBOX_READ		((volatile unsigned int *)(MAILBOX_BASE))
#define MAILBOX_STATUS		((volatile unsigned int *)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE		((volatile unsigned int *)(MAILBOX_BASE + 0x20))

#endif
