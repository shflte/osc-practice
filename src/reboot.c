#include "peripherals/power_management.h"
#include "utils.h"

void reset(int tick) {
	put32(PM_RSTC, PASSWORD | 0x20);
	put32(PM_WDOG, PASSWORD | tick);
}

void cancel_reset() {
	put32(PM_RSTC, PASSWORD | 0);
	put32(PM_WDOG, PASSWORD | 0);
}
