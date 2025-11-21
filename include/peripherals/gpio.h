#ifndef	_PERIPHERAL_GPIO_H
#define	_PERIPHERAL_GPIO_H

#include "peripherals/base.h"

#define GPIO_BASE           MMIO_BASE + 0x00200000
#define GPFSEL1				((volatile unsigned int *)(GPIO_BASE + 0x04))
#define GPSET0				((volatile unsigned int *)(GPIO_BASE + 0x1C))
#define GPCLR0				((volatile unsigned int *)(GPIO_BASE + 0x28))
#define GPPUD				((volatile unsigned int *)(GPIO_BASE + 0x94))
#define GPPUDCLK0			((volatile unsigned int *)(GPIO_BASE + 0x98))

#endif
