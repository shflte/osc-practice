#ifndef _PERIPHERAL_POWER_MANAGEMENT_H
#define _PERIPHERAL_POWER_MANAGEMENT_H

#include "peripherals/base.h"

#define PM_BASE				MMIO_BASE + 0x00100000
#define PM_RSTC				((volatile unsigned int *)(PM_BASE + 0x1C))
#define PM_WDOG				((volatile unsigned int *)(PM_BASE + 0x24))

#define PASSWORD			0x5A000000

#endif
