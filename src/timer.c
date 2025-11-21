#include "peripherals/timer.h"
#include "utils.h"
#include "timer.h"

void timer_init(void) {
    asm volatile("msr cntp_ctl_el0, %0" :: "r"(1));

    timer_clear();

    put32(CORE0_TIMER_IRQ_CTL, 2);
}

void timer_clear(void) {
    unsigned long freq;

    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
    asm volatile("msr cntp_tval_el0, %0" :: "r"(freq));
}
