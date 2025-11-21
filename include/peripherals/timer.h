#ifndef _PERIPHERALS_TIMER_H
#define _PERIPHERALS_TIMER_H

#include "peripherals/base.h"

#define CORE_BASE				0x40000000
#define CORE0_TIMER_IRQ_CTL		((volatile unsigned int *)(CORE_BASE + 0x40))
#define CORE0_INTERRUPT_SOURCE	((volatile unsigned int *)(CORE_BASE + 0x60))

#endif
