#include "peripherals/timer.h"
#include "exception.h"
#include "utils.h"
#include "timer.h"

void timer_init(void) {
    asm volatile("msr cntp_ctl_el0, %0" :: "r"(1));

    timer_clear();
}

void timer_clear(void) {
    unsigned long freq;

    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
    asm volatile("msr cntp_tval_el0, %0" :: "r"(freq));
}

void timer_enable(void) {
    put32(CORE0_TIMER_IRQ_CTL, 2);
}

void timer_disable(void) {
    put32(CORE0_TIMER_IRQ_CTL, 0);
}

void timer_handler(trap_frame_t* regs) {
    unsigned long current_time, freq;

    asm volatile("mrs %0, cntpct_el0" : "=r"(current_time));
    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));

    uart_send_f("Timer interrupt!\n");
    uart_send_f("%d seconds after booting.\n", current_time / freq);

    timer_clear();
}
