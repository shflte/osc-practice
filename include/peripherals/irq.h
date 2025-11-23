#ifndef _PERIPHERALS_IRQ_H
#define _PERIPHERALS_IRQ_H

#include "peripherals/base.h"

#define IRQ_BASE			MMIO_BASE + 0xB000
#define IRQ_BASIC_PENDING	((volatile unsigned int *)(IRQ_BASE + 0x200))
#define IRQ_PENDING_1		((volatile unsigned int *)(IRQ_BASE + 0x204))
#define IRQ_PENDING_2		((volatile unsigned int *)(IRQ_BASE + 0x208))
#define FIQ_CONTROL			((volatile unsigned int *)(IRQ_BASE + 0x20C))
#define ENABLE_IRQS_1		((volatile unsigned int *)(IRQ_BASE + 0x210))
#define ENABLE_IRQS_2		((volatile unsigned int *)(IRQ_BASE + 0x214))
#define ENABLE_BASIC_IRQS	((volatile unsigned int *)(IRQ_BASE + 0x218))
#define DISABLE_IRQS_1		((volatile unsigned int *)(IRQ_BASE + 0x21C))
#define DISABLE_IRQS_2		((volatile unsigned int *)(IRQ_BASE + 0x220))
#define DISABLE_BASIC_IRQS	((volatile unsigned int *)(IRQ_BASE + 0x224))

#endif
