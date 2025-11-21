#include "exception.h"
#include "timer.h"
#include "utils.h"

void el1_sync_router(trap_frame_t* regs) {
    unsigned long esr;
    asm volatile("mrs %0, esr_el1" : "=r"(esr));
}

void el1_irq_router(trap_frame_t* regs) {

}

void el0_sync_router(trap_frame_t* regs) {
    unsigned long spsr, elr, esr;

    asm volatile("mrs %0, spsr_el1" : "=r"(spsr));
    asm volatile("mrs %0, elr_el1" : "=r"(elr));
    asm volatile("mrs %0, esr_el1" : "=r"(esr));

    uart_send_f("Exception detected!\n");
    uart_send_f("SPSR_EL1: 0x%x\n", spsr);
    uart_send_f("ELR_EL1:  0x%x\n", elr);
    uart_send_f("ESR_EL1:  0x%x\n", esr);
}

void el0_irq_router(trap_frame_t* regs) {
    unsigned long current_time, freq;

    asm volatile("mrs %0, cntpct_el0" : "=r"(current_time));
    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));

    uart_send_f("Timer interrupt!\n");
    uart_send_f("%d seconds after booting.\n", current_time / freq);

    timer_clear();
}
